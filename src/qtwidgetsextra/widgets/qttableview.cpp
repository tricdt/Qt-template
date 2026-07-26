#include "qttableview.h"
#include "styles/qttableviewstyle.h"
#include "widgets/qtscrollbar.h"
#include <qevent.h>
class QtTableViewPrivate
{
    Q_DECLARE_PUBLIC(QtTableView)
public:
    QtTableView* q_ptr;
    QtTableViewStyle* tableViewStyle;
    QtTableViewPrivate(QtTableView* q);
    ~QtTableViewPrivate();
};

QtTableViewPrivate::QtTableViewPrivate(QtTableView* q)
    : q_ptr(q)
{
}
QtTableViewPrivate::~QtTableViewPrivate()
{
}

QtTableView::QtTableView(QWidget* parent)
    : QTableView{ parent }
    , d_ptr(new QtTableViewPrivate(this))
{
    Q_D(QtTableView);
    setMouseTracking(true);
    setObjectName("QtTableView");
    setStyleSheet("QTableView{background-color:transparent;}"
                  "QHeaderView{background-color:transparent;border:0px;}");
    setShowGrid(false);
    setVerticalScrollBar(new QtScrollBar(this));
    setHorizontalScrollBar(new QtScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    d->tableViewStyle = new QtTableViewStyle(style());
    setStyle(d->tableViewStyle);
}

QtTableView::~QtTableView()
{
    Q_D(QtTableView);
    delete d->tableViewStyle;
}

int QtTableView::headerMargin() const
{
    Q_D(const QtTableView);
    return d->tableViewStyle->headerMargin();
}
void QtTableView::setHeaderMargin(int value)
{
    Q_D(QtTableView);
    if(value >= 0)
    {
        d->tableViewStyle->setHeaderMargin(value);
        doItemsLayout();
    }
}
void QtTableView::showEvent(QShowEvent* event)
{
    Q_D(QtTableView);
    Q_EMIT tableViewShow();
    QTableView::showEvent(event);
}
void QtTableView::hideEvent(QHideEvent* event)
{
    Q_D(QtTableView);
    Q_EMIT tableViewHide();
    QTableView::hideEvent(event);
}
void QtTableView::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtTableView);
    if(selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->tableViewStyle->setCurrentHoverRow(indexAt(event->pos()).row());
        update();
    }
    QTableView::mouseMoveEvent(event);
}
void QtTableView::leaveEvent(QEvent* event)
{
    Q_D(QtTableView);
    if(selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->tableViewStyle->setCurrentHoverRow(-1);
        update();
    }
    QTableView::leaveEvent(event);
}