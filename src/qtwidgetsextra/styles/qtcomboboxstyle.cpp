#include "qtcomboboxstyle.h"
#include "qstyleoption.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qpainterpath.h>
#include <qproxystyle.h>
#include <qtypes.h>

class QtComboBoxStylePrivate
{
    Q_DECLARE_PUBLIC(QtComboBoxStyle)
public:
    QtThemeType::ThemeMode themeMode;
    int shadowBorderWidth;
    qreal expandIconRotate;
    qreal expandMarkWidth;
    QtComboBoxStyle* q_ptr;
    QtComboBoxStylePrivate(QtComboBoxStyle* q);
    ~QtComboBoxStylePrivate();
};

QtComboBoxStylePrivate::QtComboBoxStylePrivate(QtComboBoxStyle* q)
    : shadowBorderWidth(6)
    , expandIconRotate(0)
    , expandMarkWidth(0)
    , q_ptr(q)
{
}
QtComboBoxStylePrivate::~QtComboBoxStylePrivate()
{
}

QtComboBoxStyle::QtComboBoxStyle(QStyle* style)
    : d_ptr(new QtComboBoxStylePrivate(this))
{
    Q_D(QtComboBoxStyle);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtComboBoxStyle::~QtComboBoxStyle()
{
}

qreal QtComboBoxStyle::expandIconRotate() const
{
    Q_D(const QtComboBoxStyle);
    return d_ptr->expandIconRotate;
}

void QtComboBoxStyle::setExpandIconRotate(qreal value)
{
    Q_D(QtComboBoxStyle);
    d->expandIconRotate = value;
    Q_EMIT expandIconRotateChanged(value);
}

qreal QtComboBoxStyle::expandMarkWidth() const
{
    Q_D(const QtComboBoxStyle);
    return d->expandMarkWidth;
}

void QtComboBoxStyle::setExpandMarkWidth(qreal value)
{
    Q_D(QtComboBoxStyle);
    d->expandMarkWidth = value;
    Q_EMIT expandMarkWidthChanged(value);
}

void QtComboBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtComboBoxStyle);
    switch(element)
    {
        case QStyle::PE_Widget:
        case QStyle::PE_PanelLineEdit: {
            return;
        }
#ifndef Q_OS_WIN
        case PE_PanelMenu: {
            return;
        }
        case PE_IndicatorArrowDown: {
            return;
        }
#endif
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
void QtComboBoxStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtComboBoxStyle);
    switch(element)
    {
        case QStyle::CE_ComboBoxLabel: {
            return;
        }
        case QStyle::CE_ShapedFrame: {
            // container区域
            if(widget->objectName() == "QtComboBoxContainer")
            {
                QRect viewRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                qtTheme->drawEffectShadow(painter, viewRect, d->shadowBorderWidth, 6);
                QRect foregroundRect(viewRect.x() + d->shadowBorderWidth, viewRect.y(),
                    viewRect.width() - 2 * d->shadowBorderWidth,
                    viewRect.height() - d->shadowBorderWidth);
                painter->setPen(QtThemeColor(d->themeMode, PopupBorder));
                painter->setBrush(QtThemeColor(d->themeMode, PopupBase));
                painter->drawRoundedRect(foregroundRect, 3, 3);
                painter->restore();
            }
            return;
        }
        case QStyle::CE_ItemViewItem: {
            // 覆盖高亮
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                int margin = 2;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
                painter->setPen(Qt::NoPen);
                QPainterPath path;
                QRect optionRect = option->rect;
                optionRect.adjust(margin, margin, -margin, -margin);
#ifndef Q_OS_WIN
                optionRect.adjust(6, 0, -6, 0);
#endif
                path.addRoundedRect(optionRect, 5, 5);
                if(option->state & QStyle::State_Selected)
                {
                    if(option->state & QStyle::State_MouseOver)
                    {
                        // 选中时覆盖
                        painter->setBrush(QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
                        painter->drawPath(path);
                    }
                    else
                    {
                        // 选中
                        painter->setBrush(QtThemeColor(d->themeMode, BasicSelectedAlpha));
                        painter->drawPath(path);
                    }
                    // 选中Mark
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                    painter->drawRoundedRect(
                        QRectF(optionRect.x() + 3, optionRect.y() + optionRect.height() * 0.2, 3,
                            optionRect.height() - +optionRect.height() * 0.4),
                        2, 2);
                }
                else
                {
                    if(option->state & QStyle::State_MouseOver)
                    {
                        // 覆盖时颜色
                        painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                        painter->drawPath(path);
                    }
                }
                // 文字绘制
                painter->setPen(QtThemeColor(d->themeMode, BasicText));
                painter->drawText(QRect(option->rect.x() + 15, option->rect.y(),
                                      option->rect.width() - 15, option->rect.height()),
                    Qt::AlignVCenter, vopt->text);
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
void QtComboBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtComboBoxStyle);
    switch(control)
    {
        case QStyle::CC_ComboBox: {
            // 主体显示绘制
            if(const QStyleOptionComboBox* copt =
                    qstyleoption_cast<const QStyleOptionComboBox*>(option))
            {
                painter->save();
                painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing |
                                        QPainter::TextAntialiasing);
                // 背景绘制
                bool isEnabled = copt->state.testFlag(QStyle::State_Enabled);
                painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
                painter->setBrush(
                    isEnabled ? (copt->state.testFlag(QStyle::State_HasFocus) && copt->editable)
                                    ? QtThemeColor(d->themeMode, DialogBase)
                                : copt->state.testFlag(QStyle::State_MouseOver)
                                    ? QtThemeColor(d->themeMode, BasicHover)
                                    : QtThemeColor(d->themeMode, BasicBase)
                              : QtThemeColor(d->themeMode, BasicDisable));
                QRect comboBoxRect = copt->rect;
                comboBoxRect.adjust(d->shadowBorderWidth, 1, -d->shadowBorderWidth, -1);
                painter->drawRoundedRect(comboBoxRect, 3, 3);
                // 底边线绘制
                painter->setPen(QtThemeColor(d->themeMode, BasicBaseLine));
                painter->drawLine(comboBoxRect.x() + 3, comboBoxRect.y() + comboBoxRect.height(),
                    comboBoxRect.x() + comboBoxRect.width() - 3,
                    comboBoxRect.y() + comboBoxRect.height());

                // 文字绘制
                if(!copt->editable)
                {
                    QRect textRect = subControlRect(
                        QStyle::CC_ComboBox, copt, QStyle::SC_ScrollBarSubLine, widget);
                    painter->setPen(isEnabled ? QtThemeColor(d->themeMode, BasicText)
                                              : QtThemeColor(d->themeMode, BasicTextDisable));
                    painter->drawText(
                        textRect, Qt::AlignVCenter | Qt::AlignLeft, copt->currentText);
                }
                // 展开指示器绘制
                painter->setPen(Qt::NoPen);
                painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                painter->drawRoundedRect(
                    QRectF(comboBoxRect.center().x() - d->expandMarkWidth,
                        comboBoxRect.y() + comboBoxRect.height() - 3, d->expandMarkWidth * 2, 3),
                    2, 2);
                // 展开图标绘制
                QRect expandIconRect =
                    subControlRect(QStyle::CC_ComboBox, copt, QStyle::SC_ScrollBarAddPage, widget);
                if(expandIconRect.isValid())
                {
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(17);
                    painter->setFont(iconFont);
                    painter->setPen(isEnabled ? QtThemeColor(d->themeMode, BasicText)
                                              : QtThemeColor(d->themeMode, BasicTextDisable));
                    painter->translate(expandIconRect.x() + (qreal) expandIconRect.width() / 2,
                        expandIconRect.y() + (qreal) expandIconRect.height() / 2);
                    painter->rotate(d->expandIconRotate);
                    painter->translate(-expandIconRect.x() - (qreal) expandIconRect.width() / 2,
                        -expandIconRect.y() - (qreal) expandIconRect.height() / 2);
                    painter->drawText(expandIconRect, Qt::AlignCenter,
                        QChar(static_cast<ushort>(QtIconType::AngleDown)));
                    painter->restore();
                }
            }
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect QtComboBoxStyle::subControlRect(
    ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    Q_D(const QtComboBoxStyle);
    switch(cc)
    {
        case QStyle::CC_ComboBox: {
            switch(sc)
            {
                case QStyle::SC_ScrollBarSubLine: {
                    QRect textRect = QProxyStyle::subControlRect(cc, opt, sc, widget);
                    textRect.setLeft(16);
                    textRect.setRight(textRect.right() - 15);
                    return textRect;
                }
                case QStyle::SC_ScrollBarAddPage: {
                    QRect expandIconRect = QProxyStyle::subControlRect(cc, opt, sc, widget);
                    expandIconRect.setLeft(expandIconRect.left() - 25);
                    return expandIconRect;
                }
                default: {
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}
QSize QtComboBoxStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtComboBoxStyle);
    switch(type)
    {
        case QStyle::CT_ItemViewItem: {
            QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            itemSize.setHeight(35);
            return itemSize;
        }
        case QStyle::CT_ComboBox: {
            QSize comboBoxSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            comboBoxSize.setWidth(comboBoxSize.width() + 26);
            return comboBoxSize;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}