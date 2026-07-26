#include "qtscrollpage.h"
#include "command/qtscrollpageroutecommand.h"
#include "qtscrollarea.h"
#include "qtscrollbar.h"
#include "qtscrollpage_p.h"
#include <qboxlayout.h>
#include <qobject.h>
#include <qstackedwidget.h>
#include <qvariant.h>
#include <qvariantanimation.h>

QtScrollPagePrivate::QtScrollPagePrivate(QtScrollPage* q)
    : q_ptr(q)
{
}
QtScrollPagePrivate::~QtScrollPagePrivate()
{
}

void QtScrollPagePrivate::switchCentralStackIndex(int targetIndex, int lastIndex)
{
    Q_Q(QtScrollPage);
    QWidget* currentWidget = centralStackedWidget->widget(lastIndex);
    QWidget* targetWidget = centralStackedWidget->widget(targetIndex);
    targetWidget->resize(currentWidget->size());
    targetWidget->setVisible(true);

    QVariantAnimation* currentWidgetAnimation = new QVariantAnimation(q);
    currentWidgetAnimation->setEasingCurve(QEasingCurve::InExpo);
    currentWidgetAnimation->setDuration(300);
    QObject::connect(currentWidgetAnimation, &QVariantAnimation::valueChanged,
        [this, currentWidget](const QVariant& value) { currentWidget->move(value.toPoint()); });

    QVariantAnimation* targetWidgetAnimation = new QVariantAnimation(q);
    QObject::connect(targetWidgetAnimation, &QVariantAnimation::valueChanged,
        [q, targetWidget](const QVariant& value) { targetWidget->move(value.toPoint()); });
    QObject::connect(targetWidgetAnimation, &QVariantAnimation::finished,
        [this, targetIndex]() { centralStackedWidget->setCurrentIndex(targetIndex); });

    targetWidgetAnimation->setEasingCurve(QEasingCurve::InExpo);
    targetWidgetAnimation->setDuration(300);
    if(targetIndex > lastIndex)
    {
        currentWidgetAnimation->setStartValue(currentWidget->pos());
        currentWidgetAnimation->setEndValue(QPoint(-centralStackedWidget->width(), 0));
        targetWidgetAnimation->setStartValue(QPoint(centralStackedWidget->width(), 0));
        targetWidgetAnimation->setEndValue(QPoint(0, 0));
    }
    else
    {
        currentWidgetAnimation->setStartValue(currentWidget->pos());
        currentWidgetAnimation->setEndValue(QPoint(centralStackedWidget->width(), 0));
        targetWidgetAnimation->setStartValue(QPoint(-centralStackedWidget->width(), 0));
        targetWidgetAnimation->setEndValue(QPoint(0, 0));
    }
    currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    targetWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

QtScrollPage::QtScrollPage(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtScrollPagePrivate(this))
{
    Q_D(QtScrollPage);
    setProperty("QtBaseClassName", "QtScrollPage");
    d->q_ptr = this;
    d->customWidget = nullptr;
    d->breadcrumbBar = new QtBreadcrumbBar(this);
    d->breadcrumbBar->setTextPixelSize(28);
    connect(d->breadcrumbBar, &QtBreadcrumbBar::breadcrumbClicked, this,
        [this](const QString& breadcrumb, const QStringList& lastBreadcrumbList) {
            if(d_ptr->centralWidgetMap.contains(breadcrumb))
            {
                auto command = new QtScrollPageRouteCommand(this);
                command->setIsBreadcrumbClicked(true);
                command->setQtScrollPagePrivate(d_func());
                command->setUndoBreadcrumbList(lastBreadcrumbList);
                command->setRedoBreadcrumbList(d_ptr->breadcrumbBar->breadcrumbList());
                QtActionCommander::getInstance()->recordCommand("QtWidgetToolsAction", command);
            }
        });
    d->pageTitleLayout = new QHBoxLayout();
    d->pageTitleLayout->setContentsMargins(0, 0, 0, 0);
    d->pageTitleLayout->addWidget(d->breadcrumbBar);

    d->centralStackedWidget = new QStackedWidget(this);
    d->centralStackedWidget->setContentsMargins(0, 0, 0, 0);

    d->mainLayout = new QVBoxLayout(this);
    d->mainLayout->setSpacing(0);
    d->mainLayout->setContentsMargins(0, 0, 0, 0);
    d->mainLayout->addLayout(d->pageTitleLayout);
    d->mainLayout->addWidget(d->centralStackedWidget);
    setContentsMargins(20, 20, 0, 0);
}

QtScrollPage::~QtScrollPage()
{
}

void QtScrollPage::addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable,
    bool isVerticalGrabGesture, qreal mousePressEventDelay)
{
    Q_D(QtScrollPage);
    if(!centralWidget)
    {
        return;
    }
    if(centralWidget->windowTitle().isEmpty())
    {
        centralWidget->setWindowTitle(QString("Page_%1").arg(d->centralStackedWidget->count()));
    }
    if(d->centralStackedWidget->count() == 0)
    {
        d->breadcrumbBar->appendBreadcrumb(centralWidget->windowTitle());
    }
    QtScrollArea* scrollArea = new QtScrollArea(this);
    scrollArea->setMouseTracking(true);
    scrollArea->setIsAnimation(Qt::Vertical, true);
    scrollArea->setWidgetResizable(isWidgetResizeable);
    scrollArea->setIsGrabGesture(isVerticalGrabGesture, mousePressEventDelay);
    scrollArea->setIsOverShoot(Qt::Vertical, true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QtScrollBar* floatVScrollBar = new QtScrollBar(scrollArea->verticalScrollBar(), scrollArea);
    floatVScrollBar->setIsAnimation(true);

    QWidget* scrollPageContainer = new QWidget(this);
    scrollPageContainer->setObjectName("QtScrollPageContainer");
    scrollPageContainer->setStyleSheet("#QtScrollPageContainer{background-color:transparent;}");
    QVBoxLayout* scrollPageContainerLayout = new QVBoxLayout(scrollPageContainer);
    scrollPageContainerLayout->setContentsMargins(0, 0, 0, 0);
    scrollPageContainerLayout->addWidget(centralWidget);
    scrollArea->setWidget(scrollPageContainer);

    d->centralWidgetMap.insert(centralWidget->windowTitle(), d->centralStackedWidget->count());
    d->centralStackedWidget->addWidget(scrollArea);
}

void QtScrollPage::setCustomWidget(QWidget* widget)
{
    Q_D(QtScrollPage);
    if(!widget || widget == this)
    {
        return;
    }
    if(d->customWidget)
    {
        d->mainLayout->removeWidget(d->customWidget);
    }
    d->mainLayout->insertWidget(1, widget);
    d->customWidget = widget;
    Q_EMIT customWidgetChanged(widget);
}

QWidget* QtScrollPage::customWidget() const
{
    Q_D(const QtScrollPage);
    return d->customWidget;
}

void QtScrollPage::navigation(int widgetIndex, bool isLogRoute)
{
    Q_D(QtScrollPage);
    if(widgetIndex >= d->centralStackedWidget->count() || d->navigationTargetIndex == widgetIndex)
    {
        return;
    }
    int currentIndex = d->navigationTargetIndex;
    d->switchCentralStackIndex(widgetIndex, d->navigationTargetIndex);
    d->navigationTargetIndex = widgetIndex;
    QString pagetTitle = d->centralWidgetMap.key(widgetIndex);
    if(isLogRoute)
    {
        auto command = new QtScrollPageRouteCommand(this);
        command->setIsBreadcrumbClicked(false);
        command->setQtScrollPagePrivate(d);
        command->setUndoPageIndex(currentIndex);
        command->setRedoPageIndex(widgetIndex);
        QtActionCommander::getInstance()->recordCommand("QtWidgetToolsAction", command, false);
    }
    d->breadcrumbBar->appendBreadcrumb(pagetTitle);
}

void QtScrollPage::setPageTitleSpacing(int spacing)
{
    Q_D(QtScrollPage);
    d->pageTitleLayout->takeAt(0);
    d->pageTitleLayout->addSpacing(spacing);
    d->pageTitleLayout->addWidget(d->breadcrumbBar);
}

int QtScrollPage::pageTitleSpacing() const
{
    return d_ptr->pageTitleSpacing;
}

void QtScrollPage::setTitleVisible(bool isVisible)
{
    Q_D(QtScrollPage);
    d->breadcrumbBar->setVisible(isVisible);
}
