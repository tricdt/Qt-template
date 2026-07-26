#include "qtsliderstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qobject.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qtypes.h>
#include <qvariantanimation.h>
class QtSliderStylePrivate
{
    Q_DECLARE_PUBLIC(QtSliderStyle)
public:
    QtSliderStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    mutable QStyle::State lastState{ QStyle::State_None };
    mutable qreal circleRadius{ 0 };
    QtSliderStylePrivate(QtSliderStyle* q);
    ~QtSliderStylePrivate();

    void startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

QtSliderStylePrivate::QtSliderStylePrivate(QtSliderStyle* q)
    : q_ptr(q)
{
}
QtSliderStylePrivate::~QtSliderStylePrivate()
{
}

void QtSliderStylePrivate::startRadiusAnimation(
    qreal startRadius, qreal endRadius, QWidget* widget) const
{
    Q_Q(const QtSliderStyle);
    QtSliderStyle* style = const_cast<QtSliderStyle*>(q);
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

QtSliderStyle::QtSliderStyle(QStyle*)
    : d_ptr(new QtSliderStylePrivate(this))
{
    Q_D(QtSliderStyle);
    d->circleRadius = 0.01;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtSliderStyle::~QtSliderStyle()
{
}

void QtSliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtSliderStyle);
    switch(control)
    {
        case QStyle::CC_Slider: {
            const QStyleOptionSlider* sopt = qstyleoption_cast<const QStyleOptionSlider*>(option);
            if(!sopt)
            {
                break;
            }

            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            QRect sliderRect = sopt->rect;
            QRect sliderHandleRect = subControlRect(control, sopt, SC_SliderHandle, widget);
            sliderHandleRect.adjust(1, 1, -1, -1);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, BasicChute));
            if(sopt->orientation == Qt::Horizontal)
            {
                painter->drawRoundedRect(
                    QRect(sliderRect.x() + sliderRect.height() / 8,
                        sliderRect.y() + sliderRect.height() * 0.375,
                        sliderRect.width() - sliderRect.height() / 4, sliderRect.height() / 4),
                    sliderRect.height() / (qreal) 8, sliderRect.height() / (qreal) 8);
                painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRect(sliderRect.x() + sliderRect.height() / 8,
                                             sliderRect.y() + sliderRect.height() * 0.375,
                                             sliderHandleRect.x(), sliderRect.height() / 4),
                    sliderRect.height() / (qreal) 8, sliderRect.height() / (qreal) 8);
            }
            else
            {
                painter->drawRoundedRect(
                    QRect(sliderRect.x() + sliderRect.width() * 0.375,
                        sliderRect.y() + sliderRect.width() / 8, sliderRect.width() / 4,
                        sliderRect.height() - sliderRect.width() / 4),
                    sliderRect.width() / (qreal) 8, sliderRect.width() / (qreal) 8);
                painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                painter->drawRoundedRect(
                    QRect(sliderRect.x() + sliderRect.width() * 0.375, sliderHandleRect.y(),
                        sliderRect.width() / 4,
                        sliderRect.height() - sliderRect.width() / 8 - sliderHandleRect.y()),
                    sliderRect.width() / (qreal) 8, sliderRect.width() / (qreal) 8);
            }
            painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
            painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
            painter->drawEllipse(
                QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1),
                sliderHandleRect.width() / (qreal) 2, sliderHandleRect.width() / (qreal) 2);
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

int QtSliderStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch(metric)
    {
        case QStyle::PM_SliderLength: {
            return 20;
        }
        case QStyle::PM_SliderThickness: {
            return 20;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int QtSliderStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget,
    QStyleHintReturn* returnData) const
{
    if(hint == QStyle::SH_Slider_AbsoluteSetButtons)
    {
        return Qt::LeftButton;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
