#include "qtteachingtip.h"
#include "qttheme.h"
#include <qapplication.h>
#include <qevent.h>
#include <qgraphicseffect.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>
#include <qscreen.h>
#include <qvariantanimation.h>

class QtTeachingTipPrivate
{
    Q_DECLARE_PUBLIC(QtTeachingTip)
public:
    int borderRadius;
    QString title;
    QString subTitle;
    QString content;
    QtIconType::IconName tipIcon;
    QPixmap heroImage;
    bool isLightDismiss;
    qreal opacity;
    QtTeachingTip* q_ptr;
    struct ActionItem
    {
        QString text;
        std::function<void()> callback;
    };

    QtTeachingTip::TailPosition tailPosition{ QtTeachingTip::Auto };
    QWidget* target{ nullptr };
    bool closeButtonVisible{ true };
    QList<ActionItem> actions;
    QRect closeButtonRect;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    int shadowBorderWidth{ 0 };
#else
    int shadowBorderWidth{ 8 };
#endif
    int tailSize{ 8 };
    QtThemeType::ThemeMode themeMode;
    QtTeachingTipPrivate(QtTeachingTip* q);
    ~QtTeachingTipPrivate();
    QPoint calculatePosition() const;
    QtTeachingTip::TailPosition calculateAutoTailPosition() const;
    void doShowAnimation();
};

QtTeachingTipPrivate::QtTeachingTipPrivate(QtTeachingTip* q)
    : q_ptr(q)
{
}
QtTeachingTipPrivate::~QtTeachingTipPrivate()
{
}

