#include "qtwatermark.h"

class QtWaterMarkPrivate
{
    Q_DECLARE_PUBLIC(QtWaterMark)
public:
    QString text;
    int fontPixelSize;
    qreal opacity;
    qreal rotation;
    int gapX;
    int gapY;
    QColor textColor;
    QImage image;
    int imageWidth;
    int imageHeight;
    QtWaterMark* q_ptr;
    QtWaterMarkPrivate(QtWaterMark* q);
    ~QtWaterMarkPrivate();
};

QtWaterMarkPrivate::QtWaterMarkPrivate(QtWaterMark* q)
    : q_ptr(q)
{
}
QtWaterMarkPrivate::~QtWaterMarkPrivate()
{
}

QtWaterMark::QtWaterMark(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtWaterMarkPrivate(this))
{
}

QtWaterMark::~QtWaterMark()
{
}

const QString& QtWaterMark::text() const
{
    Q_D(const QtWaterMark);
    return d->text;
}

void QtWaterMark::setText(const QString& value)
{
    Q_D(QtWaterMark);
    if(d->text != value)
    {
        d->text = value;
        Q_EMIT textChanged();
    }
}

int QtWaterMark::fontPixelSize() const
{
    Q_D(const QtWaterMark);
    return d->fontPixelSize;
}

void QtWaterMark::setFontPixelSize(int value)
{
    Q_D(QtWaterMark);
    if(d->fontPixelSize != value)
    {
        d->fontPixelSize = value;
        Q_EMIT fontPixelSizeChanged();
    }
}

qreal QtWaterMark::opacity() const
{
    Q_D(const QtWaterMark);
    return d->opacity;
}

void QtWaterMark::setOpacity(qreal value)
{
    Q_D(QtWaterMark);
    if(d->opacity != value)
    {
        d->opacity = value;
        Q_EMIT opacityChanged();
    }
}

qreal QtWaterMark::rotation() const
{
    Q_D(const QtWaterMark);
    return d->rotation;
}

void QtWaterMark::setRotation(qreal value)
{
    Q_D(QtWaterMark);
    if(d->rotation != value)
    {
        d->rotation = value;
        Q_EMIT rotationChanged();
    }
}

int QtWaterMark::gapX() const
{
    Q_D(const QtWaterMark);
    return d->gapX;
}

void QtWaterMark::setGapX(int value)
{
    Q_D(QtWaterMark);
    if(d->gapX != value)
    {
        d->gapX = value;
        Q_EMIT gapXChanged();
    }
}

int QtWaterMark::gapY() const
{
    Q_D(const QtWaterMark);
    return d->gapY;
}

void QtWaterMark::setGapY(int value)
{
    Q_D(QtWaterMark);
    if(d->gapY != value)
    {
        d->gapY = value;
        Q_EMIT gapYChanged();
    }
}

const QColor& QtWaterMark::textColor() const
{
    Q_D(const QtWaterMark);
    return d->textColor;
}

void QtWaterMark::setTextColor(const QColor& value)
{
    Q_D(QtWaterMark);
    if(d->textColor != value)
    {
        d->textColor = value;
        Q_EMIT textColorChanged();
    }
}

const QImage& QtWaterMark::image() const
{
    Q_D(const QtWaterMark);
    return d->image;
}

void QtWaterMark::setImage(const QImage& value)
{
    Q_D(QtWaterMark);
    if(d->image != value)
    {
        d->image = value;
        Q_EMIT imageChanged();
    }
}

int QtWaterMark::imageWidth() const
{
    Q_D(const QtWaterMark);
    return d->imageWidth;
}

void QtWaterMark::setImageWidth(int value)
{
    Q_D(QtWaterMark);
    if(d->imageWidth != value)
    {
        d->imageWidth = value;
        Q_EMIT imageWidthChanged();
    }
}

int QtWaterMark::imageHeight() const
{
    Q_D(const QtWaterMark);
    return d->imageHeight;
}

void QtWaterMark::setImageHeight(int value)
{
    Q_D(QtWaterMark);
    if(d->imageHeight != value)
    {
        d->imageHeight = value;
        Q_EMIT imageHeightChanged();
    }
}