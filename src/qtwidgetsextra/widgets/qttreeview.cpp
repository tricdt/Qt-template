#include "qttreeview.h"
#include "styles/qttreeviewstyle.h"
#include "widgets/qtscrollbar.h"

class QtTreeViewPrivate
{
    Q_DECLARE_PUBLIC(QtTreeView)
public:
    QtTreeView* q_ptr;
    QtTreeViewStyle* treeViewStyle;
    QtTreeViewPrivate(QtTreeView* q);
    ~QtTreeViewPrivate();
};

QtTreeViewPrivate::QtTreeViewPrivate(QtTreeView* q)
    : q_ptr(q)
{
}
QtTreeViewPrivate::~QtTreeViewPrivate()
{
}

QtTreeView::QtTreeView(QWidget* parent)
    : QTreeView{ parent }
    , d_ptr(new QtTreeViewPrivate(this))
{
    Q_D(QtTreeView);
    setObjectName("QtTreeView");
    setStyleSheet("#QtTreeView{background-color:transparent;}"
                  "QHeaderView{background-color:transparent;border:0px;}");

    setAnimated(true);
    setMouseTracking(true);

    QtScrollBar* hScrollBar = new QtScrollBar(this);
    hScrollBar->setIsAnimation(true);
    connect(hScrollBar, &QtScrollBar::rangeAnimationFinished, this, [=]() { doItemsLayout(); });
    setHorizontalScrollBar(hScrollBar);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QtScrollBar* vScrollBar = new QtScrollBar(this);
    vScrollBar->setIsAnimation(true);
    connect(vScrollBar, &QtScrollBar::rangeAnimationFinished, this, [=]() { doItemsLayout(); });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    d->treeViewStyle = new QtTreeViewStyle(style());
    setStyle(d->treeViewStyle);
}

QtTreeView::~QtTreeView()
{
    Q_D(QtTreeView);
    delete d->treeViewStyle;
}

int QtTreeView::itemHeight() const
{
    Q_D(const QtTreeView);
    return d->treeViewStyle->itemHeight();
}
void QtTreeView::setItemHeight(int value)
{
    Q_D(QtTreeView);
    d->treeViewStyle->setItemHeight(value);
    doItemsLayout();
}

int QtTreeView::headerMargin() const
{
    Q_D(const QtTreeView);
    return d->treeViewStyle->headerMargin();
}
void QtTreeView::setHeaderMargin(int value)
{
    Q_D(QtTreeView);
    d->treeViewStyle->setHeaderMargin(value);
    doItemsLayout();
}