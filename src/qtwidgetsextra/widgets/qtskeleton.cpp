#include "qtskeleton.h"
#include "qttheme.h"
#include <qobject.h>
#include <qpainterpath.h>
#include <qvariantanimation.h>
class QtSkeletonPrivate
{
    Q_DECLARE_PUBLIC(QtSkeleton)
public:
    int borderRadius;
    bool isAnimated;
    QtSkeleton* q_ptr;
    QtSkeleton::SkeletonType skeletonType{ QtSkeleton::Text };
    QtThemeType::ThemeMode themeMode;
    QVariantAnimation* shimmerAnimation{ nullptr };
    qreal shimmerPosition;
    QtSkeletonPrivate(QtSkeleton* q);
    ~QtSkeletonPrivate();
    void startShimmerAnimation();
    void stopShimmerAnimation();
};

QtSkeletonPrivate::QtSkeletonPrivate(QtSkeleton* q)
    : q_ptr(q)
{
}
QtSkeletonPrivate::~QtSkeletonPrivate()
{
}

void QtSkeletonPrivate::startShimmerAnimation()
{
    Q_Q(QtSkeleton);
    if(shimmerAnimation)
    {
        shimmerAnimation->stop();
        delete shimmerAnimation;
        shimmerAnimation = nullptr;
    }
    shimmerAnimation = new QVariantAnimation(q);
    shimmerAnimation->setStartValue(0.0);
    shimmerAnimation->setEndValue(1.0);
    shimmerAnimation->setDuration(1500);
    shimmerAnimation->setLoopCount(-1);
    QObject::connect(
        shimmerAnimation, &QVariantAnimation::valueChanged, q, [this](const QVariant& value) {
            shimmerPosition = value.toReal();
            q_ptr->update();
        });
    shimmerAnimation->start();
}

void QtSkeletonPrivate::stopShimmerAnimation()
{
    if(shimmerAnimation)
    {
        shimmerAnimation->stop();
        delete shimmerAnimation;
        shimmerAnimation = nullptr;
    }
    shimmerPosition = 0.0;
}

QtSkeleton::QtSkeleton(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSkeletonPrivate(this))
{
    Q_D(QtSkeleton);
    d->borderRadius = 4;
    d->isAnimated = true;
    d->shimmerPosition = 0.0;
    setObjectName("QtSkeleton");
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    d->startShimmerAnimation();
}

QtSkeleton::~QtSkeleton()
{
    Q_D(QtSkeleton);
    d->stopShimmerAnimation();
}

void QtSkeleton::setSkeletonType(SkeletonType type)
{
    Q_D(QtSkeleton);
    d->skeletonType = type;
    if(type == Circle)
    {
        int diameter = width();
        setFixedSize(diameter, diameter);
    }
    update();
}

QtSkeleton::SkeletonType QtSkeleton::skeletonType() const
{
    return d_ptr->skeletonType;
}

void QtSkeleton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtSkeleton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QColor baseColor = QtThemeColor(d->themeMode, BasicDisable);

    if(d->isAnimated)
    {
        QLinearGradient gradient(0, 0, width(), 0);
        QColor lighterColor = baseColor.lighter(120);
        qreal pos = d->shimmerPosition;
        qreal shimmerWidth = 0.3;
        qreal start = pos - shimmerWidth;
        qreal end = pos + shimmerWidth;

        if(start < 0.0)
        {
            gradient.setColorAt(0.0, lighterColor);
            gradient.setColorAt(qMin(end, 1.0), baseColor);
            if(end < 1.0)
            {
                gradient.setColorAt(1.0, baseColor);
            }
        }
        else if(end > 1.0)
        {
            gradient.setColorAt(0.0, baseColor);
            gradient.setColorAt(start, baseColor);
            gradient.setColorAt(1.0, lighterColor);
        }
        else
        {
            gradient.setColorAt(0.0, baseColor);
            gradient.setColorAt(start, baseColor);
            gradient.setColorAt(pos, lighterColor);
            gradient.setColorAt(end, baseColor);
            gradient.setColorAt(1.0, baseColor);
        }
        painter.setBrush(gradient);
    }
    else
    {
        painter.setBrush(baseColor);
    }

    switch(d->skeletonType)
    {
        case Text: {
            QPainterPath path;
            path.addRoundedRect(QRectF(0, 0, width(), height()), 4, 4);
            painter.drawPath(path);
            break;
        }
        case Circle: {
            painter.drawEllipse(QRectF(0, 0, width(), height()));
            break;
        }
        case Rectangle: {
            QPainterPath path;
            path.addRoundedRect(
                QRectF(0, 0, width(), height()), d->borderRadius, d->borderRadius);
            painter.drawPath(path);
            break;
        }
    }
}

QSize QtSkeleton::sizeHint() const
{
    Q_D(const QtSkeleton);
    switch(d->skeletonType)
    {
        case Text:
            return QSize(200, 16);
        case Circle:
            return QSize(40, 40);
        case Rectangle:
            return QSize(200, 100);
    }
    return QSize(200, 16);
}

int QtSkeleton::borderRadius() const
{
    Q_D(const QtSkeleton);
    return d->borderRadius;
}

void QtSkeleton::setBorderRadius(int value)
{
    Q_D(QtSkeleton);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

bool QtSkeleton::isAnimated() const
{
    Q_D(const QtSkeleton);
    return d->isAnimated;
}

void QtSkeleton::setIsAnimated(bool value)
{
    Q_D(QtSkeleton);
    if(d->isAnimated != value)
    {
        d->isAnimated = value;
        Q_EMIT isAnimatedChanged();
    }
}