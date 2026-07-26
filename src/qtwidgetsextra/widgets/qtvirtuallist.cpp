#include "qtvirtuallist.h"

class QtVirtualListPrivate
{
    Q_DECLARE_PUBLIC(QtVirtualList)
public:
int itemHeight;
    bool isTransparent;
    bool isAlternatingRowColors;
    QtVirtualList* q_ptr;
    QtVirtualListPrivate(QtVirtualList* q);
    ~QtVirtualListPrivate();
};

QtVirtualListPrivate::QtVirtualListPrivate(QtVirtualList* q) : q_ptr(q)
{
}
QtVirtualListPrivate::~QtVirtualListPrivate()
{
}

QtVirtualList::QtVirtualList(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtVirtualListPrivate(this))
{
}

QtVirtualList::~QtVirtualList()
{
}
int QtVirtualList::itemHeight() const
{
    Q_D(const QtVirtualList);
    return d->itemHeight;
}

void QtVirtualList::setItemHeight(int value)
{
    Q_D(QtVirtualList);
    if (d->itemHeight != value) {
        d->itemHeight = value;
        Q_EMIT itemHeightChanged();
    }
}

bool QtVirtualList::isTransparent() const
{
    Q_D(const QtVirtualList);
    return d->isTransparent;
}

void QtVirtualList::setIsTransparent(bool value)
{
    Q_D(QtVirtualList);
    if (d->isTransparent != value) {
        d->isTransparent = value;
        Q_EMIT isTransparentChanged();
    }
}

bool QtVirtualList::isAlternatingRowColors() const
{
    Q_D(const QtVirtualList);
    return d->isAlternatingRowColors;
}

void QtVirtualList::setIsAlternatingRowColors(bool value)
{
    Q_D(QtVirtualList);
    if (d->isAlternatingRowColors != value) {
        d->isAlternatingRowColors = value;
        Q_EMIT isAlternatingRowColorsChanged();
    }
}