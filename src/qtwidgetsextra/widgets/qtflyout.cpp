#include "qtflyout.h"
#include "qttheme.h"
#include <qapplication.h>
#include <qboxlayout.h>
#include <qcoreevent.h>
#include <qgraphicseffect.h>
#include <qpropertyanimation.h>
#include <qscreen.h>

class QtFlyoutPrivate
{
    Q_DECLARE_PUBLIC(QtFlyout)
public:
    int borderRadius;
    QString title;
    QString content;
    bool isLightDismiss;
    QtFlyout* q_ptr;
    QVBoxLayout* mainLayout{ nullptr };
    QWidget* contentWidget{ nullptr };
    QGraphicsOpacityEffect* opacityEffect{ nullptr };
    QtThemeType::ThemeMode themeMode;
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    int shadowBorderWidth{ 0 };
#else
    int shadowBorderWidth{ 8 };
#endif

    QtFlyoutPrivate(QtFlyout* q);
    ~QtFlyoutPrivate();

    void doShowAnimation();
};

QtFlyoutPrivate::QtFlyoutPrivate(QtFlyout* q)
    : q_ptr(q)
{
}
QtFlyoutPrivate::~QtFlyoutPrivate()
{
}

void QtFlyoutPrivate::doShowAnimation()
{
    QPropertyAnimation* animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(250);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QtFlyout::QtFlyout(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtFlyoutPrivate(this))
{
    	Q_D(QtFlyout);
	d->borderRadius = 8;
	d->isLightDismiss = true;
	setObjectName("QtFlyout");
	setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	d->mainLayout = new QVBoxLayout(this);
	int margin = d->shadowBorderWidth + 8;
	d->mainLayout->setContentsMargins(margin, margin, margin, margin);

	d->opacityEffect = new QGraphicsOpacityEffect(this);
	d->opacityEffect->setOpacity(0);
	setGraphicsEffect(d->opacityEffect);

	d->themeMode = qtTheme->getThemeMode();
	connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode)
	{
		d_ptr->themeMode = themeMode;
		update();
	});
	hide();
}

QtFlyout::~QtFlyout()
{
}

void QtFlyout::setContentWidget(QWidget* widget)
{
    Q_D(QtFlyout);
    if(!widget || widget == this)
    {
        return;
    }
    if(d->contentWidget)
    {
        d->mainLayout->removeWidget(d->contentWidget);
        d->contentWidget->deleteLater();
    }
    d->contentWidget = widget;
    d->mainLayout->addWidget(widget);
}

void QtFlyout::showFlyout(QWidget* target)
{
    Q_D(QtFlyout);
    if(!target)
    {
        return;
    }
    // Calculate size based on content
    if(!d->contentWidget)
    {
        setFixedWidth(280);
        int contentHeight = d->shadowBorderWidth * 2 + 12;
        int textWidth = 280 - d->shadowBorderWidth * 2 - 30;
        if(!d->title.isEmpty())
        {
            QFont titleFont = font();
            titleFont.setPixelSize(14);
            titleFont.setBold(true);
            QFontMetrics tfm(titleFont);
            contentHeight += tfm.height() + 6;
        }
        if(!d->content.isEmpty())
        {
            QFont contentFont = font();
            contentFont.setPixelSize(13);
            QFontMetrics cfm(contentFont);
            QRect br = cfm.boundingRect(QRect(0, 0, textWidth, 0), Qt::TextWordWrap, d->content);
            contentHeight += br.height() + 8;
        }
        contentHeight += 12;
        setFixedHeight(contentHeight);
    }
    else
    {
        setMinimumWidth(0);
        setMaximumWidth(QWIDGETSIZE_MAX);
        setMinimumHeight(0);
        setMaximumHeight(QWIDGETSIZE_MAX);
        adjustSize();
    }
    QPoint targetGlobal = target->mapToGlobal(QPoint(0, target->height()));
    int x = targetGlobal.x() + (target->width() - width()) / 2;
    int y = targetGlobal.y() + 5;

    QScreen* screen = QApplication::screenAt(targetGlobal);
    if(screen)
    {
        QRect screenRect = screen->availableGeometry();
        if(x < screenRect.left())
        {
            x = screenRect.left();
        }
        if(x + width() > screenRect.right())
        {
            x = screenRect.right() - width();
        }
        if(y + height() > screenRect.bottom())
        {
            y = targetGlobal.y() - target->height() - height() - 5;
        }
        if(y < screenRect.top())
        {
            y = screenRect.top();
        }
    }
    move(x, y);
    show();
    raise();
    if(d->isLightDismiss)
    {
        qApp->installEventFilter(this);
    }
    d->doShowAnimation();
}

