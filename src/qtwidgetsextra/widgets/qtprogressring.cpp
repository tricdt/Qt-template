#include "qtprogressring.h"
#include "qttheme.h"
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>
#include <qvariant.h>
#include <qvariantanimation.h>

class QtProgressRingPrivate
{
    Q_DECLARE_PUBLIC(QtProgressRing)
public:
    bool isBusying;
    bool isTransparent;
    bool isDisplayValue;
    QtProgressRingType::ValueDisplayMode valueDisplayMode;
    int busyingWidth;
    int busyingDurationTime;
    int minimum;
    int maximum;
    int value;
    int valuePixelSize;
    int busyIndex;
    int busyStartDeg;
    int busyContentDeg;
    QtProgressRing* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QVariantAnimation* busyStartDegAnimation{ nullptr };
    QVariantAnimation* busyContentDegAnimation{ nullptr };
    QtProgressRingPrivate(QtProgressRing* q);
    ~QtProgressRingPrivate();
};

QtProgressRingPrivate::QtProgressRingPrivate(QtProgressRing* q)
    : q_ptr(q)
{
}
QtProgressRingPrivate::~QtProgressRingPrivate()
{
}

QtProgressRing::QtProgressRing(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtProgressRingPrivate(this))
{
    Q_D(QtProgressRing);
    d->isDisplayValue = true;
    d->valuePixelSize = 17;
    d->valueDisplayMode = QtProgressRingType::ValueDisplayMode::Actual;
    d->minimum = 0;
    d->maximum = 100;
    d->value = 0;
    d->busyIndex = 0xe100;
    d->busyingDurationTime = 1000;
    d->busyingWidth = 6;
    d->busyStartDeg = 0;
    d->busyContentDeg = 0;
    d->isBusying = false;
    d->isTransparent = false;
    setFixedSize(70, 70);
    setObjectName("QtProgressRing");
    setStyleSheet("#QtProgressRing{background-color:transparent;}");

    d->busyStartDegAnimation = new QVariantAnimation(this);
    connect(d->busyStartDegAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) {
            d_ptr->busyStartDeg = value.toInt();
            update();
        });
    d->busyStartDegAnimation->setEasingCurve(QEasingCurve::Linear);
    d->busyStartDegAnimation->setDuration(1000);
    d->busyStartDegAnimation->setStartValue(0);
    d->busyStartDegAnimation->setEndValue(360);
    d->busyStartDegAnimation->setLoopCount(-1);

    d->busyContentDegAnimation = new QVariantAnimation(this);
    connect(d->busyContentDegAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) { d_ptr->busyContentDeg = value.toInt(); });
    d->busyContentDegAnimation->setEasingCurve(QEasingCurve::Linear);
    d->busyContentDegAnimation->setDuration(1800);
    d->busyContentDegAnimation->setStartValue(10);
    d->busyContentDegAnimation->setKeyValueAt(0.5, 140);
    d->busyContentDegAnimation->setEndValue(10);
    d->busyContentDegAnimation->setLoopCount(-1);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtProgressRing::~QtProgressRing()
{
}

bool QtProgressRing::isBusying() const
{
    Q_D(const QtProgressRing);
    return d->isBusying;
}
void QtProgressRing::setIsBusying(bool value)
{
    Q_D(QtProgressRing);
    d->isBusying = value;
    if(d->isBusying)
    {
        d->busyStartDegAnimation->start();
        d->busyContentDegAnimation->start();
    }
    else
    {
        d->busyStartDegAnimation->stop();
        d->busyContentDegAnimation->stop();
    }
    update();
    Q_EMIT isBusyingChanged(d->isBusying);
}

bool QtProgressRing::isTransparent() const
{
    Q_D(const QtProgressRing);
    return d->isTransparent;
}
void QtProgressRing::setIsTransparent(bool value)
{
    Q_D(QtProgressRing);
    d->isTransparent = value;
}

bool QtProgressRing::isDisplayValue() const
{
    Q_D(const QtProgressRing);
    return d->isDisplayValue;
}
void QtProgressRing::setDisplayValue(bool value)
{
    Q_D(QtProgressRing);
    d->isDisplayValue = value;
}