QPoint QtTeachingTipPrivate::calculatePosition() const
{
    Q_Q(const QtTeachingTip);
    if(!target)
    {
        return QPoint(0, 0);
    }
    QPoint targetCenter = target->mapToGlobal(QPoint(target->width() / 2, target->height() / 2));
    QtTeachingTip::TailPosition pos = tailPosition;
    if(pos == QtTeachingTip::Auto)
    {
        pos = calculateAutoTailPosition();
    }

    int gap = tailSize + 4;
    QPoint result;
    switch(pos)
    {
        case QtTeachingTip::Bottom:
            // Tip appears above target, tail points down
            result = QPoint(targetCenter.x() - q->width() / 2,
                target->mapToGlobal(QPoint(0, 0)).y() - q->height() - gap);
            break;
        case QtTeachingTip::Top:
            // Tip appears below target, tail points up
            result = QPoint(targetCenter.x() - q->width() / 2,
                target->mapToGlobal(QPoint(0, target->height())).y() + gap);
            break;
        case QtTeachingTip::Right:
            // Tip appears left of target, tail points right
            result = QPoint(target->mapToGlobal(QPoint(0, 0)).x() - q->width() - gap,
                targetCenter.y() - q->height() / 2);
            break;
        case QtTeachingTip::Left:
            // Tip appears right of target, tail points left
            result = QPoint(target->mapToGlobal(QPoint(target->width(), 0)).x() + gap,
                targetCenter.y() - q->height() / 2);
            break;
        default:
            result = QPoint(targetCenter.x() - q->width() / 2,
                target->mapToGlobal(QPoint(0, 0)).y() - q->height() - gap);
            break;
    }

    // Screen boundary clamping
    QScreen* screen = QApplication::screenAt(targetCenter);
    if(screen)
    {
        QRect screenGeo = screen->availableGeometry();
        result.setX(qBound(screenGeo.left() + 4, result.x(), screenGeo.right() - q->width() - 4));
        result.setY(qBound(screenGeo.top() + 4, result.y(), screenGeo.bottom() - q->height() - 4));
    }
    return result;
}
QtTeachingTip::TailPosition QtTeachingTipPrivate::calculateAutoTailPosition() const
{
    Q_Q(const QtTeachingTip);
    if(!target)
    {
        return QtTeachingTip::Bottom;
    }
    QPoint targetGlobal = target->mapToGlobal(QPoint(0, 0));
    QScreen* screen = QApplication::screenAt(targetGlobal);
    if(!screen)
    {
        return QtTeachingTip::Bottom;
    }
    QRect screenGeo = screen->availableGeometry();
    int spaceAbove = targetGlobal.y() - screenGeo.top();
    int spaceBelow = screenGeo.bottom() - targetGlobal.y() - target->height();

    // Prefer showing above (tail at bottom)
    if(spaceAbove >= q->sizeHint().height() + tailSize + 4)
    {
        return QtTeachingTip::Bottom;
    }
    if(spaceBelow >= q->sizeHint().height() + tailSize + 4)
    {
        return QtTeachingTip::Top;
    }
    return QtTeachingTip::Bottom;
}
void QtTeachingTipPrivate::doShowAnimation()
{
    Q_Q(QtTeachingTip);
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(q);
    q->setGraphicsEffect(effect);
    opacity = 0.0;
    effect->setOpacity(0);

    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity", q);
    animation->setDuration(200);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    QObject::connect(
        animation, &QPropertyAnimation::finished, q, [=]() { q->setGraphicsEffect(nullptr); });
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QtTeachingTip::QtTeachingTip(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtTeachingTipPrivate(this))
{
    Q_UNUSED(parent);
    Q_D(QtTeachingTip);
    d->q_ptr = this;
    d->borderRadius = 8;
    d->title = "";
    d->subTitle = "";
    d->content = "";
    d->tipIcon = QtIconType::None;
    d->isLightDismiss = true;
    d->opacity = 1.0;
    setObjectName("QtTeachingTip");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setFixedWidth(320);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    hide();
}

QtTeachingTip::~QtTeachingTip()
{
}

void QtTeachingTip::setTailPosition(TailPosition position)
{
    Q_D(QtTeachingTip);
    d->tailPosition = position;
}

QtTeachingTip::TailPosition QtTeachingTip::tailPosition() const
{
    return d_ptr->tailPosition;
}

void QtTeachingTip::setTarget(QWidget* target)
{
    Q_D(QtTeachingTip);
    if(d->target)
    {
        d->target->removeEventFilter(this);
    }
    d->target = target;
    if(target)
    {
        target->installEventFilter(this);
    }
}

QWidget* QtTeachingTip::target() const
{
    return d_ptr->target;
}

void QtTeachingTip::setCloseButtonVisible(bool visible)
{
    Q_D(QtTeachingTip);
    d->closeButtonVisible = visible;
    update();
}

void QtTeachingTip::addAction(const QString& text, const std::function<void()>& callback)
{
    Q_D(QtTeachingTip);
    d->actions.append({ text, callback });
}

void QtTeachingTip::clearActions()
{
    Q_D(QtTeachingTip);
    d->actions.clear();
}

void QtTeachingTip::showTip()
{
    Q_D(QtTeachingTip);
    if(d->isLightDismiss)
    {
        qApp->installEventFilter(this);
    }
    // Calculate size based on content
    int contentHeight = d->shadowBorderWidth * 2;
    if(!d->heroImage.isNull())
    {
        QPixmap scaled = d->heroImage.scaledToWidth(
            width() - d->shadowBorderWidth * 2, Qt::SmoothTransformation);
        contentHeight += scaled.height();
    }
    contentHeight += 12; // top padding
    if(!d->title.isEmpty())
    {
        contentHeight += 24;
    }
    if(!d->subTitle.isEmpty())
    {
        contentHeight += 20;
    }
    if(!d->content.isEmpty())
    {
        QFont contentFont = font();
        contentFont.setPixelSize(13);
        QFontMetrics fm(contentFont);
        int textWidth = width() - d->shadowBorderWidth * 2 - 30;
        QRect boundingRect =
            fm.boundingRect(QRect(0, 0, textWidth, 0), Qt::TextWordWrap, d->content);
        contentHeight += boundingRect.height() + 8;
    }
    if(!d->actions.isEmpty())
    {
        contentHeight += 44;
    }
    contentHeight += d->shadowBorderWidth + 4; // bottom padding
    setFixedHeight(contentHeight);

    QPoint pos = d->calculatePosition();
    move(pos);
    show();
    raise();
    d->doShowAnimation();
}

void QtTeachingTip::closeTip()
{
    Q_D(QtTeachingTip);
    qApp->removeEventFilter(this);
    hide();
    Q_EMIT closed();
}

void QtTeachingTip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtTeachingTip);
    QPainter painter(this);
    painter.setRenderHints(
        QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    // Shadow
    qtTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
    // Main body
    QRect bodyRect = rect().adjusted(
        d->shadowBorderWidth, d->shadowBorderWidth, -d->shadowBorderWidth, -d->shadowBorderWidth);
    painter.setPen(QPen(QtThemeColor(d->themeMode, PopupBorder), 1));
    painter.setBrush(QtThemeColor(d->themeMode, PopupBase));
    painter.drawRoundedRect(bodyRect, d->borderRadius, d->borderRadius);

    int currentY = d->shadowBorderWidth;
    int contentLeft = d->shadowBorderWidth + 15;
    int contentWidth = bodyRect.width() - 30;

    // Hero image
    if(!d->heroImage.isNull())
    {
        QPixmap scaled = d->heroImage.scaledToWidth(bodyRect.width(), Qt::SmoothTransformation);
        QRect imageRect(d->shadowBorderWidth, currentY, bodyRect.width(), scaled.height());

        // Clip to rounded top corners
        QPainterPath clipPath;
        clipPath.addRoundedRect(QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(),
                                    scaled.height() + d->borderRadius),
            d->borderRadius, d->borderRadius);
        painter.save();
        painter.setClipPath(clipPath);
        painter.drawPixmap(imageRect, scaled);
        painter.restore();

        currentY += scaled.height();
    }

    currentY += 12;

    // Close button
    if(d->closeButtonVisible)
    {
        QFont closeFont = QFont("ElaAwesome");
        closeFont.setPixelSize(12);
        painter.setFont(closeFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicDetailsText));
        d->closeButtonRect = QRect(bodyRect.right() - 28, d->shadowBorderWidth + 8, 20, 20);
        painter.drawText(
            d->closeButtonRect, Qt::AlignCenter, QChar((unsigned short) QtIconType::Xmark));
    }

    // Icon + Title
    if(!d->title.isEmpty())
    {
        int titleX = contentLeft;
        if(d->tipIcon != QtIconType::None)
        {
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(16);
            painter.setFont(iconFont);
            painter.setPen(QtThemeColor(d->themeMode, PrimaryNormal));
            painter.drawText(QRect(contentLeft, currentY, 20, 22), Qt::AlignVCenter | Qt::AlignLeft,
                QChar((unsigned short) d->tipIcon));
            titleX += 26;
        }
        QFont titleFont = font();
        titleFont.setPixelSize(14);
        titleFont.setBold(true);
        painter.setFont(titleFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        painter.drawText(QRect(titleX, currentY, contentWidth - (titleX - contentLeft) - 25, 22),
            Qt::AlignVCenter | Qt::AlignLeft, d->title);
        currentY += 24;
    }

    // SubTitle
    if(!d->subTitle.isEmpty())
    {
        QFont subFont = font();
        subFont.setPixelSize(12);
        painter.setFont(subFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicDetailsText));
        painter.drawText(QRect(contentLeft, currentY, contentWidth, 18),
            Qt::AlignVCenter | Qt::AlignLeft, d->subTitle);
        currentY += 20;
    }

    // Content text (word wrap)
    if(!d->content.isEmpty())
    {
        currentY += 4;
        QFont contentFont = font();
        contentFont.setPixelSize(13);
        painter.setFont(contentFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        QRect textRect(
            contentLeft, currentY, contentWidth, height() - currentY - d->shadowBorderWidth);
        painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, d->content);
        QFontMetrics fm(contentFont);
        QRect boundingRect = fm.boundingRect(textRect, Qt::TextWordWrap, d->content);
        currentY += boundingRect.height() + 4;
    }

    // Action buttons area
    if(!d->actions.isEmpty())
    {
        currentY += 8;
        int buttonWidth = 80;
        int buttonHeight = 30;
        int buttonSpacing = 8;
        int totalWidth = d->actions.size() * buttonWidth + (d->actions.size() - 1) * buttonSpacing;
        int startX = bodyRect.right() - 15 - totalWidth;

        QFont buttonFont = font();
        buttonFont.setPixelSize(12);
        painter.setFont(buttonFont);

        for(int i = 0; i < d->actions.size(); ++i)
        {
            QRect btnRect(
                startX + i * (buttonWidth + buttonSpacing), currentY, buttonWidth, buttonHeight);
            if(i == d->actions.size() - 1)
            {
                // Primary button (last)
                painter.setPen(Qt::NoPen);
                painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                painter.drawRoundedRect(btnRect, 4, 4);
                painter.setPen(Qt::white);
            }
            else
            {
                painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), 1));
                painter.setBrush(QtThemeColor(d->themeMode, BasicBase));
                painter.drawRoundedRect(btnRect, 4, 4);
                painter.setPen(QtThemeColor(d->themeMode, BasicText));
            }
            painter.drawText(btnRect, Qt::AlignCenter, d->actions[i].text);
        }
    }

    // Tail/arrow
    if(d->target)
    {
        QtTeachingTip::TailPosition tailPos = d->tailPosition;
        if(tailPos == QtTeachingTip::Auto)
        {
            tailPos = d->calculateAutoTailPosition();
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(QtThemeColor(d->themeMode, PopupBase));

        QPolygonF triangle;
        int ts = d->tailSize;
        switch(tailPos)
        {
            case QtTeachingTip::Bottom: {
                // Tail at bottom center
                int cx = width() / 2;
                int by = bodyRect.bottom();
                triangle << QPointF(cx - ts, by) << QPointF(cx + ts, by) << QPointF(cx, by + ts);
                break;
            }
            case QtTeachingTip::Top: {
                int cx = width() / 2;
                int ty = bodyRect.top();
                triangle << QPointF(cx - ts, ty) << QPointF(cx + ts, ty) << QPointF(cx, ty - ts);
                break;
            }
            case QtTeachingTip::Left: {
                int cy = height() / 2;
                int lx = bodyRect.left();
                triangle << QPointF(lx, cy - ts) << QPointF(lx, cy + ts) << QPointF(lx - ts, cy);
                break;
            }
            case QtTeachingTip::Right: {
                int cy = height() / 2;
                int rx = bodyRect.right();
                triangle << QPointF(rx, cy - ts) << QPointF(rx, cy + ts) << QPointF(rx + ts, cy);
                break;
            }
            default:
                break;
        }
        if(!triangle.isEmpty())
        {
            QPainterPath tailPath;
            tailPath.addPolygon(triangle);
            tailPath.closeSubpath();
            painter.drawPath(tailPath);
        }
    }
}

