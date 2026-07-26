#include "qtmaskwidget.h"
#include <qpainter.h>
#include <qpropertyanimation.h>

class QtMaskWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtMaskWidget)
public:
    int maskAlpha;
    QtMaskWidget* q_ptr;
    QtMaskWidgetPrivate(QtMaskWidget* q);
    ~QtMaskWidgetPrivate();
};

QtMaskWidgetPrivate::QtMaskWidgetPrivate(QtMaskWidget* q)
    : q_ptr(q)
{
}
QtMaskWidgetPrivate::~QtMaskWidgetPrivate()
{
}

QtMaskWidget::QtMaskWidget(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtMaskWidgetPrivate(this))
{
    Q_D(QtMaskWidget);
    setObjectName("MaskWidget");
    setStyleSheet("#MaskWidget{background-color:transparent;}");
    d->maskAlpha = 0;
}

QtMaskWidget::~QtMaskWidget()
{
}

int QtMaskWidget::maskAlpha() const
{
    Q_D(const QtMaskWidget);
    return d->maskAlpha;
}
void QtMaskWidget::setMaskAlpha(int value)
{
    Q_D(QtMaskWidget);
    d->maskAlpha = value;
}

void QtMaskWidget::doMaskAnimation(int endValue)
{
    Q_D(QtMaskWidget);
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "maskAlpha");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) { update(); });
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [this, endValue]() {
        if(endValue == 0)
        {
            setVisible(false);
        }
    });
    opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(d->maskAlpha);
    opacityAnimation->setEndValue(endValue);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtMaskWidget::paintEvent(QPaintEvent* event)
{
    Q_D(QtMaskWidget);
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(0x00, 0x00, 0x00, d->maskAlpha));
    painter.drawRect(rect());
    painter.restore();
}
