#include "qtselectorbar.h"

class QtSelectorBarPrivate
{
    Q_DECLARE_PUBLIC(QtSelectorBar)
public:
int currentIndex;
    int borderRadius;
    QtSelectorBar* q_ptr;
    QtSelectorBarPrivate(QtSelectorBar* q);
    ~QtSelectorBarPrivate();
};

QtSelectorBarPrivate::QtSelectorBarPrivate(QtSelectorBar* q) : q_ptr(q)
{
}
QtSelectorBarPrivate::~QtSelectorBarPrivate()
{
}

QtSelectorBar::QtSelectorBar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSelectorBarPrivate(this))
{
}

QtSelectorBar::~QtSelectorBar()
{
}

int QtSelectorBar::currentIndex() const
{
    Q_D(const QtSelectorBar);
    return d->currentIndex;
}

void QtSelectorBar::setCurrentIndex(int value)
{
    Q_D(QtSelectorBar);
    if (d->currentIndex != value) {
        d->currentIndex = value;
        Q_EMIT currentIndexChanged();
    }
}

int QtSelectorBar::borderRadius() const
{
    Q_D(const QtSelectorBar);
    return d->borderRadius;
}

void QtSelectorBar::setBorderRadius(int value)
{
    Q_D(QtSelectorBar);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}