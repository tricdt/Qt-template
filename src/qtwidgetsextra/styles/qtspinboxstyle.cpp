#include "qtspinboxstyle.h"

#include "qtdef.h"
#include "qttheme.h"
#include <cmath>
#include <qpainterpath.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qtypes.h>

class QtSpinBoxStylePrivate
{
    Q_DECLARE_PUBLIC(QtSpinBoxStyle)
public:
    QtSpinBoxType::ButtonMode buttonMode;
    QtSpinBoxStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtSpinBoxStylePrivate(QtSpinBoxStyle* q);
    ~QtSpinBoxStylePrivate();
};

QtSpinBoxStylePrivate::QtSpinBoxStylePrivate(QtSpinBoxStyle* q)
    : q_ptr(q)
{
}
QtSpinBoxStylePrivate::~QtSpinBoxStylePrivate()
{
}

QtSpinBoxStyle::QtSpinBoxStyle(QStyle*)
    : d_ptr(new QtSpinBoxStylePrivate(this))
{
    Q_D(QtSpinBoxStyle);
    d->buttonMode = QtSpinBoxType::Inline;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtSpinBoxStyle::~QtSpinBoxStyle()
{
}

QtSpinBoxType::ButtonMode QtSpinBoxStyle::buttonMode() const
{
    Q_D(const QtSpinBoxStyle);
    return d->buttonMode;
}

void QtSpinBoxStyle::setButtonMode(QtSpinBoxType::ButtonMode buttonMode)
{
    Q_D(QtSpinBoxStyle);
    d->buttonMode = buttonMode;
}

void QtSpinBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtSpinBoxStyle);
    switch(control)
    {
        case QStyle::CC_SpinBox: {
            const QStyleOptionSpinBox* sopt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
            if(!sopt)
            {
                break;
            }
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            // 背景
            QRect spinBoxRect = sopt->rect.adjusted(1, 1, -1, -1);
            painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
            if(sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(QtThemeColor(d->themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
            }
            painter->drawRoundedRect(spinBoxRect, 4, 4);
            // 添加按钮
            QRect addLineRect = subControlRect(control, sopt, SC_ScrollBarAddLine, widget);
            if(sopt->activeSubControls == SC_ScrollBarAddLine)
            {
                painter->setPen(Qt::NoPen);
                if(sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicPressAlpha));
                }
                else
                {
                    if(sopt->state & QStyle::State_MouseOver)
                    {
                        painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                    }
                }
                painter->drawRoundedRect(addLineRect, 4, 4);
            }

            // 减少按钮
            QRect subLineRect = subControlRect(control, sopt, SC_ScrollBarSubLine, widget);
            if(sopt->activeSubControls == SC_ScrollBarSubLine)
            {
                painter->setPen(Qt::NoPen);
                if(sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicPressAlpha));
                }
                else
                {
                    if(sopt->state & QStyle::State_MouseOver)
                    {
                        painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                    }
                }
                painter->drawRoundedRect(subLineRect, 4, 4);
            }

            // 底边线
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(4, spinBoxRect.y() + spinBoxRect.height());
            path.lineTo(spinBoxRect.width() - 4, spinBoxRect.y() + spinBoxRect.height());
            path.arcTo(
                QRectF(spinBoxRect.width() - 8, spinBoxRect.y() + spinBoxRect.height() - 8, 8, 8),
                -90, 45);
            path.lineTo(4 - 2 * std::sqrt(2),
                spinBoxRect.y() + spinBoxRect.height() - (4 - 2 * std::sqrt(2)));
            path.arcTo(QRectF(0, spinBoxRect.y() + spinBoxRect.height() - 8, 8, 8), 225, 45);
            path.closeSubpath();
            painter->drawPath(path);

            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(17);
            painter->setFont(iconFont);
            painter->setPen(QtThemeColor(d->themeMode, BasicText));
            painter->drawText(addLineRect, Qt::AlignCenter,
                d->buttonMode == QtSpinBoxType::PMSide
                    ? QChar(static_cast<ushort>(QtIconType::Plus))
                    : QChar(static_cast<ushort>(QtIconType::AngleUp)));
            painter->drawText(subLineRect, Qt::AlignCenter,
                d->buttonMode == QtSpinBoxType::PMSide
                    ? QChar(static_cast<ushort>(QtIconType::Minus))
                    : QChar(static_cast<ushort>(QtIconType::AngleDown)));
            painter->restore();
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QRect QtSpinBoxStyle::subControlRect(
    ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    Q_D(const QtSpinBoxStyle);
    QRect rect = QProxyStyle::subControlRect(cc, opt, sc, widget);
    switch(cc)
    {
        case CC_SpinBox: {
            switch(sc)
            {
                case SC_ScrollBarAddLine: {
                    QRect spinBoxRect =
                        QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget)
                            .adjusted(1, 1, -1, -1);
                    switch(d->buttonMode)
                    {
                        case QtSpinBoxType::Inline: {
                            return QRect(spinBoxRect.width() - spinBoxRect.height(),
                                spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height())
                                .adjusted(3, 4, -3, -5);
                        }
                        case QtSpinBoxType::Compact: {
                            return QRect(spinBoxRect.width() - spinBoxRect.height(),
                                spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height() / 2)
                                .adjusted(3, 4, -3, 0);
                        }
                        case QtSpinBoxType::Side:
                        case QtSpinBoxType::PMSide: {
                            return QRect(spinBoxRect.width() - spinBoxRect.height(),
                                spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height())
                                .adjusted(3, 4, -3, -5);
                        }
                    }
                }
                case SC_ScrollBarSubLine: {
                    // 减少按钮
                    QRect spinBoxRect =
                        QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget)
                            .adjusted(1, 1, -1, -1);
                    switch(d->buttonMode)
                    {
                        case QtSpinBoxType::Inline: {
                            return QRect(spinBoxRect.width() - 2 * spinBoxRect.height(),
                                spinBoxRect.y(), spinBoxRect.height(), spinBoxRect.height())
                                .adjusted(6, 4, 0, -5);
                        }
                        case QtSpinBoxType::Compact: {
                            return QRect(spinBoxRect.width() - spinBoxRect.height(),
                                spinBoxRect.center().y(), spinBoxRect.height(),
                                spinBoxRect.height() / 2)
                                .adjusted(3, 0, -3, -5);
                        }
                        case QtSpinBoxType::Side:
                        case QtSpinBoxType::PMSide: {
                            return QRect(spinBoxRect.x(), spinBoxRect.y(), spinBoxRect.height(),
                                spinBoxRect.height())
                                .adjusted(3, 4, -3, -5);
                        }
                    }
                }
                case SC_SpinBoxEditField: {
                    QRect spinBoxRect =
                        QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
                    switch(d->buttonMode)
                    {
                        case QtSpinBoxType::Inline: {
                            return { spinBoxRect.x(), spinBoxRect.y(),
                                spinBoxRect.width() - 2 * spinBoxRect.height() + 6,
                                spinBoxRect.height() };
                        }
                        case QtSpinBoxType::Compact: {
                            return { spinBoxRect.x(), spinBoxRect.y(),
                                spinBoxRect.width() - spinBoxRect.height() + 3,
                                spinBoxRect.height() };
                        }
                        case QtSpinBoxType::Side:
                        case QtSpinBoxType::PMSide: {
                            return { spinBoxRect.height(), spinBoxRect.y(),
                                spinBoxRect.width() - 2 * spinBoxRect.height(),
                                spinBoxRect.height() };
                        }
                    }
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
    return rect;
}

QStyle::SubControl QtSpinBoxStyle::hitTestComplexControl(ComplexControl control,
    const QStyleOptionComplex* option, const QPoint& pt, const QWidget* widget) const
{
    if(control == CC_SpinBox)
    {
        const QStyleOptionSpinBox* sopt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if(sopt)
        {
            // Kiểm tra xem chuột pt có nằm trong vùng Nút Tăng (Up) không
            QRect upRect = subControlRect(CC_SpinBox, sopt, SC_SpinBoxUp, widget);
            if(upRect.contains(pt))
            {
                return SC_SpinBoxUp;
            }

            // Kiểm tra xem chuột pt có nằm trong vùng Nút Giảm (Down) không
            QRect downRect = subControlRect(CC_SpinBox, sopt, SC_SpinBoxDown, widget);
            if(downRect.contains(pt))
            {
                return SC_SpinBoxDown;
            }

            // Kiểm tra xem chuột pt có nằm trong khung nhập liệu không
            QRect editRect = subControlRect(CC_SpinBox, sopt, SC_SpinBoxEditField, widget);
            if(editRect.contains(pt))
            {
                return SC_SpinBoxEditField;
            }
        }
    }
    return QProxyStyle::hitTestComplexControl(control, option, pt, widget);
}
