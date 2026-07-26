#include "qttoolbuttonstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <QPainter>
#include <QStyle>
#include <qtypes.h>


class QtToolButtonStylePrivate
{
    Q_DECLARE_PUBLIC(QtToolButtonStyle)
public:
    bool isTransparent;
    int borderRadius;
    qreal expandIconRotate;
    bool isSelected;
    QtToolButtonStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    int contentMargin{ 6 };
    int hasMenuIndicatorWidth{ 30 };
    QtToolButtonStylePrivate(QtToolButtonStyle* q);
    void drawIndicator(
        QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt,
        const QWidget* widget) const;
    void drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const;
    qreal calculateExpandIndicatorWidth(
        const QStyleOptionToolButton* bopt, QPainter* painter) const;
};

QtToolButtonStylePrivate::QtToolButtonStylePrivate(QtToolButtonStyle* q)
    : q_ptr(q)
{
    isSelected = false;
    isTransparent = true;
    expandIconRotate = 0;
    borderRadius = 4;
    themeMode = qtTheme->getThemeMode();
    QObject::connect(qtTheme, &QtTheme::themeModeChanged, q,
        [this](QtThemeType::ThemeMode theme) { this->themeMode = theme; });
}

void QtToolButtonStylePrivate::drawIndicator(
    QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    Q_Q(const QtToolButtonStyle);
    if(bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
    {
        QRect indicatorRect =
            q->subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
        if(bopt->state.testFlag(QStyle::State_Enabled) &&
            bopt->activeSubControls.testFlag(QStyle::SC_ScrollBarSubLine))
        {
            painter->setBrush(QtThemeColor(themeMode, BasicIndicator));
            QPainterPath path;
            path.moveTo(indicatorRect.topLeft());
            path.lineTo(indicatorRect.right() - 4, indicatorRect.y());
            path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.y(), 8, 8), 90, -90);
            path.lineTo(indicatorRect.right(), indicatorRect.bottom() - 4);
            path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.bottom() - 8, 8, 8), 0, -90);
            path.lineTo(indicatorRect.bottomLeft());
            path.closeSubpath();
            painter->drawPath(path);
        }
        painter->setBrush(bopt->state.testFlag(QStyle::State_Enabled)
                              ? QtThemeColor(themeMode, BasicText)
                              : QtThemeColor(themeMode, BasicTextDisable));
        QPainterPath indicatorPath;
        qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
        indicatorPath.moveTo(
            indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(
            indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(
            indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
        indicatorPath.closeSubpath();
        painter->drawPath(indicatorPath);
    }
    else if(bopt->features.testFlag(QStyleOptionToolButton::HasMenu))
    {
        QSize iconSize = bopt->iconSize;
        painter->save();
        QRect toolButtonRect = bopt->rect;
        QFont iconFont = QFont("QtAwesome");
        iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
        painter->setFont(iconFont);
        int indicatorWidth = painter->fontMetrics().horizontalAdvance(
            QChar(static_cast<ushort>(QtIconType::AngleDown)));
        QRect expandIconRect(toolButtonRect.right() - contentMargin - indicatorWidth,
            toolButtonRect.y() + 1, indicatorWidth, toolButtonRect.height());
        painter->setPen(QtThemeColor(themeMode, BasicText));
        painter->translate(
            expandIconRect.center().x(), expandIconRect.y() + (qreal) expandIconRect.height() / 2);
        painter->rotate(expandIconRotate);
        painter->translate(-expandIconRect.center().x() - 1,
            -expandIconRect.y() - (qreal) expandIconRect.height() / 2);
        painter->drawText(
            expandIconRect, Qt::AlignCenter, QChar(static_cast<ushort>(QtIconType::AngleDown)));
        painter->restore();
    }
}
void QtToolButtonStylePrivate::drawIcon(QPainter* painter, QRectF iconRect,
    const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if(bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize iconSize = bopt->iconSize;
        if(widget->property("QtIconType").toString().isEmpty())
        {
            QIcon icon = bopt->icon;
            if(!icon.isNull())
            {
                QPixmap iconPix = icon.pixmap(iconSize,
                    (bopt->state & QStyle::State_Enabled) ? QIcon::Normal : QIcon::Disabled,
                    (bopt->state & QStyle::State_Selected) ? QIcon::On : QIcon::Off);
                switch(bopt->toolButtonStyle)
                {
                    case Qt::ToolButtonIconOnly: {
                        painter->drawPixmap(QRect(QPoint(iconRect.x(), iconRect.center().y() -
                                                                           iconSize.height() / 2),
                                                iconSize),
                            iconPix);
                        break;
                    }
                    case Qt::ToolButtonFollowStyle:
                    case Qt::ToolButtonTextBesideIcon: {
                        painter->drawPixmap(
                            QRect(QPoint(iconRect.x() + contentMargin,
                                      iconRect.center().y() - iconSize.height() / 2),
                                iconSize),
                            iconPix);
                        break;
                    }
                    case Qt::ToolButtonTextUnderIcon: {
                        if(bopt->features.testFlag(QStyleOptionToolButton::HasMenu) &&
                            !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                        {
                            iconRect.setRight(
                                iconRect.right() - calculateExpandIndicatorWidth(bopt, painter));
                        }
                        painter->drawPixmap(
                            QRect(
                                QPoint(iconRect.center().x() - iconSize.width() / 2, iconRect.y()),
                                iconSize),
                            iconPix);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
        else
        {
            painter->save();
            if(bopt->state.testFlag(QStyle::State_Enabled))
            {
                painter->setPen(QtThemeColor(themeMode, BasicText));
            }
            else
            {
                painter->setPen(QtThemeColor(themeMode, BasicTextDisable));
            }
            QFont iconFont = QFont("ElaAwesome");
            int iconRotate = widget->property("QtIconRotate").toInt();
            switch(bopt->toolButtonStyle)
            {
                case Qt::ToolButtonIconOnly: {
                    iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                    painter->setFont(iconFont);
                    if(iconRotate)
                    {
                        painter->translate(iconRect.center());
                        painter->rotate(iconRotate);
                        painter->translate(-iconRect.center());
                    }
                    painter->drawText(
                        iconRect, Qt::AlignCenter, widget->property("QtIconType").toString());
                    break;
                }
                case Qt::ToolButtonFollowStyle:
                case Qt::ToolButtonTextBesideIcon: {
                    QRect adjustIconRect(iconRect.x() + contentMargin, iconRect.y(),
                        iconSize.width(), iconRect.height());
                    iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                    painter->setFont(iconFont);
                    if(iconRotate)
                    {
                        painter->translate(adjustIconRect.center());
                        painter->rotate(iconRotate);
                        painter->translate(-adjustIconRect.center());
                    }
                    painter->drawText(adjustIconRect, Qt::AlignCenter,
                        widget->property("QtIconType").toString());
                    break;
                }
                case Qt::ToolButtonTextUnderIcon: {
                    if(bopt->features.testFlag(QStyleOptionToolButton::HasMenu) &&
                        !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                    {
                        iconRect.setRight(
                            iconRect.right() - calculateExpandIndicatorWidth(bopt, painter));
                    }
                    QRect adjustIconRect(iconRect.center().x() - iconSize.width() / 2,
                        iconRect.y() + 0.2 * std::min(iconSize.width(), iconSize.height()),
                        iconSize.width(), iconSize.height());
                    iconFont.setPixelSize(0.8 * std::min(iconSize.width(), iconSize.height()));
                    painter->setFont(iconFont);
                    if(iconRotate)
                    {
                        painter->translate(adjustIconRect.center());
                        painter->rotate(iconRotate);
                        painter->translate(-adjustIconRect.center());
                    }
                    painter->drawText(adjustIconRect, Qt::AlignHCenter,
                        widget->property("QtIconType").toString());
                    break;
                }
                default: {
                    break;
                }
            }
            painter->restore();
        }
    }
}
void QtToolButtonStylePrivate::drawText(
    QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const
{
    if(!bopt->text.isEmpty())
    {
        if(bopt->state.testFlag(QStyle::State_Enabled))
        {
            painter->setPen(QtThemeColor(themeMode, BasicText));
        }
        else
        {
            painter->setPen(QtThemeColor(themeMode, BasicTextDisable));
        }
        switch(bopt->toolButtonStyle)
        {
            case Qt::ToolButtonTextOnly: {
                contentRect.setLeft(contentRect.left() + contentMargin);
                painter->drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
                break;
            }
            case Qt::ToolButtonTextBesideIcon: {
                painter->drawText(
                    QRect(contentRect.x() + contentMargin * 2 + bopt->iconSize.width(),
                        contentRect.y(), contentRect.width() - bopt->iconSize.width(),
                        contentRect.height()),
                    Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
                break;
            }
            case Qt::ToolButtonTextUnderIcon: {
                if(bopt->features.testFlag(QStyleOptionToolButton::HasMenu) &&
                    !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                {
                    contentRect.setLeft(contentRect.left() + contentMargin);
                    painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignLeft, bopt->text);
                }
                else
                {
                    painter->drawText(contentRect, Qt::AlignBottom | Qt::AlignHCenter, bopt->text);
                }
                break;
            }
            case Qt::ToolButtonFollowStyle: {
                break;
            }
            default: {
                break;
            }
        }
    }
}
qreal QtToolButtonStylePrivate::calculateExpandIndicatorWidth(
    const QStyleOptionToolButton* bopt, QPainter* painter) const
{
    Q_Q(const QtToolButtonStyle);
    QSize iconSize = bopt->iconSize;
    painter->save();
    QFont iconFont = QFont("QtAwesome");
    iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
    painter->setFont(iconFont);
    int indicatorWidth =
        painter->fontMetrics().horizontalAdvance(QChar(static_cast<ushort>(QtIconType::AngleDown)));
    painter->restore();
    return indicatorWidth;
}

QtToolButtonStyle::QtToolButtonStyle(QStyle* style)
    : QProxyStyle(style)
    , d_ptr(new QtToolButtonStylePrivate(this))
{
}

QtToolButtonStyle::~QtToolButtonStyle()
{
}

bool QtToolButtonStyle::isTransparent() const
{
    Q_D(const QtToolButtonStyle);
    return d->isTransparent;
}

void QtToolButtonStyle::setIsTransparent(bool newIsTransparent)
{
    Q_D(QtToolButtonStyle);
    if(d->isTransparent == newIsTransparent)
        return;
    d->isTransparent = newIsTransparent;
    Q_EMIT isTransparentChanged();
}

int QtToolButtonStyle::borderRadius() const
{
    Q_D(const QtToolButtonStyle);
    return d->borderRadius;
}

void QtToolButtonStyle::setBorderRadius(int newBorderRadius)
{
    Q_D(QtToolButtonStyle);
    if(d->borderRadius == newBorderRadius)
        return;
    d->borderRadius = newBorderRadius;
    Q_EMIT borderRadiusChanged();
}

qreal QtToolButtonStyle::expandIconRotate() const
{
    Q_D(const QtToolButtonStyle);
    return d->expandIconRotate;
}

void QtToolButtonStyle::setExpandIconRotate(qreal newExpandIconRotate)
{
    Q_D(QtToolButtonStyle);
    if(qFuzzyCompare(d->expandIconRotate, newExpandIconRotate))
        return;
    d->expandIconRotate = newExpandIconRotate;
    Q_EMIT expandIconRotateChanged();
}

bool QtToolButtonStyle::isSelected() const
{
    Q_D(const QtToolButtonStyle);
    return d->isSelected;
}

void QtToolButtonStyle::setIsSelected(bool newIsSelected)
{
    Q_D(QtToolButtonStyle);
    if(d->isSelected == newIsSelected)
        return;
    d->isSelected = newIsSelected;
    Q_EMIT isSelectedChanged();
}

void QtToolButtonStyle::drawComplexControl(ComplexControl control,
    const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtToolButtonStyle);
    switch(control)
    {
        case QStyle::CC_ToolButton: {
            if(const QStyleOptionToolButton* bopt =
                    qstyleoption_cast<const QStyleOptionToolButton*>(option))
            {
                if(bopt->arrowType != Qt::NoArrow)
                {
                    break;
                }
                QRect toolButtonRect = bopt->rect;
                if(!d->isTransparent)
                {
                    toolButtonRect.adjust(1, 1, -1, -1);
                }
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                                        QPainter::SmoothPixmapTransform);
                painter->setPen(
                    d->isTransparent ? Qt::transparent : QtThemeColor(d->themeMode, BasicBorder));
                if(bopt->state.testFlag(QStyle::State_Enabled))
                {
                    if(bopt->state.testFlag(QStyle::State_Sunken))
                    {
                        painter->setBrush(d->isTransparent
                                              ? QtThemeColor(d->themeMode, BasicPressAlpha)
                                              : QtThemeColor(d->themeMode, BasicPress));
                        painter->drawRoundedRect(toolButtonRect, d->borderRadius, d->borderRadius);
                    }
                    else
                    {
                        if(d->isSelected)
                        {
                            painter->setBrush(d->isTransparent
                                                  ? QtThemeColor(d->themeMode, BasicSelectedAlpha)
                                                  : QtThemeColor(d->themeMode, BasicHover));
                            painter->drawRoundedRect(
                                toolButtonRect, d->borderRadius, d->borderRadius);
                        }
                        else
                        {
                            if(bopt->state.testFlag(QStyle::State_MouseOver) ||
                                bopt->state.testFlag(QStyle::State_On))
                            {
                                painter->setBrush(d->isTransparent
                                                      ? QtThemeColor(d->themeMode, BasicHoverAlpha)
                                                      : QtThemeColor(d->themeMode, BasicHover));
                                painter->drawRoundedRect(
                                    toolButtonRect, d->borderRadius, d->borderRadius);
                            }
                            else
                            {
                                if(!d->isTransparent)
                                {
                                    painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
                                    painter->drawRoundedRect(
                                        toolButtonRect, d->borderRadius, d->borderRadius);
                                    painter->setPen(QtThemeColor(d->themeMode, BasicBaseLine));
                                    painter->drawLine(toolButtonRect.x() + d->borderRadius,
                                        toolButtonRect.y() + toolButtonRect.height(),
                                        toolButtonRect.x() + toolButtonRect.width() -
                                            d->borderRadius,
                                        toolButtonRect.y() + toolButtonRect.height());
                                }
                            }
                        }
                    }
                }
                d->drawIndicator(painter, bopt, widget);
                QRect contentRect =
                    subControlRect(control, bopt, QStyle::SC_ScrollBarAddLine, widget);
                int heightOffset = contentRect.height() * 0.05;
                contentRect.adjust(0, heightOffset, 0, -heightOffset);
                d->drawIcon(painter, contentRect, bopt, widget);
                d->drawText(painter, contentRect, bopt);
                painter->restore();
            }
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}
QSize QtToolButtonStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtToolButtonStyle);
    if(type == QStyle::CT_ToolButton)
    {
        if(const QStyleOptionToolButton* bopt =
                qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            QSize toolButtonSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            if(bopt->features.testFlag(QStyleOptionToolButton::HasMenu) &&
                !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                toolButtonSize.setWidth(
                    toolButtonSize.width() + d->contentMargin +
                    0.65 * std::min(bopt->iconSize.width(), bopt->iconSize.height()));
            }
            return toolButtonSize;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
