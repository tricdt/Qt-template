#include "qtprogressbarstyle.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qobject.h>
#include <qstyleoption.h>
class QtProgressBarStylePrivate
{
    Q_DECLARE_PUBLIC(QtProgressBarStyle)
public:
    int busyStartValue {0};
    int busyEndValue {0};
    QtProgressBarStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtProgressBarStylePrivate(QtProgressBarStyle* q);
    ~QtProgressBarStylePrivate();
};

QtProgressBarStylePrivate::QtProgressBarStylePrivate(QtProgressBarStyle* q)
    : q_ptr(q)
{
    themeMode = qtTheme->getThemeMode();
    QObject::connect(qtTheme, &QtTheme::themeModeChanged, q,
        [this](QtThemeType::ThemeMode _themeMode) { themeMode = _themeMode; });
}
QtProgressBarStylePrivate::~QtProgressBarStylePrivate()
{
}

QtProgressBarStyle::QtProgressBarStyle(QStyle* parent)
    : d_ptr(new QtProgressBarStylePrivate(this))
{
}

QtProgressBarStyle::~QtProgressBarStyle()
{
}

int QtProgressBarStyle::busyStartValue() const {
    Q_D(const QtProgressBarStyle);
    return d->busyStartValue;
}
void QtProgressBarStyle::setBusyStartValue(int busyStartValue) {
    Q_D(QtProgressBarStyle);
    d->busyStartValue = busyStartValue;
}

int QtProgressBarStyle::busyEndValue() const {
    Q_D(const QtProgressBarStyle);
    return d->busyEndValue;
}
void QtProgressBarStyle::setBusyEndValue(int busyEndValue) {
    Q_D(QtProgressBarStyle);
    d->busyEndValue = busyEndValue;
}

void QtProgressBarStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtProgressBarStyle);
    switch(element)
    {
        case QStyle::CE_ProgressBarLabel: {
            if(const QStyleOptionProgressBar* popt =
                    qstyleoption_cast<const QStyleOptionProgressBar*>(option))
            {
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                painter->setPen(QtThemeColor(d->themeMode, BasicText));
                painter->drawText(popt->rect, Qt::AlignCenter, popt->text);
                painter->restore();
            }
            return;
        }
        case QStyle::CE_ProgressBarGroove: {
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, BasicChute));
            painter->drawRoundedRect(option->rect, 3, 3);
            painter->restore();
            return;
        }
        case QStyle::CE_ProgressBarContents: {
            const QStyleOptionProgressBar* popt =
                qstyleoption_cast<const QStyleOptionProgressBar*>(option);
            if(!popt)
            {
                break;
            }
            QRect contentRect = popt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
            const bool inverted = popt->invertedAppearance;
            bool reverse = popt->direction == Qt::RightToLeft;
            if(inverted)
            {
                reverse = !reverse;
            }
            if(popt->minimum == 0 && popt->maximum == 0)
            {
                int startValue = d->busyStartValue;
                if(startValue < 0)
                {
                    startValue = 0;
                }
                int endValue = d->busyEndValue;
                if(popt->state & QStyle::State_Horizontal)
                {
                    if(endValue > contentRect.width())
                    {
                        endValue = contentRect.width();
                    }
                    int busyWidth = endValue - startValue;
                    if(reverse)
                    {
                        painter->translate(
                            (contentRect.x() + contentRect.width()) / 2, contentRect.y());
                        painter->rotate(180);
                        painter->translate(-(contentRect.x() + contentRect.width()) / 2,
                            -contentRect.y() - contentRect.height() * 0.90);
                    }
                    painter->drawRoundedRect(
                        QRectF(startValue, contentRect.y(), busyWidth, contentRect.height()), 3, 3);
                }
                else
                {
                    if(endValue > contentRect.height())
                    {
                        endValue = contentRect.height();
                    }
                    int busyHeight = endValue - startValue;
                    painter->drawRoundedRect(
                        QRectF(contentRect.x(), contentRect.height() - endValue,
                            contentRect.width(), busyHeight),
                        3, 3);
                }
            }
            else
            {
                qreal ratio = popt->progress / (qreal) (popt->maximum - popt->minimum);
                if(popt->state & QStyle::State_Horizontal)
                {
                    if(reverse)
                    {
                        painter->translate(
                            (contentRect.x() + contentRect.width()) / 2, contentRect.y());
                        painter->rotate(180);
                        painter->translate(-(contentRect.x() + contentRect.width()) / 2,
                            -contentRect.y() - contentRect.height() * 0.90);
                    }
                    painter->drawRoundedRect(QRectF(contentRect.x(), contentRect.y(),
                                                 contentRect.width() * ratio, contentRect.height()),
                        3, 3);
                }
                else
                {
                    painter->drawRoundedRect(
                        QRectF(contentRect.x(),
                            contentRect.y() + contentRect.height() * (1 - ratio),
                            contentRect.width(), contentRect.height() * ratio),
                        3, 3);
                }
            }
            painter->restore();
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QRect QtProgressBarStyle::subElementRect(
    SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    switch(element)
    {
        case QStyle::SE_ProgressBarGroove:
        case QStyle::SE_ProgressBarContents: {
            QRect textRect = subElementRect(SE_ProgressBarLabel, option, widget);
            const QStyleOptionProgressBar* popt =
                qstyleoption_cast<const QStyleOptionProgressBar*>(option);
            if(!popt)
            {
                break;
            }
            QRect contentRect = popt->rect;
            int width = contentRect.width();
            int height = contentRect.height();
            if(popt->state & QStyle::State_Horizontal)
            {
                contentRect.setTop(contentRect.top() + height * 0.45);
                contentRect.setBottom(contentRect.bottom() - height * 0.30);
                if(!(popt->minimum == 0 && popt->maximum == 0))
                {
                    contentRect.setWidth(contentRect.width() - textRect.width());
                }
            }
            else
            {
                contentRect.setLeft(contentRect.left() + width * 0.375);
                contentRect.setRight(contentRect.right() - width * 0.375);
            }
            return contentRect;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::subElementRect(element, option, widget);
}
