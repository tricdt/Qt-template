#include "qtcaptcha.h"

class QtCaptchaPrivate
{
    Q_DECLARE_PUBLIC(QtCaptcha)
public:
int codeLength;
    int borderRadius;
    int boxSize;
    int boxSpacing;
    QtCaptcha* q_ptr;
    QtCaptchaPrivate(QtCaptcha* q);
    ~QtCaptchaPrivate();
};

QtCaptchaPrivate::QtCaptchaPrivate(QtCaptcha* q) : q_ptr(q)
{
}
QtCaptchaPrivate::~QtCaptchaPrivate()
{
}

QtCaptcha::QtCaptcha(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCaptchaPrivate(this))
{
}

QtCaptcha::~QtCaptcha()
{
}

int QtCaptcha::codeLength() const
{
    Q_D(const QtCaptcha);
    return d->codeLength;
}

void QtCaptcha::setCodeLength(int value)
{
    Q_D(QtCaptcha);
    if (d->codeLength != value) {
        d->codeLength = value;
        Q_EMIT codeLengthChanged();
    }
}

int QtCaptcha::borderRadius() const
{
    Q_D(const QtCaptcha);
    return d->borderRadius;
}

void QtCaptcha::setBorderRadius(int value)
{
    Q_D(QtCaptcha);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtCaptcha::boxSize() const
{
    Q_D(const QtCaptcha);
    return d->boxSize;
}

void QtCaptcha::setBoxSize(int value)
{
    Q_D(QtCaptcha);
    if (d->boxSize != value) {
        d->boxSize = value;
        Q_EMIT boxSizeChanged();
    }
}

int QtCaptcha::boxSpacing() const
{
    Q_D(const QtCaptcha);
    return d->boxSpacing;
}

void QtCaptcha::setBoxSpacing(int value)
{
    Q_D(QtCaptcha);
    if (d->boxSpacing != value) {
        d->boxSpacing = value;
        Q_EMIT boxSpacingChanged();
    }
}