bool QtTeachingTip::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtTeachingTip);
    if(watched == d->target)
    {
        if(event->type() == QEvent::Move || event->type() == QEvent::Resize)
        {
            if(isVisible())
            {
                move(d->calculatePosition());
            }
        }
        else if(event->type() == QEvent::Hide || event->type() == QEvent::Destroy)
        {
            closeTip();
        }
    }
    // Light dismiss: click outside closes the tip
    if(d->isLightDismiss && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(watched != this &&
            !this->geometry().contains(this->mapFromGlobal(mouseEvent->globalPosition().toPoint())))
        {
            closeTip();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void QtTeachingTip::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
}

void QtTeachingTip::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtTeachingTip);
    // Close button hit test
    if(d->closeButtonVisible && d->closeButtonRect.contains(event->pos()))
    {
        Q_EMIT closeButtonClicked();
        closeTip();
        return;
    }
    // Action button hit test
    if(!d->actions.isEmpty())
    {
        QRect bodyRect = rect().adjusted(d->shadowBorderWidth, d->shadowBorderWidth,
            -d->shadowBorderWidth, -d->shadowBorderWidth);
        int buttonWidth = 80;
        int buttonHeight = 30;
        int buttonSpacing = 8;
        int totalWidth = d->actions.size() * buttonWidth + (d->actions.size() - 1) * buttonSpacing;
        int startX = bodyRect.right() - 15 - totalWidth;

        // Approximate button Y
        int buttonY = height() - d->shadowBorderWidth - 12 - buttonHeight;

        for(int i = 0; i < d->actions.size(); ++i)
        {
            QRect btnRect(
                startX + i * (buttonWidth + buttonSpacing), buttonY, buttonWidth, buttonHeight);
            if(btnRect.contains(event->pos()))
            {
                if(d->actions[i].callback)
                {
                    d->actions[i].callback();
                }
                return;
            }
        }
    }
    QWidget::mousePressEvent(event);
}