void QtFlyout::closeFlyout()
{
    Q_D(QtFlyout);
    qApp->removeEventFilter(this);
    hide();
    Q_EMIT closed();
}

bool QtFlyout::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtFlyout);
    if(event->type() == QEvent::MouseButtonPress)
    {
        QWidget* clickedWidget = qobject_cast<QWidget*>(watched);
        if(clickedWidget && !this->isAncestorOf(clickedWidget) && clickedWidget != this)
        {
            this->closeFlyout();
            return false;
        }
    }
    return QObject::eventFilter(watched, event);
}

void QtFlyout::paintEvent(QPaintEvent* event)
{
    Q_D(QtFlyout);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    // Shadow
    qtTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
    QRect foregroundRect = rect();
    foregroundRect.adjust(
        d->shadowBorderWidth, d->shadowBorderWidth, -d->shadowBorderWidth, -d->shadowBorderWidth);
    painter.setPen(QtThemeColor(d->themeMode, PopupBorder));
    painter.setBrush(QtThemeColor(d->themeMode, PopupBase));
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);

    if(!d->contentWidget)
    {
        int textX = foregroundRect.x() + 12;
        int textY = foregroundRect.y() + 12;
        int textWidth = foregroundRect.width() - 24;

        // Title
        if(!d->title.isEmpty())
        {
            QFont titleFont = painter.font();
            titleFont.setPixelSize(14);
            titleFont.setBold(true);
            painter.setFont(titleFont);
            painter.setPen(QtThemeColor(d->themeMode, BasicText));
            QRect titleRect(textX, textY, textWidth, painter.fontMetrics().height());
            painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, d->title);
            textY += titleRect.height() + 6;
        }

        // Content
        if(!d->content.isEmpty())
        {
            QFont contentFont = painter.font();
            contentFont.setPixelSize(13);
            contentFont.setBold(false);
            painter.setFont(contentFont);
            painter.setPen(QtThemeColor(d->themeMode, BasicDetailsText));
            QRect contentRect(textX, textY, textWidth, foregroundRect.bottom() - textY - 12);
            painter.drawText(contentRect, Qt::AlignLeft | Qt::TextWordWrap, d->content);
        }
    }
    painter.restore();
}

int QtFlyout::borderRadius() const
{
    Q_D(const QtFlyout);
    return d->borderRadius;
}

void QtFlyout::setBorderRadius(int value)
{
    Q_D(QtFlyout);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

const QString& QtFlyout::title() const
{
    Q_D(const QtFlyout);
    return d->title;
}

void QtFlyout::setTitle(const QString& value)
{
    Q_D(QtFlyout);
    if(d->title != value)
    {
        d->title = value;
        Q_EMIT titleChanged();
    }
}

const QString& QtFlyout::content() const
{
    Q_D(const QtFlyout);
    return d->content;
}

void QtFlyout::setContent(const QString& value)
{
    Q_D(QtFlyout);
    if(d->content != value)
    {
        d->content = value;
        Q_EMIT contentChanged();
    }
}

bool QtFlyout::isLightDismiss() const
{
    Q_D(const QtFlyout);
    return d->isLightDismiss;
}

void QtFlyout::setIsLightDismiss(bool value)
{
    Q_D(QtFlyout);
    if(d->isLightDismiss != value)
    {
        d->isLightDismiss = value;
        Q_EMIT isLightDismissChanged();
    }
}