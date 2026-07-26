#include "qtthemeanimationwidget.h"
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>

class QtThemeAnimationWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtThemeAnimationWidget)
public:
    qreal radius;
    qreal endRadius;
    QPoint center;
    QImage oldWindowBackground;
    QtThemeAnimationWidget* q_ptr;
    QtThemeAnimationWidgetPrivate(QtThemeAnimationWidget* q);
    ~QtThemeAnimationWidgetPrivate();
};

QtThemeAnimationWidgetPrivate::QtThemeAnimationWidgetPrivate(QtThemeAnimationWidget* q)
    : q_ptr(q)
{
}
QtThemeAnimationWidgetPrivate::~QtThemeAnimationWidgetPrivate()
{
}

QtThemeAnimationWidget::QtThemeAnimationWidget(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtThemeAnimationWidgetPrivate(this))
{
}

QtThemeAnimationWidget::~QtThemeAnimationWidget()
{
}

void QtThemeAnimationWidget::startAnimation(int msec)
{
    Q_D(QtThemeAnimationWidget);
    QPropertyAnimation* themeChangeAnimation = new QPropertyAnimation(this, "radius");
    themeChangeAnimation->setDuration(msec);
    themeChangeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(themeChangeAnimation, &QPropertyAnimation::finished, this, [this]() {
        Q_EMIT animationFinished();
        this->deleteLater();
    });
    connect(themeChangeAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) { update(); });
    themeChangeAnimation->setStartValue(0);
    themeChangeAnimation->setEndValue(d->endRadius);
    themeChangeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    show();
}

qreal QtThemeAnimationWidget::radius() const
{
    Q_D(const QtThemeAnimationWidget);
    return d->radius;
}
void QtThemeAnimationWidget::setRadius(qreal value)
{
    Q_D(QtThemeAnimationWidget);
    d->radius = value;
}

qreal QtThemeAnimationWidget::endRadius() const
{
    Q_D(const QtThemeAnimationWidget);
    return d->endRadius;
}
void QtThemeAnimationWidget::setEndRadius(qreal value)
{
    Q_D(QtThemeAnimationWidget);
    d->endRadius = value;
}

QPoint QtThemeAnimationWidget::center() const
{
    Q_D(const QtThemeAnimationWidget);
    return d->center;
}
void QtThemeAnimationWidget::setCenter(QPoint value)
{
    Q_D(QtThemeAnimationWidget);
    d->center = value;
}

QImage QtThemeAnimationWidget::oldWindowBackground() const
{
    Q_D(const QtThemeAnimationWidget);
    return d->oldWindowBackground;
}
void QtThemeAnimationWidget::setOldWindowBackground(QImage value)
{
    Q_D(QtThemeAnimationWidget);
    d->oldWindowBackground = value;
}

void QtThemeAnimationWidget::paintEvent(QPaintEvent* event)
{
    Q_D(QtThemeAnimationWidget);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    QImage animationImage(d->oldWindowBackground.size(), QImage::Format_ARGB32);
    animationImage.fill(Qt::transparent);
    QPainter animationImagePainter(&animationImage);
    animationImagePainter.setRenderHints(QPainter::Antialiasing);
    animationImagePainter.drawImage(d->oldWindowBackground.rect(), d->oldWindowBackground);
    animationImagePainter.setCompositionMode(QPainter::CompositionMode::CompositionMode_SourceOut);
    qreal devicePixelRatioF = d->oldWindowBackground.devicePixelRatioF();
    QPainterPath clipPath;
    clipPath.moveTo(d->center.x() * devicePixelRatioF, d->center.y() * devicePixelRatioF);
    clipPath.addEllipse(
        QPointF(d->center.x() * devicePixelRatioF, d->center.y() * devicePixelRatioF),
        d->radius * devicePixelRatioF, d->radius * devicePixelRatioF);
    animationImagePainter.setClipPath(clipPath);
    animationImagePainter.drawImage(animationImage.rect(), animationImage);
    animationImagePainter.end();

    painter.drawImage(rect(), animationImage);
    painter.restore();
}
