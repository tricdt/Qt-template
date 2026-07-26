#include "qtlineeditstyle.h"
#include "qttheme.h"
#include <qpainterpath.h>
#include <qstyleoption.h>
class QtLineEditStylePrivate
{
    Q_DECLARE_PUBLIC(QtLineEditStyle)
public:
    QtLineEditStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtLineEditStylePrivate(QtLineEditStyle* q);
    ~QtLineEditStylePrivate();
};

QtLineEditStylePrivate::QtLineEditStylePrivate(QtLineEditStyle* q)
    : q_ptr(q)
{
}
QtLineEditStylePrivate::~QtLineEditStylePrivate()
{
}

QtLineEditStyle::QtLineEditStyle(QStyle* style)
    : d_ptr(new QtLineEditStylePrivate(this))
{
    Q_D(QtLineEditStyle);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtLineEditStyle::~QtLineEditStyle()
{
}

void QtLineEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtLineEditStyle);
    switch(element)
    {
        case PE_PanelLineEdit: {
            if(const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
            {
                QRect lineEditRect = fopt->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing);
                // 边框绘制
                painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
                painter->setBrush(Qt::NoBrush);
                painter->drawRoundedRect(lineEditRect.adjusted(1, 1, -1, -1), 6, 6);
                painter->setPen(Qt::NoPen);
                //  背景绘制
                if(fopt->state & QStyle::State_HasFocus)
                {
                    painter->setBrush(QtThemeColor(d->themeMode, DialogBase));
                }
                else if(fopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicHoverAlpha));
                }
                else
                {
                    painter->setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
                }
                painter->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5,
                                             lineEditRect.width() - 3, lineEditRect.height() - 3),
                    6, 6);

                // 底边线绘制
                painter->setBrush(QtThemeColor(d->themeMode, BasicHemline));
                QPainterPath path;
                path.moveTo(6, lineEditRect.height());
                path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
                path.arcTo(
                    QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
                path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
                path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 225, 45);
                path.closeSubpath();
                painter->drawPath(path);
                painter->restore();
            }
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
