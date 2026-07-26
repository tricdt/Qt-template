#include "qtcheckboxstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qtclasshelpermacros.h>
#include <qtypes.h>

class QtCheckBoxStylePrivate
{
    Q_DECLARE_PUBLIC(QtCheckBoxStyle)
public:
    int checkIndicatorWidth;
    QtThemeType::ThemeMode themeMode;
    QtCheckBoxStyle* q_ptr;
    QtCheckBoxStylePrivate(QtCheckBoxStyle* q);
    ~QtCheckBoxStylePrivate();
};

QtCheckBoxStylePrivate::QtCheckBoxStylePrivate(QtCheckBoxStyle* q)
    : checkIndicatorWidth(21)
    , q_ptr(q)
{
}
QtCheckBoxStylePrivate::~QtCheckBoxStylePrivate()
{
}

QtCheckBoxStyle::QtCheckBoxStyle(QStyle*)
    : d_ptr(new QtCheckBoxStylePrivate(this))
{
    Q_D(QtCheckBoxStyle);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtCheckBoxStyle::~QtCheckBoxStyle()
{
}

void QtCheckBoxStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtCheckBoxStyle);
    switch(element)
    {
        case QStyle::CE_CheckBox: {
            if(const QStyleOptionButton* bopt =
                    qstyleoption_cast<const QStyleOptionButton*>(option))
            {
                bool isEnabled = bopt->state.testFlag(QStyle::State_Enabled);
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                QRect checkBoxRect = bopt->rect;
                QRect checkRect(checkBoxRect.x(), checkBoxRect.y(), d->checkIndicatorWidth,
                    d->checkIndicatorWidth);
                checkRect.adjust(1, 1, -1, -1);
                painter->setPen(Qt::NoPen);
                if(bopt->state.testFlag(QStyle::State_On) ||
                    bopt->state.testFlag(QStyle::State_NoChange))
                {
                    painter->setPen(Qt::NoPen);
                    if(bopt->state.testFlag(QStyle::State_Sunken))
                    {
                        painter->setBrush(QtThemeColor(d->themeMode, PrimaryPress));
                    }
                    else
                    {
                        if(bopt->state.testFlag(QStyle::State_MouseOver))
                        {
                            painter->setBrush(QtThemeColor(d->themeMode, PrimaryHover));
                        }
                        else
                        {
                            painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                        }
                    }
                }
                else
                {
                    if(bopt->state.testFlag(QStyle::State_Sunken))
                    {
                        painter->setPen(QtThemeColor(d->themeMode, BasicBorderDeep));
                    }
                    else
                    {
                        painter->setPen(QtThemeColor(d->themeMode, BasicBorderDeep));
                        if(bopt->state.testFlag(QStyle::State_MouseOver))
                        {
                            painter->setBrush(QtThemeColor(d->themeMode, BasicHover));
                        }
                        else
                        {
                            painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
                        }
                    }
                }
                painter->drawRoundedRect(checkRect, 2, 2);
                painter->setPen(QtThemeColor(QtThemeType::Dark, BasicText));
                if(bopt->state.testFlag(QStyle::State_On))
                {
                    painter->save();
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(d->checkIndicatorWidth * 0.75);
                    painter->setFont(iconFont);
                    painter->drawText(
                        checkRect, Qt::AlignCenter, QChar(static_cast<ushort>(QtIconType::Check)));
                    painter->restore();
                }
                else if(bopt->state.testFlag(QStyle::State_NoChange))
                {
                    QLine checkLine(checkRect.x() + 3, checkRect.center().y(),
                        checkRect.right() - 3, checkRect.center().y());
                    painter->drawLine(checkLine);
                }
                painter->setPen(isEnabled ? QtThemeColor(d->themeMode, BasicText)
                                          : QtThemeColor(d->themeMode, BasicTextDisable));
                QRect textRect(checkRect.right() + 10, checkBoxRect.y(), checkBoxRect.width(),
                    checkBoxRect.height() - 3);
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
                painter->restore();
            }
            return;
        }
        default: {
            break;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}
int QtCheckBoxStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtCheckBoxStyle);
    switch(metric)
    {
        case QStyle::PM_IndicatorWidth: {
            return d->checkIndicatorWidth;
        }
        case QStyle::PM_IndicatorHeight: {
            return d->checkIndicatorWidth;
        }
        case QStyle::PM_CheckBoxLabelSpacing: {
            return 10;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}