int QtProgressRing::busyingWidth() const
{
    Q_D(const QtProgressRing);
    return d->busyingWidth;
}
void QtProgressRing::setBusyingWidth(int value)
{
    Q_D(QtProgressRing);
    d->busyingWidth = value;
    update();
    Q_EMIT busyingWidthChanged(d->busyingWidth);
}

QtProgressRingType::ValueDisplayMode QtProgressRing::valueDisplayMode() const
{
    Q_D(const QtProgressRing);
    return d->valueDisplayMode;
}
void QtProgressRing::QtProgressRing::setValueDisplayMode(QtProgressRingType::ValueDisplayMode value)
{
    Q_D(QtProgressRing);
    d->valueDisplayMode = value;
}

int QtProgressRing::busyingDurationTime() const
{
    Q_D(const QtProgressRing);
    return d->busyingDurationTime;
}
void QtProgressRing::setBusyingDurationTime(int value)
{
    Q_D(QtProgressRing);
    d->busyingDurationTime = value;
    d->busyStartDegAnimation->setDuration(d->busyingDurationTime);
    Q_EMIT busyingDurationTimeChanged(d->busyingDurationTime);
}

int QtProgressRing::minimum() const
{
    Q_D(const QtProgressRing);
    return d->minimum;
}
void QtProgressRing::setMinimum(int value)
{
    Q_D(QtProgressRing);
    d->minimum = value;
    update();
    Q_EMIT minimumChanged(d->minimum);
    Q_EMIT rangeChanged(d->minimum, d->maximum);
}

int QtProgressRing::maximum() const
{
    Q_D(const QtProgressRing);
    return d->maximum;
}
void QtProgressRing::setMaximum(int value)
{
    Q_D(QtProgressRing);
    d->maximum = value;
    update();
    Q_EMIT maximumChanged(d->maximum);
    Q_EMIT rangeChanged(d->minimum, d->maximum);
}

int QtProgressRing::value() const
{
    Q_D(const QtProgressRing);
    return d->value;
}
void QtProgressRing::setValue(int value)
{
    Q_D(QtProgressRing);
    if(value < d->minimum || value > d->maximum)
    {
        return;
    }
    d->value = value;
    update();
    Q_EMIT valueChanged(d->value);
}

int QtProgressRing::valuePixelSize() const
{
    Q_D(const QtProgressRing);
    return d->valuePixelSize;
}
void QtProgressRing::setValuePixelSize(int value)
{
    Q_D(QtProgressRing);
    d->valuePixelSize = value;
}

void QtProgressRing::setRange(int min, int max)
{
    Q_D(QtProgressRing);
    if(min < 0 || max < 0 || min > max)
    {
        return;
    }
    d->minimum = min;
    d->maximum = max;
    update();
    Q_EMIT minimumChanged(d->minimum);
    Q_EMIT maximumChanged(d->maximum);
    Q_EMIT rangeChanged(min, max);
}

void QtProgressRing::paintEvent(QPaintEvent* event)
{
    Q_D(QtProgressRing);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    if(d->isDisplayValue && !d->isBusying)
    {
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        QString valueText;
        if(d->valueDisplayMode == QtProgressRingType::ValueDisplayMode::Actual)
        {
            valueText = QString::number(d->value);
        }
        else
        {
            valueText = QString::number(d->value / (qreal) (d->maximum - d->minimum) * 100) + "%";
        }
        painter.drawText(rect(), Qt::AlignCenter | Qt::TextSingleLine, valueText);
    }
    QRect ringRect = rect().adjusted(1, 1, -1, -1);
    QPainterPath basePath;
    basePath.addEllipse(ringRect);
    QPainterPath maskPath;
    maskPath.addEllipse(
        ringRect.adjusted(d->busyingWidth, d->busyingWidth, -d->busyingWidth, -d->busyingWidth));
    painter.setClipPath(basePath.subtracted(maskPath));
    painter.setPen(Qt::NoPen);
    if(!d->isTransparent)
    {
        painter.setBrush(QtThemeColor(d->themeMode, BasicBaseDeep));
        painter.drawEllipse(ringRect);
    }
    painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
    if(d->isBusying)
    {
        painter.drawPie(ringRect, -d->busyStartDeg * 16, d->busyContentDeg * 16);
    }
    else
    {
        qreal valueDeg = d->value / (qreal) (d->maximum - d->minimum) * 360;
        painter.drawPie(ringRect, 90 * 16, -valueDeg * 16);
    }
    painter.restore();
}
