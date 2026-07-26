#include "qtpivotstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qabstractitemmodel.h>
#include <qstyleoption.h>

class QtPivotStylePrivate
{
    Q_DECLARE_PUBLIC(QtPivotStyle)
public:
    int currentIndex;
    int pivotSpacing;
    QModelIndex pressIndex;

    QtPivotStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtPivotStylePrivate(QtPivotStyle* q);
    ~QtPivotStylePrivate();
};

QtPivotStylePrivate::QtPivotStylePrivate(QtPivotStyle* q)
    : q_ptr(q)
{
}
QtPivotStylePrivate::~QtPivotStylePrivate()
{
}

QtPivotStyle::QtPivotStyle(QStyle* style)
    : d_ptr(new QtPivotStylePrivate(this))
{
    Q_D(QtPivotStyle);
    d->currentIndex = -1;
    d->pivotSpacing = 5;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtPivotStyle::~QtPivotStyle()
{
}

QModelIndex QtPivotStyle::pressIndex() const
{
    Q_D(const QtPivotStyle);
    return d->pressIndex;
}
void QtPivotStyle::setPressIndex(QModelIndex value)
{
    Q_D(QtPivotStyle);
    d->pressIndex = value;
}
int QtPivotStyle::currentIndex() const
{
    Q_D(const QtPivotStyle);
    return d->currentIndex;
}
void QtPivotStyle::setCurrentIndex(int value)
{
    Q_D(QtPivotStyle);
    d->currentIndex = value;
}

int QtPivotStyle::pivotSpacing() const
{
    Q_D(const QtPivotStyle);
    return d->pivotSpacing;
}
void QtPivotStyle::setPivotSpacing(int value)
{
    Q_D(QtPivotStyle);
    d->pivotSpacing = value;
}

const QColor& QtPivotStyle::markColor()
{
    Q_D(QtPivotStyle);
    return QtThemeColor(d->themeMode, PrimaryNormal);
}

void QtPivotStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    switch(element)
    {
        case QStyle::PE_PanelItemViewRow: {
            return;
        }
        case QStyle::PE_Widget: {
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QtPivotStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtPivotStyle);
    switch(element)
    {
        case QStyle::CE_ShapedFrame: {
            // viewport视口外的其他区域背景
            return;
        }
        case QStyle::CE_ItemViewItem: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                // 内容绘制
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
                textRect.adjust(0, 0, 0, -5);
                // 文字绘制
                if(!vopt->text.isEmpty())
                {
                    if(d->pressIndex == vopt->index)
                    {
                        painter->setPen(QtThemeColor(d->themeMode, BasicTextPress));
                    }
                    else
                    {
                        if(d->currentIndex == vopt->index.row() ||
                            vopt->state.testFlag(QStyle::State_MouseOver))
                        {
                            painter->setPen(QtThemeColor(d->themeMode, BasicText));
                        }
                        else
                        {
                            painter->setPen(QtThemeColor(d->themeMode, BasicTextNoFocus));
                        }
                    }
                    painter->drawText(textRect, Qt::AlignCenter, vopt->text);
                }
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

int QtPivotStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtPivotStyle);
    switch(metric)
    {
        case QStyle::PM_FocusFrameHMargin: {
            return d->pivotSpacing;
        }
        default: {
            break;
        }
    }

    return QProxyStyle::pixelMetric(metric, option, widget);
}
