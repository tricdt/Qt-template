#include "QtLCDNumberStyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qpainter.h>
#include <qstyleoption.h>

class QtLCDNumberStylePrivate
{
    Q_DECLARE_PUBLIC(QtLCDNumberStyle)
public:
    bool isTransparent;
    QtLCDNumberStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtLCDNumberStylePrivate(QtLCDNumberStyle* q);
    ~QtLCDNumberStylePrivate();
};

QtLCDNumberStylePrivate::QtLCDNumberStylePrivate(QtLCDNumberStyle* q)
    : q_ptr(q)
{
}
QtLCDNumberStylePrivate::~QtLCDNumberStylePrivate()
{
}

QtLCDNumberStyle::QtLCDNumberStyle(QStyle* style)
    : d_ptr(new QtLCDNumberStylePrivate(this))
{
    Q_D(QtLCDNumberStyle);
    d->isTransparent = false;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtLCDNumberStyle::~QtLCDNumberStyle()
{
}

void QtLCDNumberStyle::drawControl(QStyle::ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtLCDNumberStyle);
    switch(element)
    {
        case QStyle::CE_ShapedFrame: {
            if(const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
            {
                if(!d->isTransparent)
                {
                    QRect numberRect = option->rect;
                    painter->save();
                    painter->setRenderHints(QPainter::Antialiasing);
                    painter->setPen(QtThemeColor(d->themeMode, BasicBorder));
                    painter->setBrush(Qt::NoBrush);
                    painter->drawRoundedRect(numberRect.adjusted(1, 1, -1, -1), 6, 6);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
                    painter->drawRoundedRect(QRectF(numberRect.x() + 1.5, numberRect.y() + 1.5,
                                                 numberRect.width() - 3, numberRect.height() - 3),
                        6, 6);
                    painter->restore();
                }
            }
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

bool QtLCDNumberStyle::isTransparent() const
{
    Q_D(const QtLCDNumberStyle);
    return d->isTransparent;
}
void QtLCDNumberStyle::setIsTransparent(bool value)
{
    Q_D(QtLCDNumberStyle);
    d->isTransparent = value;
}