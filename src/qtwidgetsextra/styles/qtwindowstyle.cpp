#include "qtwindowstyle.h"

class QtWindowStylePrivate
{
    Q_DECLARE_PUBLIC(QtWindowStyle)
public:
    QtWindowStyle* q_ptr;
    QtWindowStylePrivate(QtWindowStyle* q);
    ~QtWindowStylePrivate();
};

QtWindowStylePrivate::QtWindowStylePrivate(QtWindowStyle* q)
    : q_ptr(q)
{
}
QtWindowStylePrivate::~QtWindowStylePrivate()
{
}

QtWindowStyle::QtWindowStyle(QStyle* style)
    : d_ptr(new QtWindowStylePrivate(this))
{
}

QtWindowStyle::~QtWindowStyle()
{
}