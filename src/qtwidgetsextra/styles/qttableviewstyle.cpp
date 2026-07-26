#include "qttableviewstyle.h"
#include "qttheme.h"
#include "widgets/qttableview.h"
#include <qstyleoption.h>
class QtTableViewStylePrivate
{
    Q_DECLARE_PUBLIC(QtTableViewStyle)
public:
    int headerMargin;
    int currentHoverRow;
    QtTableViewStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    int horizontalPadding{ 11 };
    QtTableViewStylePrivate(QtTableViewStyle* q);
    ~QtTableViewStylePrivate();
};

QtTableViewStylePrivate::QtTableViewStylePrivate(QtTableViewStyle* q)
    : q_ptr(q)
{
}
QtTableViewStylePrivate::~QtTableViewStylePrivate()
{
}

QtTableViewStyle::QtTableViewStyle(QStyle* style)
    : d_ptr(new QtTableViewStylePrivate(this))
{
    Q_D(QtTableViewStyle);
    d->headerMargin = 6;
    d->currentHoverRow = -1;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtTableViewStyle::~QtTableViewStyle()
{
}

int QtTableViewStyle::headerMargin() const
{
    Q_D(const QtTableViewStyle);
    return d->headerMargin;
}
void QtTableViewStyle::setHeaderMargin(int value)
{
    Q_D(QtTableViewStyle);
    d->headerMargin = value;
}

int QtTableViewStyle::currentHoverRow() const
{
    Q_D(const QtTableViewStyle);
    return d->currentHoverRow;
}
void QtTableViewStyle::setCurrentHoverRow(int value)
{
    Q_D(QtTableViewStyle);
    d->currentHoverRow = value;
}

void QtTableViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtTableViewStyle);
    switch(element)
    {
        case QStyle::PE_PanelItemViewItem: {
            // 行覆盖绘制
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                const QtTableView* tabView = dynamic_cast<const QtTableView*>(widget);
                if(!tabView)
                {
                    return;
                }
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);
                QAbstractItemView::SelectionBehavior selectionBehavior =
                    tabView->selectionBehavior();
                if(selectionBehavior == QAbstractItemView::SelectRows)
                {
                    if(vopt->index.row() == d->currentHoverRow)
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                        painter->drawRect(vopt->rect);
                    }
                }
                else
                {
                    if(vopt->state.testFlag(QStyle::State_Selected) ||
                        vopt->state.testFlag(QStyle::State_MouseOver))
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                        painter->drawRect(vopt->rect);
                    }
                }
                painter->restore();
            }
            return;
        }
        case QStyle::PE_PanelItemViewRow: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);
                QRect itemRect = vopt->rect;
                painter->setPen(Qt::NoPen);
                if(vopt->state & QStyle::State_Selected)
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicSelectedAlpha));
                    painter->drawRect(itemRect);
                }
                else
                {
                    if(vopt->features.testFlag(QStyleOptionViewItem::Alternate))
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(QtThemeColor(d->themeMode, BasicAlternating));
                        painter->drawRect(vopt->rect);
                    }
                }
                painter->restore();
            }
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

void QtTableViewStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtTableViewStyle);
    switch(element)
    {
        case QStyle::CE_ShapedFrame: {
            QRect frameRect = option->rect;
            frameRect.adjust(1, 1, -1, -1);
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(QtThemeColor(d->themeMode, PopupBorder));
            painter->setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
            painter->drawRoundedRect(frameRect, 3, 3);
            painter->restore();
            return;
        }
        case QStyle::CE_HeaderLabel: {
            // 表头文字绘制
            if(const QStyleOptionHeader* hopt =
                    qstyleoption_cast<const QStyleOptionHeader*>(option))
            {
                QRect headerRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                if(!hopt->text.isEmpty())
                {
                    painter->setPen(QtThemeColor(d->themeMode, BasicText));
                    painter->drawText(headerRect, hopt->textAlignment, hopt->text);
                }
                painter->restore();
            }
            return;
        }
        case QStyle::CE_HeaderSection: {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                    QPainter::TextAntialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, BasicBaseDeepAlpha));
            painter->drawRect(option->rect);
            if(option->state.testFlag(QStyle::State_Sunken))
            {
                painter->setBrush(QtThemeColor(d->themeMode, BasicPressAlpha));
            }
            else
            {
                if(option->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                }
            }
            painter->drawRect(option->rect);
            painter->restore();
            return;
        }
        case QStyle::CE_HeaderEmptyArea: {
            QRect frameRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, BasicBaseDeep));
            painter->drawRect(frameRect);
            painter->restore();
            return;
        }
        case QStyle::CE_ItemViewItem: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                // 背景绘制
                const QtTableView* tabView = dynamic_cast<const QtTableView*>(widget);
                if(!tabView)
                {
                    return;
                }
                this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
                QAbstractItemView::SelectionBehavior selectionBehavior =
                    tabView->selectionBehavior();
                // 内容绘制
                QRect itemRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                // QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt,
                // widget);
                QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
                QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
                if(vopt->index.column() == 0)
                {
                    iconRect.adjust(d->horizontalPadding, 0, d->horizontalPadding, 0);
                    textRect.adjust(d->horizontalPadding, 0, 0, 0);
                }
                // 图标绘制
                if(!vopt->icon.isNull())
                {
                    QIcon::Mode mode = QIcon::Normal;
                    // if (!(vopt->state.testFlag(QStyle::State_Enabled)))
                    // {
                    //     mode = QIcon::Disabled;
                    // }
                    // else if (vopt->state.testFlag(QStyle::State_Selected))
                    // {
                    //     mode = QIcon::Selected;
                    // }
                    QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                    vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
                }
                if(!vopt->text.isEmpty())
                {
                    painter->setPen(QtThemeColor(d->themeMode, BasicText));
                    painter->drawText(textRect, vopt->displayAlignment, vopt->text);
                }
                int heightOffset = itemRect.height() / 4;
                painter->setPen(Qt::NoPen);
                painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                if(vopt->state.testFlag(QStyle::State_Selected))
                {
                    if(selectionBehavior == QAbstractItemView::SelectRows &&
                        vopt->index.column() == 0)
                    {
                        painter->drawRoundedRect(
                            QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3,
                                itemRect.height() - 2 * heightOffset),
                            3, 3);
                    }
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

int QtTableViewStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtTableViewStyle);
    switch(metric)
    {
        case QStyle::PM_HeaderMargin: {
            return d->headerMargin;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
