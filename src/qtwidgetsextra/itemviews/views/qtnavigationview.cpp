#include "qtnavigationview.h"
#include "styles/qtnavigationstyle.h"
#include "utils/qtnavigationnode.h"
#include "widgets/qtmenu.h"
#include "widgets/qtscrollbar.h"
#include "widgets/qttooltip.h"
#include <qcoreevent.h>
#include <qevent.h>
#include <qscroller.h>
#include <qscrollerproperties.h>

class QtNavigationViewPrivate
{
    Q_DECLARE_PUBLIC(QtNavigationView)
public:
    QtNavigationBar* navigationBar;
    QtNavigationView* q_ptr;
    QtNavigationStyle* navigationStyle{ nullptr };
    QtToolTip* compactToolTip{ nullptr };
    QtNavigationViewPrivate(QtNavigationView* q);
    ~QtNavigationViewPrivate();
    void doCompactToolTip();
};

QtNavigationViewPrivate::QtNavigationViewPrivate(QtNavigationView* q)
    : q_ptr(q)
{
}
QtNavigationViewPrivate::~QtNavigationViewPrivate()
{
}

void QtNavigationViewPrivate::doCompactToolTip()
{
    Q_Q(QtNavigationView);
    if(navigationBar->currentDisplayMode() == QtNavigationType::NavigationDisplayMode::Compact)
    {
        if(!compactToolTip)
        {
            compactToolTip = new QtToolTip(q);
        }
        QModelIndex posIndex = q->indexAt(q->mapFromGlobal(QCursor::pos()));
        if(!posIndex.isValid())
        {
            compactToolTip->hide();
            return;
        }
        QtNavigationNode* posNode = static_cast<QtNavigationNode*>(posIndex.internalPointer());
        compactToolTip->setToolTip(posNode->nodeTitle());
        compactToolTip->updatePos();
        compactToolTip->show();
    }
    else
    {
        if(compactToolTip)
        {
            compactToolTip->deleteLater();
            compactToolTip = nullptr;
        }
    }
}

QtNavigationView::QtNavigationView(QWidget* parent)
    : QTreeView{ parent }
    , d_ptr(new QtNavigationViewPrivate(this))
{
    Q_D(QtNavigationView);
    setObjectName("QtNavigationView");
    setStyleSheet("#QtNavigationView{background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setExpandsOnDoubleClick(false);
    setAutoScroll(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QtScrollBar* vScrollBar = new QtScrollBar(this);
    connect(vScrollBar, &QtScrollBar::rangeAnimationFinished, this, [this]() { doItemsLayout(); });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QtScrollBar* floatVScrollBar = new QtScrollBar(vScrollBar, this);
    floatVScrollBar->setIsAnimation(true);
    floatVScrollBar->installEventFilter(this);

    d->navigationStyle = new QtNavigationStyle(this->style());
    d->navigationStyle->setNavigationView(this);
    setStyle(d->navigationStyle);

    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(this->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(
        QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(
        QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [this](QScroller::State newstate) {
        if(newstate == QScroller::Pressed)
        {
            d_ptr->navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            viewport()->update();
        }
        else if(newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            d_ptr->navigationStyle->setPressIndex(QModelIndex());
        }
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QtNavigationView::customContextMenuRequested, this,
        &QtNavigationView::onCustomContextMenuRequested);
}

QtNavigationView::~QtNavigationView()
{
}

QtNavigationBar* QtNavigationView::navigationBar() const
{
    Q_D(const QtNavigationView);
    return d->navigationBar;
}
void QtNavigationView::setNavigationBar(QtNavigationBar* value)
{
    Q_D(QtNavigationView);
    d->navigationBar = value;
}

void QtNavigationView::navigationNodeStateChange(QVariantMap data)
{
    Q_D(QtNavigationView);
    d->navigationStyle->navigationNodeStateChange(data);
}

void QtNavigationView::onCustomContextMenuRequested(const QPoint& pos)
{
    Q_D(QtNavigationView);
    if(!d->navigationBar->isAllowPageOpenInNewWindow())
    {
        return;
    }
    QModelIndex posIndex = indexAt(pos);
    if(!posIndex.isValid())
    {
        return;
    }
    QtNavigationNode* posNode = static_cast<QtNavigationNode*>(posIndex.internalPointer());
    if(!posNode->isExpanderNode() && !posNode->isCategoryNode())
    {
        QtMenu menu;
        menu.setMenuItemHeight(27);
        QAction* openAction = menu.addQtIconAction(QtIconType::ObjectGroup, "Open in New window    ");
        connect(openAction, &QAction::triggered, this,
            [this, &posNode]() { Q_EMIT navigationOpenNewWindow(posNode->nodeKey()); });
        menu.exec(mapToGlobal(pos));
    }
}

void QtNavigationView::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtNavigationView);
    d->doCompactToolTip();
    QTreeView::mouseMoveEvent(event);
}
void QtNavigationView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_D(QtNavigationView);
    d->navigationStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QTreeView::mouseDoubleClickEvent(event);
}
void QtNavigationView::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtNavigationView);
    QTreeView::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        if(index.isValid())
        {
            Q_EMIT navigationClicked(index);
        }
        d->navigationStyle->setPressIndex(QModelIndex());
    }
}
bool QtNavigationView::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtNavigationView);
    switch(event->type())
    {
        case QEvent::MouseMove:
        case QEvent::HoverMove: {
            d->doCompactToolTip();
            break;
        }
        default: {
            break;
        }
    }
    return QAbstractItemView::eventFilter(watched, event);
}