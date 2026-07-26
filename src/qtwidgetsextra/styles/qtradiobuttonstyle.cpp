#include "qtradiobuttonstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qpainter.h>
#include <qstyleoption.h>
#include <qtypes.h>


class QtRadioButtonStylePrivate
{
    Q_DECLARE_PUBLIC(QtRadioButtonStyle)
public:
    QtRadioButtonStyle* q_ptr;
    QtRadioButtonStylePrivate(QtRadioButtonStyle* q);
    ~QtRadioButtonStylePrivate();
    QtThemeType::ThemeMode themeMode;
};

QtRadioButtonStylePrivate::QtRadioButtonStylePrivate(QtRadioButtonStyle* q)
    : q_ptr(q)
{
}
QtRadioButtonStylePrivate::~QtRadioButtonStylePrivate()
{
}

QtRadioButtonStyle::QtRadioButtonStyle(QStyle*)
    : d_ptr(new QtRadioButtonStylePrivate(this))
{
    Q_D(QtRadioButtonStyle);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtRadioButtonStyle::~QtRadioButtonStyle()
{
}

void QtRadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtRadioButtonStyle);
    switch(element)
    {
        case PE_IndicatorRadioButton: {
            const QStyleOptionButton* bopt = qstyleoption_cast<const QStyleOptionButton*>(option);
            if(!bopt)
            {
                break;
            }
            QRect buttonRect = bopt->rect;
            buttonRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

            if(bopt->state & QStyle::State_Off)
            {
                painter->setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), 1.5));
                if(bopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicHover));
                }
                else
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
                }
                painter->drawEllipse(
                    QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 8.5, 8.5);
            }
            else
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                painter->drawEllipse(
                    QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                    buttonRect.width() / (qreal) 2, buttonRect.width() / (qreal) 2);
                painter->setBrush(QtThemeColor(d->themeMode, BasicTextInvert));
                if(bopt->state & QStyle::State_Sunken)
                {
                    if(bopt->state & QStyle::State_MouseOver)
                    {
                        painter->drawEllipse(
                            QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                            buttonRect.width() / 4.5, buttonRect.width() / 4.5);
                    }
                }
                else
                {
                    if(bopt->state & QStyle::State_MouseOver)
                    {
                        painter->drawEllipse(
                            QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                            buttonRect.width() / 3.5, buttonRect.width() / 3.5);
                    }
                    else
                    {
                        painter->drawEllipse(
                            QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1),
                            buttonRect.width() / (qreal) 4, buttonRect.width() / (qreal) 4);
                    }
                }
            }
            painter->restore();
            return;
        }
        default: {
            break;
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int QtRadioButtonStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch(metric)
    {
        case QStyle::PM_ExclusiveIndicatorWidth: {
            return 20;
        }
        case QStyle::PM_ExclusiveIndicatorHeight: {
            return 20;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
