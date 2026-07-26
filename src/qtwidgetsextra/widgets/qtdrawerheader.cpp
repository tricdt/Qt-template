#include "qtdrawerheader.h"
#include "qttheme.h"
#include <qboxlayout.h>
#include <qcoreevent.h>
#include <qevent.h>
#include <qpropertyanimation.h>
class QtDrawerHeaderPrivate
{
    Q_DECLARE_PUBLIC(QtDrawerHeader)
public:
    qreal expandIconRotate;
    int borderRadius;
    bool isExpand;
    QtDrawerHeader* q_ptr;
    QtThemeType::ThemeMode themeMode;
    bool isPressed{ false };
    QWidget* headerWidget{ nullptr };
    QVBoxLayout* mainLayout{ nullptr };
    QtDrawerHeaderPrivate(QtDrawerHeader* q);
    ~QtDrawerHeaderPrivate();
};

QtDrawerHeaderPrivate::QtDrawerHeaderPrivate(QtDrawerHeader* q)
    : q_ptr(q)
{
}
QtDrawerHeaderPrivate::~QtDrawerHeaderPrivate()
{
}

QtDrawerHeader::QtDrawerHeader(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtDrawerHeaderPrivate(this))
{
    Q_D(QtDrawerHeader);
    d->borderRadius = 6;
    d->isExpand = false;
    d->expandIconRotate = 0;
    setFixedHeight(75);
    setMouseTracking(true);
    setObjectName("QtDrawerHeader");
    setStyleSheet("#QtDrawerHeader{background-color:transparent;}");

    d->mainLayout = new QVBoxLayout(this);
    d->mainLayout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 30, 0);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtDrawerHeader::~QtDrawerHeader()
{
}

void QtDrawerHeader::setHeaderWidget(QWidget* widget)
{
    Q_D(QtDrawerHeader);
    if(!widget)
    {
        return;
    }
    if(d->headerWidget)
    {
        d->mainLayout->removeWidget(d->headerWidget);
        d->headerWidget->deleteLater();
    }
    d->mainLayout->addWidget(widget);
    d->headerWidget = widget;
}

void QtDrawerHeader::doExpandOrCollapseAnimation()
{
    Q_D(QtDrawerHeader);
    QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "expandIconRotate");
    connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) { update(); });
    rotateAnimation->setDuration(300);
    rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
    rotateAnimation->setStartValue(d->expandIconRotate);
    rotateAnimation->setEndValue(d->isExpand ? -180.0 : 0.0);
    rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool QtDrawerHeader::event(QEvent* event)
{
    Q_D(QtDrawerHeader);
    switch(event->type())
    {
        case QEvent::Enter:
        case QEvent::Leave: {
            update();
            break;
        }
        default: {
            break;
        }
    }
    return QWidget::event(event);
}

void QtDrawerHeader::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtDrawerHeader);
    QWidget* posWidget = childAt(event->pos());
    if(!posWidget || (posWidget && posWidget->objectName().isEmpty()))
    {
        d->isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void QtDrawerHeader::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtDrawerHeader);
    QWidget* posWidget = childAt(event->pos());
    if(!posWidget || (posWidget && posWidget->objectName().isEmpty()))
    {
        d->isPressed = false;
        d->isExpand = !d->isExpand;
        // 指示器动画
        doExpandOrCollapseAnimation();
        Q_EMIT drawerHeaderClicked(d->isExpand);
    }
    QWidget::mouseReleaseEvent(event);
}

void QtDrawerHeader::mouseMoveEvent(QMouseEvent* event)
{
    update();
    QWidget::mouseMoveEvent(event);
}

void QtDrawerHeader::paintEvent(QPaintEvent* event)
{
    Q_D(QtDrawerHeader);
    bool isUnderMouse = underMouse();
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(d->isPressed   ? QtThemeColor(d->themeMode, BasicSelectedAlpha)
                     : isUnderMouse ? QtThemeColor(d->themeMode, BasicPressAlpha)
                                    : QtThemeColor(d->themeMode, BasicBaseAlpha));
    QRect foregroundRect(
        1, 1, width() - 2, d->isExpand ? height() + d->borderRadius : height() - 2);
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    if(isUnderMouse)
    {
        painter.setPen(QtThemeColor(d->themeMode, BasicBorderDeep));
    }
    if(d->isExpand)
    {
        painter.drawLine(0, height() - 1, width(), height() - 1);
    }
    else
    {
        painter.drawLine(d->borderRadius, height() - 1, width() - d->borderRadius, height() - 1);
        painter.drawArc(
            QRectF(1, height() - 1 - 2 * d->borderRadius, 2 * d->borderRadius, 2 * d->borderRadius),
            240 * 16, 30 * 16);
        painter.drawArc(QRectF(width() - 2 * d->borderRadius, height() - 1 - 2 * d->borderRadius,
                            2 * d->borderRadius, 2 * d->borderRadius),
            -90 * 16, 30 * 16);
    }

    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    painter.setPen(isEnabled() ? QtThemeColor(d->themeMode, BasicText)
                               : QtThemeColor(d->themeMode, BasicTextDisable));
    QRectF expandIconRect(width() - 25, 0, 20, height());
    painter.translate(expandIconRect.x() + (qreal) expandIconRect.width() / 2 - 2,
        expandIconRect.y() + (qreal) expandIconRect.height() / 2);
    painter.rotate(d->expandIconRotate);
    painter.translate(-expandIconRect.x() - (qreal) expandIconRect.width() / 2 + 2,
        -expandIconRect.y() - (qreal) expandIconRect.height() / 2);
    painter.drawText(
        expandIconRect, Qt::AlignVCenter, QChar((unsigned short) QtIconType::AngleDown));

    painter.restore();
}

qreal QtDrawerHeader::expandIconRotate() const
{
    Q_D(const QtDrawerHeader);
    return d->expandIconRotate;
}
void QtDrawerHeader::setExpandIconRotate(qreal value)
{
    Q_D(QtDrawerHeader);
    d->expandIconRotate = value;
}

int QtDrawerHeader::borderRadius() const
{
    Q_D(const QtDrawerHeader);
    return d->borderRadius;
}
void QtDrawerHeader::setBorderRadius(int value)
{
    Q_D(QtDrawerHeader);
    d->borderRadius = value;
}

int QtDrawerHeader::isExpand() const
{
    Q_D(const QtDrawerHeader);
    return d->isExpand;
}
void QtDrawerHeader::setIsExpand(bool value)
{
    Q_D(QtDrawerHeader);
    d->isExpand = value;
}