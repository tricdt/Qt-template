#include "qtmenustyle.h"
#include "qttheme.h"
#include "widgets/qtmenu.h"
#include <QStyleOption>
#include <qtclasshelpermacros.h>
#include <qtpreprocessorsupport.h>

class QtMenuStylePrivate
{
    Q_DECLARE_PUBLIC(QtMenuStyle)
public:
    mutable bool isAnyoneItemHasIcon{ false };
    int shadowBorderWidth{ 6 };
    QtThemeType::ThemeMode themeMode;
    mutable int iconWidth{ 0 };
    int menuItemHeight{ 32 };
    QtMenuStyle* q_ptr;
    QtMenuStylePrivate(QtMenuStyle* q) : q_ptr(q){}
};

QtMenuStyle::QtMenuStyle(QStyle* style)
    : d_ptr(new QtMenuStylePrivate(this))
{
    Q_D(QtMenuStyle);
    Q_UNUSED(style)
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}
QtMenuStyle::~QtMenuStyle()
{
}

int QtMenuStyle::menuItemHeight() const
{
    Q_D(const QtMenuStyle);
    return d->menuItemHeight;
}
void QtMenuStyle::setMenuItemHeight(int height)
{
    Q_D(QtMenuStyle);
    d->menuItemHeight = height;
}

void QtMenuStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtMenuStyle);
    switch(element)
    {
        case QStyle::PE_PanelMenu: {
            // 高性能阴影
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            qtTheme->drawEffectShadow(painter, option->rect, d->shadowBorderWidth, 6);
            // 背景绘制
            QRect foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth,
                option->rect.width() - 2 * d->shadowBorderWidth,
                option->rect.height() - 2 * d->shadowBorderWidth);
            painter->setPen(QtThemeColor(d->themeMode, PopupBorder));
            painter->setBrush(QtThemeColor(d->themeMode, PopupBase));
            painter->drawRoundedRect(foregroundRect, 6, 6);
            painter->restore();
            return;
        }
        case QStyle::PE_FrameMenu: {
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
void QtMenuStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
    const QWidget* widget) const
{
    Q_D(const QtMenuStyle);
    switch(element)
    {
        case QStyle::CE_MenuItem: {
            // 内容绘制 区分类型
            if(const QStyleOptionMenuItem* mopt =
                    qstyleoption_cast<const QStyleOptionMenuItem*>(option))
            {
                if(mopt->menuItemType == QStyleOptionMenuItem::Separator)
                {
                    QRect separatorRect = mopt->rect;
                    painter->save();
                    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, BasicBaseLine));
                    painter->drawRoundedRect(
                        QRectF(separatorRect.x() + separatorRect.width() * 0.055,
                            separatorRect.center().y(),
                            separatorRect.width() - separatorRect.width() * 0.11, 1.5),
                        1, 1);
                    painter->restore();
                    return;
                }
                else
                {
                    QRect menuRect = mopt->rect;
                    qreal contentPadding = menuRect.width() * 0.055;
                    qreal textLeftSpacing = 8;
                    painter->save();
                    painter->setRenderHints(QPainter::SmoothPixmapTransform |
                                            QPainter::Antialiasing | QPainter::TextAntialiasing);
                    // 覆盖效果
                    if(mopt->state.testFlag(QStyle::State_Enabled) &&
                        (mopt->state.testFlag(QStyle::State_MouseOver) ||
                            mopt->state.testFlag(QStyle::State_Selected)))
                    {
                        QRect hoverRect = menuRect;
                        hoverRect.adjust(0, 2, 0, -2);
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(QtThemeColor(d->themeMode, PopupHover));
                        painter->drawRoundedRect(hoverRect, 5, 5);
                    }
                    // Icon绘制
                    QIcon menuIcon = mopt->icon;
                    // check绘制
                    if(mopt->menuHasCheckableItems)
                    {
                        painter->save();
                        painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray
                                        : d->themeMode == QtThemeType::Light         ? Qt::black
                                                                                     : Qt::white);
                        QFont iconFont = QFont("ElaAwesome");
                        QFont emptyFOnt = QFont("");
                        iconFont.setPixelSize(d->menuItemHeight * 0.57);
                        painter->setFont(iconFont);
                        painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(),
                                              d->iconWidth, menuRect.height()),
                            Qt::AlignCenter,
                            mopt->checked
                                ? QChar::fromUcs2(static_cast<char16_t>(QtIconType::Check))
                                : QChar::fromUcs2(static_cast<char16_t>(QtIconType::None)));
                        painter->restore();
                    }
                    else
                    {
                        QString iconText;
                        const QtMenu* menu = dynamic_cast<const QtMenu*>(widget);
                        if(menu)
                        {
                            QAction* action = menu->actionAt(menuRect.center());
                            if(action)
                            {
                                iconText = action->property("QtIconType").toString();
                            }
                        }
                        if(!iconText.isEmpty())
                        {
                            painter->save();
                            painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray
                                            : d->themeMode == QtThemeType::Light         ? Qt::black
                                                                                 : Qt::white);
                            QFont iconFont = QFont("ElaAwesome");
                            iconFont.setPixelSize(d->menuItemHeight * 0.57);
                            painter->setFont(iconFont);
                            painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(),
                                                  d->iconWidth, menuRect.height()),
                                Qt::AlignCenter, iconText);
                            painter->restore();
                        }
                        else
                        {
                            if(!menuIcon.isNull())
                            {
                                painter->drawPixmap(QRect(menuRect.x() + contentPadding,
                                                        menuRect.center().y() - d->iconWidth / 2,
                                                        d->iconWidth, d->iconWidth),
                                    menuIcon.pixmap(d->iconWidth, d->iconWidth));
                            }
                        }
                    }
                    // 文字和快捷键绘制
                    if(!mopt->text.isEmpty())
                    {
                        QStringList textList = mopt->text.split("\t");
                        painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray
                                        : d->themeMode == QtThemeType::Light         ? Qt::black
                                                                                     : Qt::white);

                        painter->drawText(
                            QRectF(menuRect.x() +
                                       (d->isAnyoneItemHasIcon ? contentPadding + textLeftSpacing
                                                               : 0) +
                                       d->iconWidth,
                                menuRect.y(), menuRect.width(), menuRect.height()),
                            Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, textList[0]);
                        if(textList.count() > 1)
                        {
                            painter->drawText(
                                QRectF(
                                    menuRect.x() + contentPadding + d->iconWidth + textLeftSpacing,
                                    menuRect.y(),
                                    menuRect.width() -
                                        (contentPadding * 2 + d->iconWidth + textLeftSpacing),
                                    menuRect.height()),
                                Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine,
                                textList[1]);
                        }
                    }
                    // 展开图标
                    if(mopt->menuItemType == QStyleOptionMenuItem::SubMenu)
                    {
                        painter->save();
                        painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray
                                        : d->themeMode == QtThemeType::Light         ? Qt::black
                                                                                     : Qt::white);
                        QFont iconFont = QFont("ElaAwesome");
                        iconFont.setPixelSize(18);
                        painter->setFont(iconFont);
                        painter->drawText(
                            QRect(menuRect.right() - 25, menuRect.y(), 25, menuRect.height()),
                            Qt::AlignVCenter,
                            QChar::fromUcs2(static_cast<char16_t>(QtIconType::AngleRight)));
                        painter->restore();
                    }
                    painter->restore();
                }
            }

            return;
        }
        case QStyle::CE_MenuEmptyArea: {
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}
int QtMenuStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtMenuStyle);
    switch(metric)
    {
        case QStyle::PM_SmallIconSize: {
            // 图标宽度
            d->iconWidth = d->menuItemHeight * 0.7;
            return d->iconWidth;
        }
        case QStyle::PM_MenuPanelWidth: {
            // 外围容器宽度
            return d->shadowBorderWidth * 1.5;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
QSize QtMenuStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtMenuStyle);
    switch(type)
    {
        case QStyle::CT_MenuItem: {
            if(const QStyleOptionMenuItem* mopt =
                    qstyleoption_cast<const QStyleOptionMenuItem*>(option))
            {
                if(mopt->menuItemType == QStyleOptionMenuItem::Separator)
                {
                    break;
                }
                QSize menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
                const QtMenu* menu = dynamic_cast<const QtMenu*>(widget);
                if(menu->isHasIcon() || mopt->menuHasCheckableItems)
                {
                    d->isAnyoneItemHasIcon = true;
                }
                if(menu->isHasChildMenu())
                {
                    return QSize(menuItemSize.width() + 20, d->menuItemHeight);
                }
                else
                {
                    return QSize(menuItemSize.width(), d->menuItemHeight);
                }
            }
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}