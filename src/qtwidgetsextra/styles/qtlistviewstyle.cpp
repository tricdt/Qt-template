#include "qtlistviewstyle.h"
#include "qttheme.h"
#include "widgets/qtlistview.h"
#include <qpainterpath.h>

class QtListViewStylePrivate
{
    Q_DECLARE_PUBLIC(QtListViewStyle)
public:
    int itemHeight;
    int isTransparent;
    QtListViewStyle* q_ptr;
    int leftPadding{ 11 };
    QtThemeType::ThemeMode themeMode;
    QtListViewStylePrivate(QtListViewStyle* q);
    ~QtListViewStylePrivate();
};

QtListViewStylePrivate::QtListViewStylePrivate(QtListViewStyle* q)
    : q_ptr(q)
{
}
QtListViewStylePrivate::~QtListViewStylePrivate()
{
}

QtListViewStyle::QtListViewStyle(QStyle* style)
    : d_ptr(new QtListViewStylePrivate(this))
{
    Q_D(QtListViewStyle);
    d->itemHeight = 35;
    d->isTransparent = false;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtListViewStyle::~QtListViewStyle()
{
}

int QtListViewStyle::itemHeight() const
{
    Q_D(const QtListViewStyle);
    return d->itemHeight;
}
void QtListViewStyle::setItemHeight(int value)
{
    Q_D(QtListViewStyle);
    d->itemHeight = value;
}

bool QtListViewStyle::isTransparent() const
{
    Q_D(const QtListViewStyle);
    return d->isTransparent;
}
void QtListViewStyle::setIsTransparent(bool value)
{
    Q_D(QtListViewStyle);
    d->isTransparent = value;
}

void QtListViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtListViewStyle);
    switch(element)
    {
        case QStyle::PE_PanelItemViewItem: {
            // Item背景
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);
                QRect itemRect = vopt->rect;
                itemRect.adjust(0, 2, 0, -2);
                QPainterPath path;
                path.addRoundedRect(itemRect, 4, 4);
                if(vopt->state & QStyle::State_Selected)
                {
                    if(vopt->state & QStyle::State_MouseOver)
                    {
                        // 选中时覆盖
                        painter->fillPath(path, QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
                    }
                    else
                    {
                        // 选中
                        painter->fillPath(path, QtThemeColor(d->themeMode, BasicSelectedAlpha));
                    }
                }
                else
                {
                    if(vopt->state & QStyle::State_MouseOver)
                    {
                        // 覆盖时颜色
                        painter->fillPath(path, QtThemeColor(d->themeMode, BasicHoverAlpha));
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
                if(vopt->features == QStyleOptionViewItem::Alternate)
                {
                    painter->save();
                    painter->setRenderHint(QPainter::Antialiasing);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, BasicAlternating));
                    painter->drawRect(vopt->rect);
                    painter->restore();
                }
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

void QtListViewStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtListViewStyle);
    switch(element)
    {
        case QStyle::CE_ShapedFrame: {
            // viewport视口外的其他区域背景
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
        case QStyle::CE_ItemViewItem: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);

                QRect itemRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                const QtListView* listView = dynamic_cast<const QtListView*>(widget);
                QListView::ViewMode viewMode = listView->viewMode();
                // QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt,
                // widget);
                QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
                QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
                iconRect.adjust(d->leftPadding, 0, 0, 0);
                textRect.adjust(d->leftPadding, 0, 0, 0);
                if(!vopt->icon.isNull())
                {
                    QIcon::Mode mode = QIcon::Normal;
                    if(!(vopt->state.testFlag(QStyle::State_Enabled)))
                    {
                        mode = QIcon::Disabled;
                    }
                    else if(vopt->state.testFlag(QStyle::State_Selected))
                    {
                        mode = QIcon::Selected;
                    }
                    QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                    vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
                }
                if(!vopt->text.isEmpty())
                {
                    painter->setPen(QtThemeColor(d->themeMode, BasicText));
                    painter->drawText(textRect, vopt->displayAlignment, vopt->text);
                }
                if(vopt->state.testFlag(QStyle::State_Selected) && viewMode == QListView::ListMode)
                {
                    int heightOffset = itemRect.height() / 4;
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                    painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset,
                                                 3, itemRect.height() - 2 * heightOffset),
                        3, 3);
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

QSize QtListViewStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtListViewStyle);
    switch(type)
    {
        case QStyle::CT_ItemViewItem: {
            QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            const QtListView* listView = dynamic_cast<const QtListView*>(widget);
            QListView::ViewMode viewMode = listView->viewMode();
            if(viewMode == QListView::ListMode)
            {
                itemSize.setWidth(itemSize.width() + d->leftPadding);
            }
            itemSize.setHeight(d->itemHeight);
            return itemSize;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
