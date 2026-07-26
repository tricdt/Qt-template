#include "qtspotlight.h"

class QtSpotlightPrivate
{
    Q_DECLARE_PUBLIC(QtSpotlight)
public:
int borderRadius;
    int padding;
    int overlayAlpha;
    bool isCircle;
    QString title;
    QString content;
    QtSpotlight* q_ptr;
    QtSpotlightPrivate(QtSpotlight* q);
    ~QtSpotlightPrivate();
};

QtSpotlightPrivate::QtSpotlightPrivate(QtSpotlight* q) : q_ptr(q)
{
}
QtSpotlightPrivate::~QtSpotlightPrivate()
{
}

QtSpotlight::QtSpotlight(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSpotlightPrivate(this))
{
}

QtSpotlight::~QtSpotlight()
{
}

int QtSpotlight::borderRadius() const
{
    Q_D(const QtSpotlight);
    return d->borderRadius;
}

void QtSpotlight::setBorderRadius(int value)
{
    Q_D(QtSpotlight);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtSpotlight::padding() const
{
    Q_D(const QtSpotlight);
    return d->padding;
}

void QtSpotlight::setPadding(int value)
{
    Q_D(QtSpotlight);
    if (d->padding != value) {
        d->padding = value;
        Q_EMIT paddingChanged();
    }
}

int QtSpotlight::overlayAlpha() const
{
    Q_D(const QtSpotlight);
    return d->overlayAlpha;
}

void QtSpotlight::setOverlayAlpha(int value)
{
    Q_D(QtSpotlight);
    if (d->overlayAlpha != value) {
        d->overlayAlpha = value;
        Q_EMIT overlayAlphaChanged();
    }
}

bool QtSpotlight::isCircle() const
{
    Q_D(const QtSpotlight);
    return d->isCircle;
}

void QtSpotlight::setIsCircle(bool value)
{
    Q_D(QtSpotlight);
    if (d->isCircle != value) {
        d->isCircle = value;
        Q_EMIT isCircleChanged();
    }
}

const QString& QtSpotlight::title() const
{
    Q_D(const QtSpotlight);
    return d->title;
}

void QtSpotlight::setTitle(const QString &value)
{
    Q_D(QtSpotlight);
    if (d->title != value) {
        d->title = value;
        Q_EMIT titleChanged();
    }
}

const QString& QtSpotlight::content() const
{
    Q_D(const QtSpotlight);
    return d->content;
}

void QtSpotlight::setContent(const QString &value)
{
    Q_D(QtSpotlight);
    if (d->content != value) {
        d->content = value;
        Q_EMIT contentChanged();
    }
}