int QtTeachingTip::borderRadius() const
{
    Q_D(const QtTeachingTip);
    return d->borderRadius;
}
void QtTeachingTip::setBorderRadius(int value)
{
    Q_D(QtTeachingTip);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}
QString QtTeachingTip::title() const
{
    Q_D(const QtTeachingTip);
    return d->title;
}
void QtTeachingTip::setTitle(QString value)
{
    Q_D(QtTeachingTip);
    d->title = value;
    Q_EMIT titleChanged();
}
QString QtTeachingTip::subTitle() const
{
    Q_D(const QtTeachingTip);
    return d->subTitle;
}
void QtTeachingTip::setSubTitle(QString value)
{
    Q_D(QtTeachingTip);
    d->subTitle = value;
    Q_EMIT subTitleChanged();
}
QString QtTeachingTip::content() const
{
    Q_D(const QtTeachingTip);
    return d->content;
}
void QtTeachingTip::setContent(QString value)
{
    Q_D(QtTeachingTip);
    d->content = value;
    Q_EMIT contentChanged();
}
QtIconType::IconName QtTeachingTip::tipIcon() const
{
    Q_D(const QtTeachingTip);
    return d->tipIcon;
}
void QtTeachingTip::setTipIcon(QtIconType::IconName value)
{
    Q_D(QtTeachingTip);
    d->tipIcon = value;
    Q_EMIT tipIconChanged();
}
QPixmap QtTeachingTip::heroImage() const
{
    Q_D(const QtTeachingTip);
    return d->heroImage;
}
void QtTeachingTip::setHeroImage(QPixmap value)
{
    Q_D(QtTeachingTip);
    d->heroImage = value;
    Q_EMIT heroImageChanged();
}
bool QtTeachingTip::isLightDismiss() const
{
    Q_D(const QtTeachingTip);
    return d->isLightDismiss;
}
void QtTeachingTip::setIsLightDismiss(bool value)
{
    Q_D(QtTeachingTip);
    d->isLightDismiss = value;
    Q_EMIT isLightDismissChanged();
}
// int
// QString
// QString
// QString
// QtIconType::IconName
// QPixmap
// bool

// borderRadius
// title
// subTitle
// content
// tipIcon
// heroImage
// isLightDismiss