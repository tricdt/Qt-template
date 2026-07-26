#include "qtautocomplete.h"

class QtAutoCompletePrivate
{
    Q_DECLARE_PUBLIC(QtAutoComplete)
public:
    int borderRadius;
    int maxVisibleItems;
    Qt::CaseSensitivity caseSensitivity;
    QtAutoComplete* q_ptr;
    QtAutoCompletePrivate(QtAutoComplete* q);
    ~QtAutoCompletePrivate();
};

QtAutoCompletePrivate::QtAutoCompletePrivate(QtAutoComplete* q)
    : q_ptr(q)
{
}
QtAutoCompletePrivate::~QtAutoCompletePrivate()
{
}

QtAutoComplete::QtAutoComplete(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtAutoCompletePrivate(this))
{
}

QtAutoComplete::~QtAutoComplete()
{
}
int QtAutoComplete::borderRadius() const
{
    Q_D(const QtAutoComplete);
    return d->borderRadius;
}

void QtAutoComplete::setBorderRadius(int value)
{
    Q_D(QtAutoComplete);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtAutoComplete::maxVisibleItems() const
{
    Q_D(const QtAutoComplete);
    return d->maxVisibleItems;
}

void QtAutoComplete::setMaxVisibleItems(int value)
{
    Q_D(QtAutoComplete);
    if(d->maxVisibleItems != value)
    {
        d->maxVisibleItems = value;
        Q_EMIT maxVisibleItemsChanged();
    }
}

Qt::CaseSensitivity QtAutoComplete::caseSensitivity() const
{
    Q_D(const QtAutoComplete);
    return d->caseSensitivity;
}

void QtAutoComplete::setCaseSensitivity(Qt::CaseSensitivity value)
{
    Q_D(QtAutoComplete);
    if(d->caseSensitivity != value)
    {
        d->caseSensitivity = value;
        Q_EMIT caseSensitivityChanged();
    }
}