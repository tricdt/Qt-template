#include "qttoggleswitch.h"
#include "qttheme.h"
#include <qcoreevent.h>
#include <qevent.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>

class QtToggleSwitchPrivate
{
    Q_DECLARE_PUBLIC(QtToggleSwitch)
public:
    bool isToggled{ false };
    int margin{ 1 };
    qreal circleCenterX{ 0 };
    qreal circleRadius{ 0 };
    bool isLeftButtonPress{ false };
    bool isMousePressMove{ false };
    int lastMouseX{ 0 };
    QtThemeType::ThemeMode themeMode;
    QtToggleSwitch* q_ptr;
    QtToggleSwitchPrivate(QtToggleSwitch* q);
    ~QtToggleSwitchPrivate();

    void startPosAnimation(qreal startX, qreal endX, bool isToggle);
    void startRadiusAnimation(qreal startRadius, qreal endRadius);
    void adjustCircleCenterX();
};

QtToggleSwitchPrivate::QtToggleSwitchPrivate(QtToggleSwitch* q)
    : q_ptr(q)
{
}
QtToggleSwitchPrivate::~QtToggleSwitchPrivate()
{
}

void QtToggleSwitchPrivate::startPosAnimation(qreal startX, qreal endX, bool isToggle)
{
    Q_Q(QtToggleSwitch);
    QPropertyAnimation* circleAnimation = new QPropertyAnimation(q, "circleCenterX");
    QObject::connect(
        circleAnimation, &QVariantAnimation::valueChanged, q, [this](const QVariant& value) {
            circleCenterX = value.toReal();
            q_ptr->update();
        });
    circleAnimation->setStartValue(startX);
    circleAnimation->setEndValue(endX);
    circleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    isToggled = isToggle;
    Q_EMIT q->toggled(isToggle);
}
void QtToggleSwitchPrivate::startRadiusAnimation(qreal startRadius, qreal endRadius)
{
    Q_Q(QtToggleSwitch);
    QPropertyAnimation* circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius");
    QObject::connect(
        circleRadiusAnimation, &QVariantAnimation::valueChanged, q, [this](const QVariant& value) {
            circleRadius = value.toReal();
            q_ptr->update();
        });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtToggleSwitchPrivate::adjustCircleCenterX()
{
    Q_Q(QtToggleSwitch);
    if(circleCenterX > q->width() - q->height() / (qreal) 2 - margin * 2)
    {
        circleCenterX = q->width() - q->height() / (qreal) 2 - margin * 2;
    }
    if(circleCenterX < q->height() / (qreal) 2)
    {
        circleCenterX = q->height() / (qreal) 2;
    }
}

QtToggleSwitch::QtToggleSwitch(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtToggleSwitchPrivate(this))
{
    Q_D(QtToggleSwitch);
    setObjectName("QtToggleSwitch");
    setMouseTracking(true);
    setFixedSize(44, 22);
    d->circleCenterX = -1;
    d->isToggled = false;
    d->themeMode = qtTheme->getThemeMode();
    setProperty("circleCenterX", 0.01);
    setProperty("circleRadius", 0.01);
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtToggleSwitch::~QtToggleSwitch()
{
}

void QtToggleSwitch::setIsToggled(bool isToggled)
{
    Q_D(QtToggleSwitch);
    if(d->isToggled == isToggled)
    {
        return;
    }
    if(d->isToggled)
    {
        d->startPosAnimation(
            width() - height() / (qreal) 2 - d->margin * 2, height() / (qreal) 2, isToggled);
    }
    else
    {
        d->startPosAnimation(
            height() / (qreal) 2, width() - height() / (qreal) 2 - d->margin * 2, isToggled);
    }
}
bool QtToggleSwitch::getIsToggled() const
{
    Q_D(const QtToggleSwitch);
    return d->isToggled;
}

bool QtToggleSwitch::event(QEvent* event)
{
    Q_D(QtToggleSwitch);
    switch(event->type())
    {
        case QEvent::Enter: {
            if(isEnabled())
            {
                d->startRadiusAnimation(height() * 0.3, height() * 0.35);
            }
            break;
        }
        case QEvent::Leave: {
            if(isEnabled())
            {
                d->startRadiusAnimation(height() * 0.35, height() * 0.3);
            }
            break;
        }
        case QEvent::MouseMove: {
            update();
            break;
        }
        default: {
            break;
        }
    }
    return QWidget::event(event);
}
void QtToggleSwitch::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtToggleSwitch);
    d->adjustCircleCenterX();
    d->isLeftButtonPress = true;
    d->lastMouseX = event->pos().x();
    d->startRadiusAnimation(d->circleRadius, height() * 0.25);
    QWidget::mousePressEvent(event);
}
void QtToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtToggleSwitch);
    d->isLeftButtonPress = false;
    QWidget::mouseReleaseEvent(event);
    if(d->isMousePressMove)
    {
        d->isMousePressMove = false;
        if(d->circleCenterX > width() / (qreal) 2)
        {
            d->startPosAnimation(
                d->circleCenterX, width() - height() / (qreal) 2 - d->margin * 2, true);
        }
        else
        {
            d->startPosAnimation(d->circleCenterX, height() / (qreal) 2, false);
        }
    }
    else
    {
        if(d->isToggled)
        {
            d->startPosAnimation(d->circleCenterX, height() / (qreal) 2, false);
        }
        else
        {
            d->startPosAnimation(
                d->circleCenterX, width() - height() / (qreal) 2 - d->margin * 2, true);
        }
    }
    d->startRadiusAnimation(height() * 0.25, height() * 0.35);
}
void QtToggleSwitch::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtToggleSwitch);
    if(d->isLeftButtonPress)
    {
        d->isMousePressMove = true;
        int moveX = event->pos().x() - d->lastMouseX;
        d->lastMouseX = event->pos().x();
        d->circleCenterX += moveX;
        d->adjustCircleCenterX();
    }
    QWidget::mouseMoveEvent(event);
}
void QtToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_D(QtToggleSwitch);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    painter.setPen(d->isToggled ? QPen(QtThemeColor(d->themeMode, BasicBorder), 1.5)
                                 : QPen(QtThemeColor(d->themeMode, BasicBorderDeep), 1.5));
    painter.setBrush(isEnabled() ? d->isToggled
                                       ? QtThemeColor(d->themeMode, PrimaryNormal)
                                       : (underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                                       : QtThemeColor(d->themeMode, BasicBase))
                                 : QtThemeColor(d->themeMode, BasicDisable));
    QPainterPath path;
    path.moveTo(width() - height() - d->margin, height() - d->margin);
    path.arcTo(QRectF(QPointF(width() - height() - d->margin, d->margin),
                   QSize(height() - d->margin * 2, height() - d->margin * 2)),
        -90, 180);
    path.lineTo(height() / (qreal)2 + d->margin, d->margin);
    path.arcTo(QRectF(QPointF(d->margin, d->margin),
                   QSize(height() - d->margin * 2, height() - d->margin * 2)),
        90, 180);
    path.lineTo(width() - height() - d->margin, height() - d->margin);
    path.closeSubpath();
    painter.drawPath(path);

    // 圆心绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(isEnabled() ? d->isToggled
                                       ? QtThemeColor(d->themeMode, BasicTextInvert)
                                       : QtThemeColor(d->themeMode, ToggleSwitchNoToggledCenter)
                                 : QtThemeColor(d->themeMode, BasicTextDisable));
    if(d->circleRadius == 0)
    {
        d->circleRadius =
            this->isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3;
    }
    if(d->isLeftButtonPress)
    {
        painter.drawEllipse(
            QPointF(d->circleCenterX, height() / (qreal)2), d->circleRadius, d->circleRadius);
    }
    else
    {
        if(d->circleCenterX == -1)
        {
            d->circleCenterX =
                d->isToggled ? width() - height() / 2 - d->margin * 2 : height() / 2;
        }
        painter.drawEllipse(
            QPointF(d->circleCenterX, height() / (qreal)2), d->circleRadius, d->circleRadius);
    }
    painter.restore();
}