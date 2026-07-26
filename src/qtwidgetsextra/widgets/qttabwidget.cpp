#include "QtTabWidget.h"
#include "qdrag.h"
#include "qtcustomtabwidget.h"
#include "qtimer.h"
#include "qttabbar.h"
#include "qttabwidget_p.h"
#include <qapplication.h>
#include <qobject.h>

QtDragMonitor::QtDragMonitor(QObject* parent)
{
    _isInDrag = false;
}
QtDragMonitor::~QtDragMonitor()
{
}

bool QtDragMonitor::isInDrag() const
{
    return _isInDrag;
}
void QtDragMonitor::setIsInDrag(bool value)
{
    _isInDrag = value;
}

QtTabWidgetPrivate::QtTabWidgetPrivate(QtTabWidget* q)
    : q_ptr(q)
{
}
QtTabWidgetPrivate::~QtTabWidgetPrivate()
{
}

void QtTabWidgetPrivate::onTabDragCreate(QMimeData* mimeData)
{
    Q_Q(QtTabWidget);
    if(QtDragMonitor::getInstance()->isInDrag())
    {
        return;
    }
    QtDragMonitor::getInstance()->setIsInDrag(true);
    mimeData->setProperty("QtTabWidgetObject", QVariant::fromValue(q));
    int index = q->currentIndex();
    QString tabText = q->tabText(index);
    QIcon tabIcon = q->tabIcon(index);
    QWidget* dragWidget = q->widget(index);
    dragWidget->setProperty("TabIcon", q->tabIcon(index));
    dragWidget->setProperty("TabText", q->tabText(index));
    QWidget* originTabWidget = dragWidget->property("QtOriginTabWidget").value<QtTabWidget*>();
    if(!originTabWidget && !dragWidget->property("IsMetaWidget").toBool())
    {
        dragWidget->setProperty("QtOriginTabWidget", QVariant::fromValue<QtTabWidget*>(q));
        originTabWidget = q;
    }
    mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
    QtTabBar* tabBarObject = mimeData->property("QtTabBarObject").value<QtTabBar*>();
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1),
        Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(tabBarObject, &releaseEvent);
    if(!originTabWidget)
    {
        originTabWidget = dragWidget->property("QtFloatParentWidget").value<QWidget*>();
    }
    bool isFloatWidget = mimeData->property("IsFloatWidget").toBool();
    QSize tabSize = mimeData->property("TabSize").toSize();
    QtCustomTabWidget* floatWidget = nullptr;
    if(isFloatWidget)
    {
        floatWidget = dynamic_cast<QtCustomTabWidget*>(q->window());
        if(floatWidget->windowHandle())
        {
            floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, true);
        }
    }
    else
    {
        floatWidget = new QtCustomTabWidget(originTabWidget);
        q->removeTab(index);
        QtTabBar* originCustomTabBar = tabBarObject;
        if(originCustomTabBar && originCustomTabBar->objectName() == "QtCustomTabBar")
        {
            originCustomTabBar->removeTab(index);
        }
        QtTabBar* customTabBar = floatWidget->customTabBar();
        customTabBar->setTabSize(tabSize);
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<QtTabBar*>(customTabBar));
        floatWidget->addTab(dragWidget, tabIcon, tabText);
    }
    QPoint dragPos = mimeData->property("DragPos").toPoint();
    QTimer* dragTimer = new QTimer(q);
    dragTimer->start(10);
    QObject::connect(dragTimer, &QTimer::timeout, floatWidget, [=]() {
        if(floatWidget->isFinished() && !isFloatWidget)
        {
            dragTimer->stop();
        }
        else
        {
            QPoint cursorPoint = QCursor::pos();
            if(isFloatWidget)
            {
                floatWidget->move(
                    cursorPoint.x() - dragPos.x() - 10, cursorPoint.y() - dragPos.y() - 10);
            }
            else
            {
                floatWidget->move(cursorPoint.x() - tabSize.width() / 2 - 10,
                    cursorPoint.y() - tabSize.height() / 2 - 10);
            }
        }
    });
    QDrag* drag = new QDrag(q);
    QPixmap pix(1, 1);
    pix.fill(Qt::transparent);
    drag->setPixmap(pix);
    drag->setMimeData(mimeData);
    QObject::connect(drag, &QDrag::destroyed, q, [dragTimer]() { dragTimer->deleteLater(); });
    drag->setHotSpot(QPoint(tabSize.width() / 2, 0));
    QTimer::singleShot(1, q, [floatWidget, isFloatWidget]() {
        floatWidget->show();
        if(floatWidget->windowHandle())
        {
            floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, true);
        }
        if(!isFloatWidget)
        {
            floatWidget->resize(700, 500);
        }
    });
    auto ret = drag->exec();
    QtDragMonitor::getInstance()->setIsInDrag(false);
    QtCustomTabWidget* tempFloatWidget =
        mimeData->property("TempFloatWidget").value<QtCustomTabWidget*>();
    if(tempFloatWidget)
    {
        if(ret == Qt::IgnoreAction)
        {
            if(tempFloatWidget->windowHandle())
            {
                tempFloatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, false);
            }
        }
        else
        {
            tempFloatWidget->deleteLater();
        }
        floatWidget->deleteLater();
    }
    else
    {
        if(floatWidget->windowHandle())
        {
            floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, false);
        }
    }
}
void QtTabWidgetPrivate::onTabDragEnter(QMimeData* mimeData)
{
    Q_Q(QtTabWidget);
    mimeData->setProperty(
        "QtTabBarObject", QVariant::fromValue<QtTabBar*>(dynamic_cast<QtTabBar*>(q->tabBar())));
    onTabDragDrop(mimeData);
}
void QtTabWidgetPrivate::onTabDragLeave(QMimeData* mimeData)
{
    Q_Q(QtTabWidget);
    QWidget* dragWidget = mimeData->property("DragWidget").value<QWidget*>();
    int index = q->indexOf(dragWidget);
    mimeData->setProperty("QtTabWidgetObject", QVariant::fromValue(q));
    QString tabText = q->tabText(index);
    QIcon tabIcon = q->tabIcon(index);

    QtTabBar* tabBarObject = mimeData->property("QtTabBarObject").value<QtTabBar*>();
    mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
    q->removeTab(index);
    if(customTabBar && customTabBar != tabBarObject)
    {
        customTabBar->removeTab(index);
    }
    QWidget* originTabWidget = dragWidget->property("QtOriginTabWidget").value<QtTabWidget*>();
    if(!originTabWidget)
    {
        originTabWidget = dragWidget->property("QtFloatParentWidget").value<QWidget*>();
    }
    QtCustomTabWidget* floatWidget = new QtCustomTabWidget(originTabWidget);
    QSize tabSize = mimeData->property("TabSize").toSize();
    QtTabBar* customTabBar = floatWidget->customTabBar();
    customTabBar->setTabSize(tabSize);
    dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<QtTabBar*>(customTabBar));
    floatWidget->addTab(dragWidget, tabIcon, tabText);
    floatWidget->show();
    floatWidget->resize(700, 500);
    if(floatWidget->windowHandle())
    {
        floatWidget->windowHandle()->setFlag(Qt::WindowTransparentForInput, true);
    }
    QPoint cursorPoint = QCursor::pos();
    floatWidget->move(
        cursorPoint.x() - tabSize.width() / 2, cursorPoint.y() - tabSize.height() / 2);
    QTimer* dragTimer = new QTimer(q);
    dragTimer->start(10);
    QObject::connect(dragTimer, &QTimer::timeout, floatWidget, [floatWidget, tabSize]() {
        QPoint cursorPoint = QCursor::pos();
        floatWidget->move(cursorPoint.x() - tabSize.width() / 2 - 10,
            cursorPoint.y() - tabSize.height() / 2 - 10);
    });
    QObject::connect(
        mimeData, &QMimeData::destroyed, q, [dragTimer]() { dragTimer->deleteLater(); });
    QtCustomTabWidget* tempFloatWidget =
        mimeData->property("TempFloatWidget").value<QtCustomTabWidget*>();
    if(tempFloatWidget)
    {
        tempFloatWidget->deleteLater();
    }
    mimeData->setProperty("TempFloatWidget", QVariant::fromValue<QtCustomTabWidget*>(floatWidget));
}
void QtTabWidgetPrivate::onTabDragDrop(QMimeData* mimeData)
{
    Q_Q(QtTabWidget);
    QWidget* dragWidget = mimeData->property("DragWidget").value<QWidget*>();
    QString tabText = dragWidget->property("TabText").toString();
    QIcon tabIcon = dragWidget->property("TabIcon").value<QIcon>();
    int dropIndex = mimeData->property("TabDropIndex").toInt();
    if(dropIndex < 0)
    {
        dropIndex = q->count();
    }
    q->insertTab(dropIndex, dragWidget, tabIcon, tabText);
    q->setCurrentWidget(dragWidget);
    if(customTabBar)
    {
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<QtTabBar*>(customTabBar));
        customTabBar->insertTab(dropIndex, tabIcon, tabText);
        customTabBar->setCurrentIndex(dropIndex);
    }
}
void QtTabWidgetPrivate::onTabCloseRequested(int index)
{
    Q_Q(QtTabWidget);
    QWidget* closeWidget = q->widget(index);
    QtTabWidget* originTabWidget = closeWidget->property("QtOriginTabWidget").value<QtTabWidget*>();
    if(originTabWidget && originTabWidget != q)
    {
        QtTabBar* customTabBar = closeWidget->property("CurrentCustomBar").value<QtTabBar*>();
        if(customTabBar)
        {
            customTabBar->removeTab(index);
            closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<QtTabBar*>(nullptr));
        }
        originTabWidget->addTab(closeWidget, q->tabIcon(index), q->tabText(index));
        originTabWidget->setCurrentWidget(closeWidget);
    }
    else
    {
        if(!originTabWidget && q->objectName() == "QtCustomTabWidget")
        {
            customTabBar->removeTab(index);
        }
        q->removeTab(index);
        if(allTabWidgetList.contains(closeWidget))
        {
            allTabWidgetList.removeOne(closeWidget);
        }
        closeWidget->deleteLater();
    }
}



