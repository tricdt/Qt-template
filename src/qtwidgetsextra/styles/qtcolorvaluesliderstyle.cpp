#include "qtcolorvaluesliderstyle.h"
#include "qttheme.h"
#include <qstyleoption.h>
#include <qvariantanimation.h>
class QtColorValueSliderStylePrivate
{
    Q_DECLARE_PUBLIC(QtColorValueSliderStyle)
public:
    QColor baseColor;
    QtColorValueSliderStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QLinearGradient* baseGradient{ nullptr };
    mutable QStyle::State lastState{ QStyle::State_None };
    mutable qreal circleRadius{ 0 };
    QtColorValueSliderStylePrivate(QtColorValueSliderStyle* q);
    ~QtColorValueSliderStylePrivate();

    void startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

QtColorValueSliderStylePrivate::QtColorValueSliderStylePrivate(QtColorValueSliderStyle* q)
    : q_ptr(q)
{
}
QtColorValueSliderStylePrivate::~QtColorValueSliderStylePrivate()
{
}

void QtColorValueSliderStylePrivate::startRadiusAnimation(
    qreal startRadius, qreal endRadius, QWidget* widget) const
{
    Q_Q(const QtColorValueSliderStyle);
    QtColorValueSliderStyle* style = const_cast<QtColorValueSliderStyle*>(q);
    QVariantAnimation* circleRadiusAnimation = new QVariantAnimation(style);
    QObject::connect(circleRadiusAnimation, &QVariantAnimation::valueChanged, style,
        [this, widget](const QVariant& value) {
            this->circleRadius = value.toReal();
            widget->update();
        });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

QtColorValueSliderStyle::QtColorValueSliderStyle(QStyle* style)
    : d_ptr(new QtColorValueSliderStylePrivate(this))
{
    Q_D(QtColorValueSliderStyle);
    d->baseGradient = new QLinearGradient(0, 0, 100, 100);
    setProperty("circleRadius", 0.01);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtColorValueSliderStyle::~QtColorValueSliderStyle()
{
}

QColor QtColorValueSliderStyle::baseColor() const
{
    Q_D(const QtColorValueSliderStyle);
    return d->baseColor;
}
void QtColorValueSliderStyle::setBaseColor(QColor value)
{
    Q_D(QtColorValueSliderStyle);
    d->baseColor = value;
}

void QtColorValueSliderStyle::drawComplexControl(ComplexControl control,
    const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtColorValueSliderStyle);
    switch(control)
    {
        case QStyle::CC_Slider: {
            const QStyleOptionSlider* sopt = qstyleoption_cast<const QStyleOptionSlider*>(option);
            if(!sopt)
            {
                break;
            }
            d->baseGradient->setFinalStop(widget->rect().bottomRight());
            QColor highValueColor = d->baseColor.toHsv();
            highValueColor.setHsv(highValueColor.hue(), highValueColor.saturation(), 255);
            QColor lowValueColor = d->baseColor.toHsv();
            lowValueColor.setHsv(lowValueColor.hue(), lowValueColor.saturation(), 0);
            d->baseGradient->setColorAt(0, highValueColor);
            d->baseGradient->setColorAt(1, lowValueColor);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            QRectF sliderRect = sopt->rect;
            sliderRect.adjust(1.5, 0, -1.5, 0);
            QRect sliderHandleRect = subControlRect(control, sopt, SC_SliderHandle, widget);
            sliderHandleRect.adjust(1, 1, -1, -1);
            // 滑槽
            painter->setPen(Qt::NoPen);
            painter->setBrush(*d->baseGradient);
            // 渐变背景
            painter->drawRoundedRect(
                QRectF(sliderRect.x(), sliderRect.y() + sliderRect.width() / 8, sliderRect.width(),
                    sliderRect.height() - sliderRect.width() / 4),
                sliderRect.width() / 2, sliderRect.width() / 2);

            // 滑块
            // 外圆形
            painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
            painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
            painter->drawEllipse(
                QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1),
                sliderHandleRect.width() / 2, sliderHandleRect.width() / 2);
            // 内圆形
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
            if(d->lastState == 0)
            {
                d->lastState = sopt->state;
            }
            if(d->circleRadius == 0)
            {
                d->circleRadius = sliderHandleRect.width() / 3.8;
            }
            if(sopt->activeSubControls == SC_SliderHandle)
            {
                if(sopt->state & QStyle::State_Sunken)
                {
                    if(sopt->state & QStyle::State_MouseOver)
                    {
                        if(!d->lastState.testFlag(QStyle::State_Sunken))
                        {
                            d->startRadiusAnimation(sliderHandleRect.width() / 2.8,
                                sliderHandleRect.width() / 4.5, const_cast<QWidget*>(widget));
                            d->lastState = sopt->state;
                        }
                        painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1,
                                                 sliderHandleRect.center().y() + 1),
                            d->circleRadius, d->circleRadius);
                    }
                }
                else
                {
                    if(sopt->state & QStyle::State_MouseOver)
                    {
                        if(!d->lastState.testFlag(QStyle::State_MouseOver))
                        {
                            d->startRadiusAnimation(d->circleRadius, sliderHandleRect.width() / 2.8,
                                const_cast<QWidget*>(widget));
                            d->lastState = sopt->state;
                        }
                        if(d->lastState.testFlag(QStyle::State_Sunken))
                        {
                            d->startRadiusAnimation(d->circleRadius, sliderHandleRect.width() / 2.8,
                                const_cast<QWidget*>(widget));
                            d->lastState = sopt->state;
                        }
                        painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1,
                                                 sliderHandleRect.center().y() + 1),
                            d->circleRadius, d->circleRadius);
                    }
                }
            }
            else
            {
                if(d->lastState.testFlag(QStyle::State_MouseOver) ||
                    d->lastState.testFlag(QStyle::State_Sunken))
                {
                    d->startRadiusAnimation(d->circleRadius, sliderHandleRect.width() / 3.8,
                        const_cast<QWidget*>(widget));
                    d->lastState &= ~QStyle::State_MouseOver;
                    d->lastState &= ~QStyle::State_Sunken;
                }
                painter->drawEllipse(
                    QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1),
                    d->circleRadius, d->circleRadius);
            }
            painter->restore();
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int QtColorValueSliderStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch(metric)
    {
        case QStyle::PM_SliderLength: {
            return 20;
        }
        case QStyle::PM_SliderThickness: {
            return 16;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int QtColorValueSliderStyle::styleHint(StyleHint hint, const QStyleOption* option,
    const QWidget* widget, QStyleHintReturn* returnData) const
{
    if(hint == QStyle::SH_Slider_AbsoluteSetButtons)
    {
        return Qt::LeftButton;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
