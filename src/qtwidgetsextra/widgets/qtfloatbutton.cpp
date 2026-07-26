#include "qtfloatbutton.h"

class QtFloatButtonPrivate
{
    Q_DECLARE_PUBLIC(QtFloatButton)
public:
    int buttonSize;
    int margin;
    QtFloatButton* q_ptr;
    QtFloatButtonPrivate(QtFloatButton* q);
    ~QtFloatButtonPrivate();
};

QtFloatButtonPrivate::QtFloatButtonPrivate(QtFloatButton* q)
    : q_ptr(q)
{
}
QtFloatButtonPrivate::~QtFloatButtonPrivate()
{
}

QtFloatButton::QtFloatButton(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtFloatButtonPrivate(this))
{
}

QtFloatButton::~QtFloatButton()
{
}

int QtFloatButton::buttonSize() const
{
    Q_D(const QtFloatButton);
    return d->buttonSize;
}

void QtFloatButton::setButtonSize(int value)
{
    Q_D(QtFloatButton);
    if(d->buttonSize != value)
    {
        d->buttonSize = value;
        Q_EMIT buttonSizeChanged();
    }
}

int QtFloatButton::margin() const
{
    Q_D(const QtFloatButton);
    return d->margin;
}

void QtFloatButton::setMargin(int value)
{
    Q_D(QtFloatButton);
    if(d->margin != value)
    {
        d->margin = value;
        Q_EMIT marginChanged();
    }
}