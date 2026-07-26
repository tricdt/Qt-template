#include "qttablewidgetstyle.h"
#include "qttheme.h"
#include "widgets/qttablewidget.h"
#include <QAbstractItemView>
#include <qstyleoption.h>

class QtTableWidgetStylePrivate
{
    Q_DECLARE_PUBLIC(QtTableWidgetStyle)
public:
    int itemHeight;
    int headerMargin;
    int currentHoverRow;
    bool isTransparent;
    QtTableWidgetStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    int horizontalPadding{ 8 };
    QtTableWidgetStylePrivate(QtTableWidgetStyle* q);
    ~QtTableWidgetStylePrivate();
};

QtTableWidgetStylePrivate::QtTableWidgetStylePrivate(QtTableWidgetStyle* q)
    : q_ptr(q)
{
}
QtTableWidgetStylePrivate::~QtTableWidgetStylePrivate()
{
}

QtTableWidgetStyle::QtTableWidgetStyle(QStyle* style)
    : d_ptr(new QtTableWidgetStylePrivate(this))
{
    Q_D(QtTableWidgetStyle);
    d->itemHeight = 35;
    d->headerMargin = 6;
    d->currentHoverRow = -1;
    d->isTransparent = false;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtTableWidgetStyle::~QtTableWidgetStyle()
{
}

void QtTableWidgetStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtTableWidgetStyle);
    switch(element)
    {
        case QStyle::PE_PanelItemViewItem: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                const QtTableWidget* tableWidget = dynamic_cast<const QtTableWidget*>(widget);
                if(!tableWidget)
                {
                    return;
                }

                QVariant backgroundData = vopt->index.data(Qt::BackgroundRole);
                bool hasCustomBackground = backgroundData.isValid();

                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);
                painter->setPen(Qt::NoPen);

                if(hasCustomBackground)
                {
                    QBrush customBrush = backgroundData.value<QBrush>();
                    painter->setBrush(customBrush);
                    painter->drawRect(vopt->rect);
                }

                QAbstractItemView::SelectionBehavior selectionBehavior =
                    tableWidget->selectionBehavior();
                if(selectionBehavior == QAbstractItemView::SelectRows)
                {
                    if(vopt->index.row() == d->currentHoverRow && !hasCustomBackground)
                    {
                        painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                        painter->drawRect(vopt->rect);
                    }
                }
                else
                {
                    if((vopt->state.testFlag(QStyle::State_Selected) ||
                           vopt->state.testFlag(QStyle::State_MouseOver)) &&
                        !hasCustomBackground)
                    {
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
                QVariant backgroundData = vopt->index.data(Qt::BackgroundRole);
                bool hasCustomBackground = backgroundData.isValid();

                if(hasCustomBackground)
                {
                    return;
                }

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

void QtTableWidgetStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtTableWidgetStyle);
    switch(element)
    {
        case QStyle::CE_ShapedFrame: {
            if(!d->isTransparent)
            {
                QRect frameRect = option->rect;
                frameRect.adjust(1, 1, -1, -1);
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing);
                painter->setPen(QtThemeColor(d->themeMode, PopupBorder));
                painter->setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
                painter->drawRoundedRect(frameRect, 3, 3);
                painter->restore();
            }
            return;
        }
        case QStyle::CE_HeaderLabel: {
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
                const QtTableWidget* tableWidget = dynamic_cast<const QtTableWidget*>(widget);
                if(!tableWidget)
                {
                    return;
                }
                this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
                QAbstractItemView::SelectionBehavior selectionBehavior =
                    tableWidget->selectionBehavior();
                QRect itemRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
                QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
                if(vopt->index.column() == 0)
                {
                    iconRect.adjust(d->horizontalPadding, 0, d->horizontalPadding, 0);
                    textRect.adjust(d->horizontalPadding, 0, 0, 0);
                }
                else
                {
                    iconRect.adjust(d->horizontalPadding / 2, 0, d->horizontalPadding / 2, 0);
                    textRect.adjust(d->horizontalPadding, 0, 0, 0);
                }
                if(!vopt->icon.isNull())
                {
                    QIcon::Mode mode = QIcon::Normal;
                    QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                    vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
                }
                if(!vopt->text.isEmpty())
                {
                    QVariant foregroundData = vopt->index.data(Qt::ForegroundRole);
                    if(foregroundData.isValid())
                    {
                        QBrush foregroundBrush = foregroundData.value<QBrush>();
                        painter->setPen(foregroundBrush.color());
                    }
                    else
                    {
                        painter->setPen(QtThemeColor(d->themeMode, BasicText));
                    }
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

QSize QtTableWidgetStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtTableWidgetStyle);
    switch(type)
    {
        case QStyle::CT_ItemViewItem: {
            QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            itemSize.setHeight(d->itemHeight);
            return itemSize;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

int QtTableWidgetStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtTableWidgetStyle);
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

int QtTableWidgetStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget,
    QStyleHintReturn* returnData) const
{
    Q_D(const QtTableWidgetStyle);
    if(hint == QStyle::SH_Table_GridLineColor)
        return QtThemeColor(d->themeMode, BasicBorder).rgba();
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

int QtTableWidgetStyle::itemHeight() const
{
    Q_D(const QtTableWidgetStyle);
    return d->itemHeight;
}
void QtTableWidgetStyle::setItemHeight(int value)
{
    Q_D(QtTableWidgetStyle);
    d->itemHeight = value;
    Q_EMIT itemHeightChanged();
}
int QtTableWidgetStyle::headerMargin() const
{
    Q_D(const QtTableWidgetStyle);
    return d->headerMargin;
}
void QtTableWidgetStyle::setHeaderMargin(int value)
{
    Q_D(QtTableWidgetStyle);
    d->headerMargin = value;
    Q_EMIT headerMarginChanged();
}
int QtTableWidgetStyle::currentHoverRow() const
{
    Q_D(const QtTableWidgetStyle);
    return d->currentHoverRow;
}
void QtTableWidgetStyle::setCurrentHoverRow(int value)
{
    Q_D(QtTableWidgetStyle);
    d->currentHoverRow = value;
    Q_EMIT currentHoverRowChanged();
}
bool QtTableWidgetStyle::isTransparent() const
{
    Q_D(const QtTableWidgetStyle);
    return d->isTransparent;
}
void QtTableWidgetStyle::setIsTransparent(bool value)
{
    Q_D(QtTableWidgetStyle);
    d->isTransparent = value;
    Q_EMIT isTransparentChanged();
}
