#include "qttreeviewstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qpainter.h>
#include <qpainterpath.h>
#include <qstyleoption.h>
#include <qtypes.h>

class QtTreeViewStylePrivate
{
    Q_DECLARE_PUBLIC(QtTreeViewStyle)
public:
    QtTreeViewStyle* q_ptr;
    int itemHeight;
    int headerMargin;
    int leftPadding{ 11 };
    QtThemeType::ThemeMode themeMode;
    QtTreeViewStylePrivate(QtTreeViewStyle* q);
    ~QtTreeViewStylePrivate();
};

QtTreeViewStylePrivate::QtTreeViewStylePrivate(QtTreeViewStyle* q)
    : q_ptr(q)
{
}
QtTreeViewStylePrivate::~QtTreeViewStylePrivate()
{
}

QtTreeViewStyle::QtTreeViewStyle(QStyle* style)
    : d_ptr(new QtTreeViewStylePrivate(this))
{
    Q_D(QtTreeViewStyle);
    d->itemHeight = 35;
    d->headerMargin = 5;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtTreeViewStyle::~QtTreeViewStyle()
{
}

void QtTreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtTreeViewStyle);
    switch(element)
    {
        case QStyle::PE_PanelItemViewItem: {
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
                        painter->fillPath(
                            path, QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
                    }
                    else
                    {
                        painter->fillPath(path, QtThemeColor(d->themeMode, BasicSelectedAlpha));
                    }
                }
                else
                {
                    if(vopt->state & QStyle::State_MouseOver)
                    {
                        painter->fillPath(path, QtThemeColor(d->themeMode, BasicHoverAlpha));
                    }
                }
                painter->restore();
            }
            return;
        }
        case QStyle::PE_IndicatorBranch: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                if(vopt->state.testFlag(QStyle::State_Children))
                {
                    painter->save();
                    QRect indicatorRect = option->rect;
                    indicatorRect.adjust(0, 0, -2, 0);
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(17);
                    painter->setFont(iconFont);
                    painter->setPen(QtThemeColor(d->themeMode, BasicText));
                    painter->drawText(indicatorRect, Qt::AlignVCenter | Qt::AlignRight,
                        vopt->state.testFlag(QStyle::State_Open)
                            ? QChar(static_cast<ushort>(QtIconType::AngleDown))
                            : QChar(static_cast<ushort>(QtIconType::AngleRight)));
                    painter->restore();
                }
            }
            return;
        }
        case QStyle::PE_PanelItemViewRow: {
            return;
        }
        case QStyle::PE_Widget: {
            return;
        }
        case QStyle::PE_Frame: {
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QtTreeViewStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtTreeViewStyle);
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
            painter->restore();
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
                QRect checkRect =
                    proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
                QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
                QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
                if(checkRect.isValid())
                {
                    painter->save();
                    if(vopt->checkState == Qt::Checked)
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                        painter->drawRoundedRect(checkRect, 2, 2);
                        QFont iconFont = QFont("ElaAwesome");
                        iconFont.setPixelSize(checkRect.width() * 0.85);
                        painter->setFont(iconFont);
                        painter->setPen(QtThemeColor(QtThemeType::Dark, BasicText));
                        painter->drawText(checkRect, Qt::AlignCenter,
                            QChar(static_cast<ushort>(QtIconType::Check)));
                    }
                    else if(vopt->checkState == Qt::PartiallyChecked)
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                        painter->drawRoundedRect(checkRect, 2, 2);
                        painter->setPen(QtThemeColor(QtThemeType::Dark, BasicText));
                        QLine checkLine(checkRect.x() + 3, checkRect.center().y(),
                            checkRect.right() - 3, checkRect.center().y());
                        painter->drawLine(checkLine);
                    }
                    else
                    {
                        painter->setPen(QPen(QtThemeColor(d->themeMode, BasicBorderDeep), 1.2));
                        painter->setBrush(Qt::transparent);
                        painter->drawRoundedRect(checkRect, 2, 2);
                    }
                    painter->restore();
                }
                if(!vopt->icon.isNull())
                {
                    QIcon::Mode mode = QIcon::Normal;
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
                if(vopt->state.testFlag(QStyle::State_Selected) &&
                    (vopt->viewItemPosition == QStyleOptionViewItem::Beginning ||
                        vopt->viewItemPosition == QStyleOptionViewItem::OnlyOne))
                {
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

QSize QtTreeViewStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtTreeViewStyle);
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

int QtTreeViewStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtTreeViewStyle);
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

QRect QtTreeViewStyle::subElementRect(
    SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtTreeViewStyle);
    switch(element)
    {
        case QStyle::SE_ItemViewItemCheckIndicator: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                if(vopt->viewItemPosition != QStyleOptionViewItem::Middle &&
                    vopt->viewItemPosition != QStyleOptionViewItem::End)
                {
                    QRect indicatorRect = QProxyStyle::subElementRect(element, option, widget);
                    indicatorRect.adjust(d->leftPadding, 0, d->leftPadding, 0);
                    return indicatorRect;
                }
            }
            break;
        }
        case QStyle::SE_ItemViewItemDecoration: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                if(vopt->viewItemPosition != QStyleOptionViewItem::Middle &&
                    vopt->viewItemPosition != QStyleOptionViewItem::End)
                {
                    QRect iconRect = QProxyStyle::subElementRect(element, option, widget);
                    iconRect.adjust(d->leftPadding + 5, 0, d->leftPadding + 5, 0);
                    return iconRect;
                }
            }
            break;
        }
        case QStyle::SE_ItemViewItemText: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                if(vopt->viewItemPosition != QStyleOptionViewItem::Middle &&
                    vopt->viewItemPosition != QStyleOptionViewItem::End)
                {
                    QRect textRect = QProxyStyle::subElementRect(element, option, widget);
                    textRect.adjust(d->leftPadding + 10, 0, 0, 0);
                    return textRect;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}

int QtTreeViewStyle::itemHeight() const
{
    Q_D(const QtTreeViewStyle);
    return d->itemHeight;
}
void QtTreeViewStyle::setItemHeight(int value)
{
    Q_D(QtTreeViewStyle);
    d->itemHeight = value;
}

int QtTreeViewStyle::headerMargin() const
{
    Q_D(const QtTreeViewStyle);
    return d->headerMargin;
}
void QtTreeViewStyle::setHeaderMargin(int value)
{
    Q_D(QtTreeViewStyle);
    d->headerMargin = value;
}