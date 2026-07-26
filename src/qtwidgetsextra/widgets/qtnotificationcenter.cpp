#include "qtnotificationcenter.h"

class QtNotificationCenterPrivate
{
    Q_DECLARE_PUBLIC(QtNotificationCenter)
public:
int borderRadius;
    int panelWidth;
    QtNotificationCenter* q_ptr;
    QtNotificationCenterPrivate(QtNotificationCenter* q);
    ~QtNotificationCenterPrivate();
};

QtNotificationCenterPrivate::QtNotificationCenterPrivate(QtNotificationCenter* q) : q_ptr(q)
{
}
QtNotificationCenterPrivate::~QtNotificationCenterPrivate()
{
}

QtNotificationCenter::QtNotificationCenter(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtNotificationCenterPrivate(this))
{
}

QtNotificationCenter::~QtNotificationCenter()
{
}

int QtNotificationCenter::borderRadius() const
{
    Q_D(const QtNotificationCenter);
    return d->borderRadius;
}

void QtNotificationCenter::setBorderRadius(int value)
{
    Q_D(QtNotificationCenter);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtNotificationCenter::panelWidth() const
{
    Q_D(const QtNotificationCenter);
    return d->panelWidth;
}

void QtNotificationCenter::setPanelWidth(int value)
{
    Q_D(QtNotificationCenter);
    if (d->panelWidth != value) {
        d->panelWidth = value;
        Q_EMIT panelWidthChanged();
    }
}