void QtTabWidgetPrivate::clearAllTabWidgetList()
{
    Q_Q(QtTabWidget);
    for(auto widget : allTabWidgetList)
    {
        auto originTabWidgetVariant = widget->property("QtOriginTabWidget");
        if(originTabWidgetVariant.isValid() && originTabWidgetVariant.value<QtTabWidget*>() == q)
        {
            widget->setProperty("QtOriginTabWidget", QVariant());
        }
    }
    allTabWidgetList.clear();
}

QtTabWidget::QtTabWidget(QWidget* parent)
    : QTabWidget{ parent }
    , d_ptr(new QtTabWidgetPrivate(this))
{
    Q_D(QtTabWidget);
    d->isContainerAcceptDrops = false;
    d->isTabTransparent = false;
    setObjectName("QtTabWidget");
    setAcceptDrops(true);
    d->tabBar = new QtTabBar(this);
    setTabBar(d->tabBar);
    connect(d->tabBar, &QtTabBar::tabDragCreate, this,
        [this](QMimeData* mimeData) { d_ptr->onTabDragCreate(mimeData); });
    connect(d->tabBar, &QtTabBar::tabDragEnter, this,
        [this](QMimeData* mimeData) { d_ptr->onTabDragEnter(mimeData); });
    connect(d->tabBar, &QtTabBar::tabDragLeave, this,
        [this](QMimeData* mimeData) { d_ptr->onTabDragLeave(mimeData); });
    connect(d->tabBar, &QtTabBar::tabDragDrop, this,
        [this](QMimeData* mimeData) { d_ptr->onTabDragDrop(mimeData); });
    connect(d->tabBar, &QtTabBar::tabCloseRequested, this,
        [this](int index) { d_ptr->onTabCloseRequested(index); });
}

