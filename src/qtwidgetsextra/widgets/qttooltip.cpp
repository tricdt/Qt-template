#include "qttooltip.h"

class QtToolTipPrivate
{
    Q_DECLARE_PUBLIC(QtToolTip)
public:
    int borderRadius;
    int displayMsec;
    int showDelayMsec;
    int hideDelayMsec;
    QString toolTip;
    QWidget* customWidget;
    QtToolTip* q_ptr;
    QtToolTipPrivate(QtToolTip* q);
    ~QtToolTipPrivate();
};

QtToolTipPrivate::QtToolTipPrivate(QtToolTip* q)
    : q_ptr(q)
{
}
QtToolTipPrivate::~QtToolTipPrivate()
{
}

QtToolTip::QtToolTip(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtToolTipPrivate(this))
{
}

QtToolTip::~QtToolTip()
{
    Q_D(QtToolTip);
}

void QtToolTip::updatePos()
{
}

int QtToolTip::borderRadius() const
{
    Q_D(const QtToolTip);
    return d->borderRadius;
}
void QtToolTip::setBorderRadius(int value)
{
    Q_D(QtToolTip);
    d->borderRadius = value;
}

int QtToolTip::displayMsec() const
{
    Q_D(const QtToolTip);
    return d->displayMsec;
}
void QtToolTip::setDisplayMsec(int value)
{
    Q_D(QtToolTip);
    d->displayMsec = value;
}

int QtToolTip::showDelayMsec() const
{
    Q_D(const QtToolTip);
    return d->showDelayMsec;
}
void QtToolTip::setShowDelayMsec(int value)
{
    Q_D(QtToolTip);
    d->showDelayMsec = value;
}

int QtToolTip::hideDelayMsec() const
{
    Q_D(const QtToolTip);
    return d->hideDelayMsec;
}
void QtToolTip::setHideDelayMsec(int value)
{
    Q_D(QtToolTip);
    d->hideDelayMsec = value;
}

QString QtToolTip::toolTip() const
{
    Q_D(const QtToolTip);
    return d->toolTip;
}
void QtToolTip::setToolTip(const QString& value)
{
    Q_D(QtToolTip);
    d->toolTip = value;
}

QWidget* QtToolTip::customWidget() const
{
    Q_D(const QtToolTip);
    return d->customWidget;
}
void QtToolTip::setCustomWidget(QWidget* value)
{
    Q_D(QtToolTip);
    d->customWidget = value;
}

void QtToolTip::paintEvent(QPaintEvent* event)
{
    Q_D(QtToolTip);
}