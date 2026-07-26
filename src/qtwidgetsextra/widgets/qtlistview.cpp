#include "qtlistview.h"
#include "styles/qtlistviewstyle.h"
#include "widgets/qtscrollbar.h"
class QtListViewPrivate
{
    Q_DECLARE_PUBLIC(QtListView)
public:
    QtListView* q_ptr;
    QtListViewStyle* listViewStyle{ nullptr };
    QtListViewPrivate(QtListView* q);
    ~QtListViewPrivate();
};

QtListViewPrivate::QtListViewPrivate(QtListView* q)
    : q_ptr(q)
{
}
QtListViewPrivate::~QtListViewPrivate()
{
}

QtListView::QtListView(QWidget* parent)
    : QListView{ parent }
    , d_ptr(new QtListViewPrivate(this))
{
    Q_D(QtListView);
    d->q_ptr = this;
    setObjectName("QtListView");
    setStyleSheet("#QtListView{background-color:transparent;}");
    d->listViewStyle = new QtListViewStyle(style());
    setStyle(d->listViewStyle);
    setMouseTracking(true);
    setVerticalScrollBar(new QtScrollBar(this));
    setHorizontalScrollBar(new QtScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

QtListView::~QtListView()
{
}

int QtListView::itemHeight() const
{
    Q_D(const QtListView);
    return d->listViewStyle->itemHeight();
}
void QtListView::setItemHeight(int value)
{
    Q_D(QtListView);
    if(value > 0)
    {
        d->listViewStyle->setItemHeight(value);
        doItemsLayout();
    }
}

bool QtListView::isTransparent() const
{
    Q_D(const QtListView);
    return d->listViewStyle->isTransparent();
}
void QtListView::setIsTransparent(bool value)
{
    Q_D(QtListView);
    d->listViewStyle->setIsTransparent(value);
    update();
}