QtTabWidget::~QtTabWidget()
{
    Q_D(QtTabWidget);
    d->clearAllTabWidgetList();
}

bool QtTabWidget::isTabTransparent() const
{
    Q_D(const QtTabWidget);
    return d->isTabTransparent;
}
void QtTabWidget::setIsTabTransparent(bool value)
{
    Q_D(QtTabWidget);
    d->isTabTransparent = value;
}

bool QtTabWidget::isContainerAcceptDrops() const
{
    Q_D(const QtTabWidget);
    return d->isContainerAcceptDrops;
}
void QtTabWidget::setIsContainerAcceptDrops(bool value)
{
    Q_D(QtTabWidget);
    d->isContainerAcceptDrops = value;
}

QSize QtTabWidget::tabSize() const
{
    Q_D(const QtTabWidget);
    return d->tabSize;
}
void QtTabWidget::setTabSize(QSize value)
{
    Q_D(QtTabWidget);
    d->tabSize = value;
}

QtTabBar* QtTabWidget::customTabBar() const
{
    Q_D(const QtTabWidget);
    return d->customTabBar;
}
void QtTabWidget::setCustomTabBar(QtTabBar* value)
{
    Q_D(QtTabWidget);
    d->customTabBar = value;
}

void QtTabWidget::onTabDragCreate(QMimeData* mimeData)
{
    Q_D(QtTabWidget);
    d->onTabDragCreate(mimeData);
}
void QtTabWidget::onTabDragEnter(QMimeData* mimeData)
{
    Q_D(QtTabWidget);
    d->onTabDragEnter(mimeData);
}
void QtTabWidget::onTabDragLeave(QMimeData* mimeData)
{
    Q_D(QtTabWidget);
    d->onTabDragLeave(mimeData);
}
void QtTabWidget::onTabDragDrop(QMimeData* mimeData)
{
    Q_D(QtTabWidget);
    d->onTabDragDrop(mimeData);
}
void QtTabWidget::onTabCloseRequested(int index)
{
    Q_D(QtTabWidget);
    d->onTabCloseRequested(index);
}

