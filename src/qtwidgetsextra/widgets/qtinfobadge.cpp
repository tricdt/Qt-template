#include "qtinfobadge.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qcoreevent.h>
class QtInfoBadgePrivate
{
    Q_DECLARE_PUBLIC(QtInfoBadge)
public:
    QtInfoBadge* q_ptr;
    int value;
    QtIconType::IconName qtIcon;
    QtInfoBadge::BadgeMode badgeMode{ QtInfoBadge::Dot };
    QtInfoBadge::Severity severity{ QtInfoBadge::Attention };
    int maxValue{ 99 };
    QWidget* target{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QtInfoBadgePrivate(QtInfoBadge* q);
    ~QtInfoBadgePrivate();
    void updatePosition();
    QColor severityColor() const;
};

QtInfoBadgePrivate::QtInfoBadgePrivate(QtInfoBadge* q)
    : q_ptr(q)
{
}
QtInfoBadgePrivate::~QtInfoBadgePrivate()
{
}

void QtInfoBadgePrivate::updatePosition()
{
    Q_Q(QtInfoBadge);
    if(!target)
    {
        return;
    }
    // Position at inner top-right corner of target
    int x = target->width() - q->width() - 2;
    int y = 2;
    q->move(x, y);
}

QColor QtInfoBadgePrivate::severityColor() const
{
    switch(severity)
    {
        case QtInfoBadge::Attention:
            return QtThemeColor(themeMode, StatusDanger);
        case QtInfoBadge::Informational:
            return QtThemeColor(themeMode, PrimaryNormal);
        case QtInfoBadge::Success:
            return QColor(0x0F, 0x7B, 0x0F);
        case QtInfoBadge::Caution:
            return QColor(0x9D, 0x5D, 0x00);
        case QtInfoBadge::Critical:
            return QtThemeColor(themeMode, StatusDanger);
    }
    return QtThemeColor(themeMode, StatusDanger);
}

QtInfoBadge::QtInfoBadge(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtInfoBadgePrivate(this))
{
    Q_D(QtInfoBadge);
    d->value = 0;
    d->qtIcon = QtIconType::None;
    setObjectName("QtInfoBadge");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    connect(this, &QtInfoBadge::valueChanged, this, [this]() {
        updateGeometry();
        adjustSize();
        d_ptr->updatePosition();
        update();
    });
}

QtInfoBadge::QtInfoBadge(int value, QWidget* parent)
    : QtInfoBadge(parent)
{
    Q_D(QtInfoBadge);
    d->badgeMode = Value_;
    d->value = value;
}
QtInfoBadge::QtInfoBadge(QtIconType::IconName icon, QWidget* parent)
    : QtInfoBadge(parent)
{
    Q_D(QtInfoBadge);
    d->badgeMode = Icon;
    d->qtIcon = icon;
}
QtInfoBadge::~QtInfoBadge()
{
}

int QtInfoBadge::value() const
{
    Q_D(const QtInfoBadge);
    return d->value;
    ;
}
void QtInfoBadge::setValue(int value)
{
    Q_D(QtInfoBadge);
    d->value = value;
    Q_EMIT valueChanged();
}

QtIconType::IconName QtInfoBadge::qtIcon() const
{
    Q_D(const QtInfoBadge);
    return d->qtIcon;
}
void QtInfoBadge::setQtIcon(QtIconType::IconName value)
{
    Q_D(QtInfoBadge);
    d->qtIcon = value;
    Q_EMIT qtIconChanged();
}

void QtInfoBadge::setBadgeMode(BadgeMode mode)
{
    Q_D(QtInfoBadge);
    d->badgeMode = mode;
    updateGeometry();
    adjustSize();
    d->updatePosition();
    update();
}
QtInfoBadge::BadgeMode QtInfoBadge::badgeMode() const
{
    Q_D(const QtInfoBadge);
    return d->badgeMode;
}

void QtInfoBadge::setMaxValue(int maxValue)
{
    Q_D(QtInfoBadge);
    d->maxValue = maxValue;
    update();
}
int QtInfoBadge::maxValue() const
{
    Q_D(const QtInfoBadge);
    return d->maxValue;
}

void QtInfoBadge::setSeverity(Severity severity)
{
    Q_D(QtInfoBadge);
    d->severity = severity;
    update();
}
QtInfoBadge::Severity QtInfoBadge::severity() const
{
    Q_D(const QtInfoBadge);
    return d->severity;
}

void QtInfoBadge::attachTo(QWidget* target)
{
    Q_D(QtInfoBadge);
    if(d->target)
    {
        d->target->removeEventFilter(this);
    }
    d->target = target;
    if(target)
    {
        setParent(target);
        target->installEventFilter(this);
        raise();
        adjustSize();
        d->updatePosition();
        show();
    }
}

bool QtInfoBadge::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtInfoBadge);
    if(watched == d->target)
    {
        if(event->type() == QEvent::Resize || event->type() == QEvent::Move)
        {
            d->updatePosition();
            raise();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void QtInfoBadge::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtInfoBadge);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    QColor badgeColor = d->severityColor();
    painter.setBrush(badgeColor);

    // White outline for contrast
    painter.setPen(QPen(Qt::white, 2));

    switch(d->badgeMode)
    {
        case Dot: {
            QRectF dotRect(1, 1, width() - 2, height() - 2);
            painter.drawEllipse(dotRect);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(dotRect.adjusted(1, 1, -1, -1));
            break;
        }
        case Value_: {
            QString text;
            if(d->value > d->maxValue)
            {
                text = QString::number(d->maxValue) + "+";
            }
            else
            {
                text = QString::number(d->value);
            }
            QRectF badgeRect(1, 1, width() - 2, height() - 2);
            painter.drawRoundedRect(badgeRect, badgeRect.height() / 2.0, badgeRect.height() / 2.0);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(badgeRect, badgeRect.height() / 2.0, badgeRect.height() / 2.0);
            painter.setPen(Qt::white);
            QFont font = this->font();
            font.setPixelSize(10);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(rect(), Qt::AlignCenter, text);
            break;
        }
        case Icon: {
            QRectF iconRect(1, 1, width() - 2, height() - 2);
            painter.drawEllipse(iconRect);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(iconRect);
            QFont iconFont = QFont("ElaAwesome");
            iconFont.setPixelSize(height() * 0.55);
            painter.setFont(iconFont);
            painter.setPen(Qt::white);
            painter.drawText(rect(), Qt::AlignCenter, QChar((unsigned short) d->qtIcon));
            break;
        }
    }
}

QSize QtInfoBadge::sizeHint() const
{
    Q_D(const QtInfoBadge);
    switch(d->badgeMode)
    {
        case Dot:
            return QSize(10, 10);
        case Value_: {
            QString text;
            if(d->value > d->maxValue)
            {
                text = QString::number(d->maxValue) + "+";
            }
            else
            {
                text = QString::number(d->value);
            }
            QFont font = this->font();
            font.setPixelSize(10);
            font.setBold(true);
            QFontMetrics fm(font);
            int textWidth = fm.horizontalAdvance(text);
            int w = qMax(16, textWidth + 8);
            return QSize(w, 16);
        }
        case Icon:
            return QSize(16, 16);
    }
    return QSize(10, 10);
}
