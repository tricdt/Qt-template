#include "qtcomboboxview.h"
#include <qevent.h>

class QtComboBoxViewPrivate
{
    Q_DECLARE_PUBLIC(QtComboBoxView)
public:
    QtComboBoxView* q_ptr;
    QtComboBoxViewPrivate(QtComboBoxView* q);
    ~QtComboBoxViewPrivate();
};

QtComboBoxViewPrivate::QtComboBoxViewPrivate(QtComboBoxView* q)
    : q_ptr(q)
{
}
QtComboBoxViewPrivate::~QtComboBoxViewPrivate()
{
}

QtComboBoxView::QtComboBoxView(QWidget* parent)
    : QListView{ parent }
    , d_ptr(new QtComboBoxViewPrivate(this))
{
}

QtComboBoxView::~QtComboBoxView()
{
}

void QtComboBoxView::mousePressEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    if(index.isValid())
    {
        Q_EMIT itemPressed(index);
    }
    event->ignore();
}