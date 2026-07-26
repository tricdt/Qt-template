#include "qtqrcode.h"

class QtQRCodePrivate
{
    Q_DECLARE_PUBLIC(QtQRCode)
public:
    int borderRadius;
    int quietZone;
    QColor foregroundColor;
    QColor backgroundColor;
    QtQRCode* q_ptr;
    QtQRCodePrivate(QtQRCode* q);
    ~QtQRCodePrivate();
};

QtQRCodePrivate::QtQRCodePrivate(QtQRCode* q)
    : q_ptr(q)
{
}
QtQRCodePrivate::~QtQRCodePrivate()
{
}

QtQRCode::QtQRCode(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtQRCodePrivate(this))
{
}

QtQRCode::~QtQRCode()
{
}
int QtQRCode::borderRadius() const
{
    Q_D(const QtQRCode);
    return d->borderRadius;
}

void QtQRCode::setBorderRadius(int value)
{
    Q_D(QtQRCode);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtQRCode::quietZone() const
{
    Q_D(const QtQRCode);
    return d->quietZone;
}

void QtQRCode::setQuietZone(int value)
{
    Q_D(QtQRCode);
    if(d->quietZone != value)
    {
        d->quietZone = value;
        Q_EMIT quietZoneChanged();
    }
}

QColor QtQRCode::foregroundColor() const
{
    Q_D(const QtQRCode);
    return d->foregroundColor;
}

void QtQRCode::setForegroundColor(QColor value)
{
    Q_D(QtQRCode);
    if(d->foregroundColor != value)
    {
        d->foregroundColor = value;
        Q_EMIT foregroundColorChanged();
    }
}

QColor QtQRCode::backgroundColor() const
{
    Q_D(const QtQRCode);
    return d->backgroundColor;
}

void QtQRCode::setBackgroundColor(QColor value)
{
    Q_D(QtQRCode);
    if(d->backgroundColor != value)
    {
        d->backgroundColor = value;
        Q_EMIT backgroundColorChanged();
    }
}