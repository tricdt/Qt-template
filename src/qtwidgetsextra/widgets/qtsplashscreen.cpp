#include "qtsplashscreen.h"

class QtSplashScreenPrivate
{
    Q_DECLARE_PUBLIC(QtSplashScreen)
public:
int borderRadius;
    int minimum;
    int maximum;
    int value;
    bool isShowProgressBar;
    bool isShowProgressRing;
    bool isClosable;
    QtSplashScreen* q_ptr;
    QtSplashScreenPrivate(QtSplashScreen* q);
    ~QtSplashScreenPrivate();
};

QtSplashScreenPrivate::QtSplashScreenPrivate(QtSplashScreen* q) : q_ptr(q)
{
}
QtSplashScreenPrivate::~QtSplashScreenPrivate()
{
}

QtSplashScreen::QtSplashScreen(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSplashScreenPrivate(this))
{
}

QtSplashScreen::~QtSplashScreen()
{
}

int QtSplashScreen::borderRadius() const
{
    Q_D(const QtSplashScreen);
    return d->borderRadius;
}

void QtSplashScreen::setBorderRadius(int value)
{
    Q_D(QtSplashScreen);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtSplashScreen::minimum() const
{
    Q_D(const QtSplashScreen);
    return d->minimum;
}

void QtSplashScreen::setMinimum(int value)
{
    Q_D(QtSplashScreen);
    if (d->minimum != value) {
        d->minimum = value;
        Q_EMIT minimumChanged();
    }
}

int QtSplashScreen::maximum() const
{
    Q_D(const QtSplashScreen);
    return d->maximum;
}

void QtSplashScreen::setMaximum(int value)
{
    Q_D(QtSplashScreen);
    if (d->maximum != value) {
        d->maximum = value;
        Q_EMIT maximumChanged();
    }
}

int QtSplashScreen::value() const
{
    Q_D(const QtSplashScreen);
    return d->value;
}

void QtSplashScreen::setValue(int value)
{
    Q_D(QtSplashScreen);
    if (d->value != value) {
        d->value = value;
        Q_EMIT valueChanged();
    }
}

bool QtSplashScreen::isShowProgressBar() const
{
    Q_D(const QtSplashScreen);
    return d->isShowProgressBar;
}

void QtSplashScreen::setIsShowProgressBar(bool value)
{
    Q_D(QtSplashScreen);
    if (d->isShowProgressBar != value) {
        d->isShowProgressBar = value;
        Q_EMIT isShowProgressBarChanged();
    }
}

bool QtSplashScreen::isShowProgressRing() const
{
    Q_D(const QtSplashScreen);
    return d->isShowProgressRing;
}

void QtSplashScreen::setIsShowProgressRing(bool value)
{
    Q_D(QtSplashScreen);
    if (d->isShowProgressRing != value) {
        d->isShowProgressRing = value;
        Q_EMIT isShowProgressRingChanged();
    }
}

bool QtSplashScreen::isClosable() const
{
    Q_D(const QtSplashScreen);
    return d->isClosable;
}

void QtSplashScreen::setIsClosable(bool value)
{
    Q_D(QtSplashScreen);
    if (d->isClosable != value) {
        d->isClosable = value;
        Q_EMIT isClosableChanged();
    }
}