void QtTabWidget::paintEvent(QPaintEvent* event)
{
    Q_D(QtTabWidget);
    if(!d->isTabTransparent)
    {
        QTabWidget::paintEvent(event);
    }
}
void QtTabWidget::dragEnterEvent(QDragEnterEvent* event)
{
    Q_D(QtTabWidget);
    if(event->mimeData()->property("DragType").toString() == "QtTabBarDrag")
    {
        event->acceptProposedAction();
    }
    QTabWidget::dragEnterEvent(event);
}
void QtTabWidget::dropEvent(QDropEvent* event)
{
    Q_D(QtTabWidget);
    if(d->isContainerAcceptDrops &&
        event->mimeData()->property("QtTabWidgetObject").value<QtTabWidget*>() != this)
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
        data->setProperty("TabDropIndex", count());
        d->onTabDragDrop(data);
        event->accept();
    }
    QTabWidget::dropEvent(event);
}
void QtTabWidget::tabInserted(int index)
{
    Q_D(QtTabWidget);
    QWidget* tabWidget = widget(index);
    if(!tabWidget->property("IsMetaWidget").toBool() &&
        !tabWidget->property("QtOriginTabWidget").isValid())
    {
        d->allTabWidgetList.append(widget(index));
    }
    QTabWidget::tabInserted(index);
}