#include "qtlcdnumber.h"
#include "qtdef.h"
#include "qttheme.h"
#include "styles/qtlcdnumberstyle.h"
#include <qdatetime.h>
#include <qtimer.h>
class QtLCDNumberPrivate
{
    Q_DECLARE_PUBLIC(QtLCDNumber)
public:
    bool isUseAutoClock;
    QString autoClockFormat;
    QtLCDNumber* q_ptr;
    QtLCDNumberStyle* lcdNumberStyle;
    QTimer* clockTimer;
    QtThemeType::ThemeMode themeMode;
    QtLCDNumberPrivate(QtLCDNumber* q);
    ~QtLCDNumberPrivate();
    void onThemeModeChanged(QtThemeType::ThemeMode themeMode);
};

QtLCDNumberPrivate::QtLCDNumberPrivate(QtLCDNumber* q)
    : q_ptr(q)
{
}
QtLCDNumberPrivate::~QtLCDNumberPrivate()
{
}

void QtLCDNumberPrivate::onThemeModeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtLCDNumber);
    this->themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, QtThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}

QtLCDNumber::QtLCDNumber(QWidget* parent)
    : QLCDNumber{ parent }
    , d_ptr(new QtLCDNumberPrivate(this))
{
    Q_D(QtLCDNumber);

    d->isUseAutoClock = false;
    d->autoClockFormat = "yyyy-MM-dd hh:mm:ss";
    setDigitCount(d->autoClockFormat.length());
    setSegmentStyle(QLCDNumber::Flat);
    setObjectName("QtLCDNumber");
    setStyleSheet("#QtLCDNumber{background-color:transparent;}");
    d->lcdNumberStyle = new QtLCDNumberStyle();
    setStyle(d->lcdNumberStyle);
    d->clockTimer = new QTimer(this);
    connect(d->clockTimer, &QTimer::timeout, this,
        [this]() { display(QDateTime::currentDateTime().toString(d_ptr->autoClockFormat)); });

    d->onThemeModeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeModeChanged(themeMode); });
}

QtLCDNumber::QtLCDNumber(uint numDigits, QWidget* parent)
    : QtLCDNumber(parent)
{
    setDigitCount(numDigits);
}

QtLCDNumber::~QtLCDNumber()
{
    Q_D(QtLCDNumber);
    delete d->lcdNumberStyle;
}

bool QtLCDNumber::isUseAutoClock() const
{
    Q_D(const QtLCDNumber);
    return d->isUseAutoClock;
}
void QtLCDNumber::setIsUseAutoClock(bool value)
{
    Q_D(QtLCDNumber);
    d->isUseAutoClock = value;
    if(d->isUseAutoClock)
    {
        setDigitCount(d->autoClockFormat.length());
        display(QDateTime::currentDateTime().toString(d->autoClockFormat));
        d->clockTimer->start(200);
    }
    else
    {
        d->clockTimer->stop();
        display("");
    }
    Q_EMIT isUseAutoClockChanged();
}
QString QtLCDNumber::autoClockFormat() const
{
    Q_D(const QtLCDNumber);
    return d->autoClockFormat;
}
void QtLCDNumber::setAutoClockFormat(QString value)
{
    Q_D(QtLCDNumber);
    d->autoClockFormat = value;
    setDigitCount(d->autoClockFormat.length());
    Q_EMIT autoClockFormatChanged();
}
bool QtLCDNumber::isTransparent() const
{
    Q_D(const QtLCDNumber);
    return d->lcdNumberStyle->isTransparent();
}
void QtLCDNumber::setIsTransparent(bool value)
{
    Q_D(QtLCDNumber);
    d->lcdNumberStyle->setIsTransparent(value);
    update();
    Q_EMIT isTransparentChanged();
}

void QtLCDNumber::paintEvent(QPaintEvent* event)
{
    Q_D(QtLCDNumber);
    if (palette().color(QPalette::WindowText) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeModeChanged(d->themeMode);
    }
    QLCDNumber::paintEvent(event);
}
