#include "qtplaintexteditstyle.h"

#include "qtdef.h"
#include "qttheme.h"
#include <qpainterpath.h>
#include <qstyleoption.h>

class QtPlainTextEditStylePrivate
{
    Q_DECLARE_PUBLIC(QtPlainTextEditStyle)
public:
    QtPlainTextEditStyle* q_ptr;
    int expandMarkWidth {0};
    QtThemeType::ThemeMode themeMode;
    QtPlainTextEditStylePrivate(QtPlainTextEditStyle* q);
    ~QtPlainTextEditStylePrivate();
};

QtPlainTextEditStylePrivate::QtPlainTextEditStylePrivate(QtPlainTextEditStyle* q)
    : q_ptr(q)
{
}
QtPlainTextEditStylePrivate::~QtPlainTextEditStylePrivate()
{
}

QtPlainTextEditStyle::QtPlainTextEditStyle(QStyle* style)
    : d_ptr(new QtPlainTextEditStylePrivate(this))
{
    Q_D(QtPlainTextEditStyle);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtPlainTextEditStyle::~QtPlainTextEditStyle()
{
}

qreal QtPlainTextEditStyle::expandMarkWidth() const
{
    Q_D(const QtPlainTextEditStyle);
    return d->expandMarkWidth;
}
void QtPlainTextEditStyle::setExpandMarkWidth(qreal expandMarkWidth)
{
    Q_D(QtPlainTextEditStyle);
    d->expandMarkWidth = expandMarkWidth;
}

void QtPlainTextEditStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtPlainTextEditStyle);
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            //背景绘制
            QRect editRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            // 边框绘制
            painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
            painter->setBrush(Qt::NoBrush);
            painter->drawRoundedRect(editRect.adjusted(1, 1, -1, -1), 6, 6);
            painter->setPen(Qt::NoPen);

            // 背景绘制
            painter->setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
            painter->drawRoundedRect(QRectF(editRect.x() + 1.5, editRect.y() + 1.5, editRect.width() - 3, editRect.height() - 3), 6, 6);

            // 底边线绘制
            painter->setBrush(QtThemeColor(d->themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(6, editRect.height());
            path.lineTo(editRect.width() - 6, editRect.height());
            path.arcTo(QRectF(editRect.width() - 12, editRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * std::sqrt(2), editRect.height() - (6 - 3 * std::sqrt(2)));
            path.arcTo(QRectF(0, editRect.height() - 12, 12, 12), 270, 45);
            path.closeSubpath();
            painter->drawPath(path);

            //焦点指示器
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
            painter->drawRoundedRect(QRectF(editRect.width() / 2 - d->expandMarkWidth, editRect.height() - 2.5, d->expandMarkWidth * 2, 2.5), 2, 2);

            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}
