#include "qtbaselistview.h"
#include "widgets/qtscrollbar.h"
#include <qevent.h>

class QtBaseListViewPrivate
{
    Q_DECLARE_PUBLIC(QtBaseListView)
public:
    QtBaseListView* q_ptr;
    QtBaseListViewPrivate(QtBaseListView* q);
    ~QtBaseListViewPrivate();
};

QtBaseListViewPrivate::QtBaseListViewPrivate(QtBaseListView* q)
    : q_ptr(q)
{
}
QtBaseListViewPrivate::~QtBaseListViewPrivate()
{
}

QtBaseListView::QtBaseListView(QWidget* parent)
    : QListView{ parent }
    , d_ptr(new QtBaseListViewPrivate(this))
{
    setObjectName("QtBaseListView");
    setStyleSheet("QtBaseListView{background-color: transparent;border:0px;}"
                  "QtBaseListView::item{border:none;}");
    setAutoScroll(false);
    setFocusPolicy(Qt::NoFocus);
    setVerticalScrollBar(new QtScrollBar(this));
    setHorizontalScrollBar(new QtScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

QtBaseListView::~QtBaseListView()
{
}

void QtBaseListView::wheelEvent(QWheelEvent* event)
{
    QListView::wheelEvent(event);
    event->accept();
}

void QtBaseListView::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT mousePress(indexAt(event->pos()));
    QListView::mousePressEvent(event);
}

void QtBaseListView::mouseReleaseEvent(QMouseEvent* event)
{
    Q_EMIT mouseRelease(indexAt(event->pos()));
    QListView::mouseReleaseEvent(event);
}

void QtBaseListView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_EMIT mouseDoubleClick(indexAt(event->pos()));
    QListView::mouseDoubleClickEvent(event);
}
