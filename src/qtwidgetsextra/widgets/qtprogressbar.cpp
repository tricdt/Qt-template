#include "qtprogressbar.h"
#include "styles/qtprogressbarstyle.h"
#include <qpropertyanimation.h>
#include <qstyleoption.h>
#include <qvariantanimation.h>

class QtProgressBarPrivate
{
    Q_DECLARE_PUBLIC(QtProgressBar)
public:
    QtProgressBar* q_ptr;
    bool isBusyAnimation{ false };
    QtProgressBarStyle* style{ nullptr };
    QVariantAnimation* busyAnimation{ nullptr };
    QtProgressBarPrivate(QtProgressBar* q);
    ~QtProgressBarPrivate();
};

QtProgressBarPrivate::QtProgressBarPrivate(QtProgressBar* q)
    : q_ptr(q)
{
}
QtProgressBarPrivate::~QtProgressBarPrivate()
{
}

QtProgressBar::QtProgressBar(QWidget* parent)
    : QProgressBar{ parent }
    , d_ptr(new QtProgressBarPrivate(this))
{
    Q_D(QtProgressBar);
    setObjectName("QtProgressBar");
    setStyleSheet("#QtProgressBar{background-color:transparent;}");
    d->q_ptr = this;
    d->style = new QtProgressBarStyle(style());
    setStyle(d->style);
    d->busyAnimation = new QVariantAnimation(d->style);
    connect(d->busyAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
        d_ptr->style->setBusyStartValue(value.toInt());
        d_ptr->style->setBusyEndValue(value.toInt() + 75);
        update();
    });
    d->busyAnimation->setDuration(2000);
    d->busyAnimation->setLoopCount(-1);
}

QtProgressBar::~QtProgressBar()
{
    Q_D(QtProgressBar);
    delete d->style;
}

void QtProgressBar::setMinimum(int minimum)
{
    Q_D(QtProgressBar);
    if(d->isBusyAnimation && !(maximum() == 0 && minimum == 0))
    {
        d->isBusyAnimation = false;
        d->busyAnimation->stop();
    }
    QProgressBar::setMinimum(minimum);
}

void QtProgressBar::setMaximum(int maximum)
{
    Q_D(QtProgressBar);
    if(d->isBusyAnimation && !(minimum() == 0 && maximum == 0))
    {
        d->isBusyAnimation = false;
        d->busyAnimation->stop();
    }
    QProgressBar::setMaximum(maximum);
}

void QtProgressBar::paintEvent(QPaintEvent* event)
{
    Q_D(QtProgressBar);
    if(!d->isBusyAnimation && minimum() == 0 && maximum() == 0)
    {
        QStyleOptionProgressBar option;
        option.initFrom(this);
        d->isBusyAnimation = true;
        d->busyAnimation->setStartValue(-75);
        if(orientation() == Qt::Horizontal)
        {
            d->busyAnimation->setEndValue(this->width());
        }
        else
        {
            d->busyAnimation->setEndValue(this->height());
        }
        d->busyAnimation->start();
    }
    QProgressBar::paintEvent(event);
}

void QtProgressBar::resizeEvent(QResizeEvent* event)
{
    Q_D(QtProgressBar);
    if(orientation() == Qt::Horizontal)
    {
        d->busyAnimation->setEndValue(this->width());
    }
    else
    {
        d->busyAnimation->setEndValue(this->height());
    }
    QProgressBar::resizeEvent(event);
}
