#include "qtsplitter.h"

class QtSplitterPrivate
{
    Q_DECLARE_PUBLIC(QtSplitter)
public:
    int handleWidth;
    int gripLength;
    QtSplitter* q_ptr;
    QtSplitterPrivate(QtSplitter* q);
    ~QtSplitterPrivate();
};

QtSplitterPrivate::QtSplitterPrivate(QtSplitter* q)
    : q_ptr(q)
{
}
QtSplitterPrivate::~QtSplitterPrivate()
{
}

QtSplitter::QtSplitter(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSplitterPrivate(this))
{
}

QtSplitter::~QtSplitter()
{
}

int QtSplitter::handleWidth() const
{
    Q_D(const QtSplitter);
    return d->handleWidth;
}

void QtSplitter::setHandleWidth(int value)
{
    Q_D(QtSplitter);
    if(d->handleWidth != value)
    {
        d->handleWidth = value;
        Q_EMIT handleWidthChanged();
    }
}

int QtSplitter::gripLength() const
{
    Q_D(const QtSplitter);
    return d->gripLength;
}

void QtSplitter::setGripLength(int value)
{
    Q_D(QtSplitter);
    if(d->gripLength != value)
    {
        d->gripLength = value;
        Q_EMIT gripLengthChanged();
    }
}