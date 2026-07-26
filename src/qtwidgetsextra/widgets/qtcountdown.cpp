#include "qtcountdown.h"

class QtCountDownPrivate
{
    Q_DECLARE_PUBLIC(QtCountDown)
public:
int borderRadius;
    bool isShowDays;
    bool isShowHours;
    bool isShowMinutes;
    bool isShowSeconds;
    int digitWidth;
    int digitHeight;
    int digitSpacing;
    int fontPixelSize;
    QtCountDown* q_ptr;
    QtCountDownPrivate(QtCountDown* q);
    ~QtCountDownPrivate();
};

QtCountDownPrivate::QtCountDownPrivate(QtCountDown* q) : q_ptr(q)
{
}
QtCountDownPrivate::~QtCountDownPrivate()
{
}

QtCountDown::QtCountDown(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCountDownPrivate(this))
{
}

QtCountDown::~QtCountDown()
{
}

int QtCountDown::borderRadius() const
{
    Q_D(const QtCountDown);
    return d->borderRadius;
}

void QtCountDown::setBorderRadius(int value)
{
    Q_D(QtCountDown);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

bool QtCountDown::isShowDays() const
{
    Q_D(const QtCountDown);
    return d->isShowDays;
}

void QtCountDown::setIsShowDays(bool value)
{
    Q_D(QtCountDown);
    if (d->isShowDays != value) {
        d->isShowDays = value;
        Q_EMIT isShowDaysChanged();
    }
}

bool QtCountDown::isShowHours() const
{
    Q_D(const QtCountDown);
    return d->isShowHours;
}

void QtCountDown::setIsShowHours(bool value)
{
    Q_D(QtCountDown);
    if (d->isShowHours != value) {
        d->isShowHours = value;
        Q_EMIT isShowHoursChanged();
    }
}

bool QtCountDown::isShowMinutes() const
{
    Q_D(const QtCountDown);
    return d->isShowMinutes;
}

void QtCountDown::setIsShowMinutes(bool value)
{
    Q_D(QtCountDown);
    if (d->isShowMinutes != value) {
        d->isShowMinutes = value;
        Q_EMIT isShowMinutesChanged();
    }
}

bool QtCountDown::isShowSeconds() const
{
    Q_D(const QtCountDown);
    return d->isShowSeconds;
}

void QtCountDown::setIsShowSeconds(bool value)
{
    Q_D(QtCountDown);
    if (d->isShowSeconds != value) {
        d->isShowSeconds = value;
        Q_EMIT isShowSecondsChanged();
    }
}

int QtCountDown::digitWidth() const
{
    Q_D(const QtCountDown);
    return d->digitWidth;
}

void QtCountDown::setDigitWidth(int value)
{
    Q_D(QtCountDown);
    if (d->digitWidth != value) {
        d->digitWidth = value;
        Q_EMIT digitWidthChanged();
    }
}

int QtCountDown::digitHeight() const
{
    Q_D(const QtCountDown);
    return d->digitHeight;
}

void QtCountDown::setDigitHeight(int value)
{
    Q_D(QtCountDown);
    if (d->digitHeight != value) {
        d->digitHeight = value;
        Q_EMIT digitHeightChanged();
    }
}

int QtCountDown::digitSpacing() const
{
    Q_D(const QtCountDown);
    return d->digitSpacing;
}

void QtCountDown::setDigitSpacing(int value)
{
    Q_D(QtCountDown);
    if (d->digitSpacing != value) {
        d->digitSpacing = value;
        Q_EMIT digitSpacingChanged();
    }
}

int QtCountDown::fontPixelSize() const
{
    Q_D(const QtCountDown);
    return d->fontPixelSize;
}

void QtCountDown::setFontPixelSize(int value)
{
    Q_D(QtCountDown);
    if (d->fontPixelSize != value) {
        d->fontPixelSize = value;
        Q_EMIT fontPixelSizeChanged();
    }
}