#include "qtimpl.h"

class QtImplPrivate
{
    Q_DECLARE_PUBLIC(QtImpl)
public:
    QtImpl* q_ptr;
    QtImplPrivate(QtImpl* q);
    ~QtImplPrivate();
};

QtImplPrivate::QtImplPrivate(QtImpl* q) : q_ptr(q)
{
}
QtImplPrivate::~QtImplPrivate()
{
}

QtImpl::QtImpl(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtImplPrivate(this))
{
}

QtImpl::~QtImpl()
{
}