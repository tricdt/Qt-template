#include "QtCustomTabWidget.h"
#include "QtTabBar.h"
#include "qtcustomtabwidget.h"
#include "qttabbar.h"
#include "qttabwidget.h"
#include <qdialog.h>
#include <qmimedata.h>

class QtCustomTabWidgetPrivate
{
    bool isFinished;
    Q_DECLARE_PUBLIC(QtCustomTabWidget)
public:
    QtCustomTabWidget* q_ptr;
    QtTabBar* customTabBar{ nullptr };
    QtTabWidget* customTabWidget{ nullptr };
    bool isAllowLeave{ false };
    QtCustomTabWidgetPrivate(QtCustomTabWidget* q);
    ~QtCustomTabWidgetPrivate();
};

QtCustomTabWidgetPrivate::QtCustomTabWidgetPrivate(QtCustomTabWidget* q)
    : q_ptr(q)
{
}
QtCustomTabWidgetPrivate::~QtCustomTabWidgetPrivate()
{
}

QtCustomTabWidget::QtCustomTabWidget(QWidget* parent)
    : QtCustomWidget{ parent }
    , d_ptr(new QtCustomTabWidgetPrivate(this))
{
    Q_D(QtCustomTabWidget);
    d->isFinished = false;
    resize(700, 500);
    setWindowTitle("");
#ifndef Q_OS_WIN
    setAttribute(Qt::WA_Hover);
#endif
    setMouseTracking(true);
    setWindowIcon(QIcon());
    d->customTabWidget = new QtTabWidget(this);
    d->customTabWidget->setIsTabTransparent(true);
    d->customTabWidget->setObjectName("QtCustomTabWidget");
    QTabBar* originTabBar = d->customTabWidget->tabBar();
    originTabBar->hide();
    setAcceptDrops(true);
    d->customTabBar = new QtTabBar(this);
    d->customTabBar->setObjectName("QtCustomTabBar");
    connect(d->customTabBar, &QtTabBar::tabMoved, this,
        [this](int from, int to) { d_ptr->customTabWidget->tabBar()->moveTab(from, to); });
    connect(d->customTabBar, &QtTabBar::currentChanged, this,
        [this](int index) { d_ptr->customTabWidget->setCurrentIndex(index); });
    connect(d->customTabWidget, &QtTabWidget::currentChanged, this, [this](int index) {
        if(index == -1)
        {
            d_ptr->isFinished = true;
            hide();
        }
    });
    connect(
        d->customTabBar, &QtTabBar::tabCloseRequested, originTabBar, &QTabBar::tabCloseRequested);
    d->customTabWidget->setCustomTabBar(d->customTabBar);
    connect(d->customTabBar, &QtTabBar::tabDragCreate, d->customTabWidget,
        [this](QMimeData* mimeData) { d_ptr->customTabWidget->onTabDragCreate(mimeData); });
    connect(d->customTabBar, &QtTabBar::tabDragDrop, d->customTabWidget,
        [this](QMimeData* mimeData) { d_ptr->customTabWidget->onTabDragDrop(mimeData); });
    connect(d->customTabBar, &QtTabBar::tabDragEnter, d->customTabWidget,
        [this](QMimeData* mimeData) { d_ptr->customTabWidget->onTabDragEnter(mimeData); });
    connect(d->customTabBar, &QtTabBar::tabDragLeave, d->customTabWidget,
        [this](QMimeData* mimeData) { d_ptr->customTabWidget->onTabDragLeave(mimeData); });

    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(10, 0, 10, 0);
    customLayout->addStretch();
    customLayout->addWidget(d->customTabBar);
    appBar->setCustomWidget(QtAppBarType::LeftArea, customWidget, this, "processHitTest");
    setCentralWidget(d->customTabWidget);
}

QtCustomTabWidget::~QtCustomTabWidget()
{
    Q_D(QtCustomTabWidget);
    while(d->customTabWidget->count() > 0)
    {
        QWidget* closeWidget = d->customTabWidget->widget(0);
        QtTabWidget* originTabWidget =
            closeWidget->property("QtOriginTabWidget").value<QtTabWidget*>();
        if(originTabWidget)
        {
            closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<QtTabBar*>(nullptr));
            originTabWidget->addTab(
                closeWidget, d->customTabWidget->tabIcon(0), d->customTabWidget->tabText(0));
            originTabWidget->setCurrentWidget(closeWidget);
        }
        else
        {
            d->customTabWidget->removeTab(0);
        }
    }
}

void QtCustomTabWidget::addTab(QWidget* widget, QIcon tabIcon, const QString& tabTitle)
{
    Q_D(QtCustomTabWidget);
    d->customTabWidget->addTab(widget, tabIcon, tabTitle);
    d->customTabBar->addTab(tabIcon, tabTitle);
}


bool QtCustomTabWidget::isFinished() const
{
    Q_D(const QtCustomTabWidget);
    return d->isFinished;
}
void QtCustomTabWidget::setIsFinished(bool value)
{
    Q_D(QtCustomTabWidget);
    d->isFinished = value;
}

bool QtCustomTabWidget::isAllowLeave() const
{
    Q_D(const QtCustomTabWidget);
    return d->isAllowLeave;
}
void QtCustomTabWidget::setIsAllowLeave(bool value)
{
    Q_D(QtCustomTabWidget);
    d->isAllowLeave = value;
}

QtTabBar* QtCustomTabWidget::customTabBar() const
{
    Q_D(const QtCustomTabWidget);
    return d->customTabBar;
}
void QtCustomTabWidget::setCustomTabBar(QtTabBar* value)
{
    Q_D(QtCustomTabWidget);
    d->customTabBar = value;
}

QtTabWidget* QtCustomTabWidget::customTabWidget() const
{
    Q_D(const QtCustomTabWidget);
    return d->customTabWidget;
}
void QtCustomTabWidget::setCustomTabWidget(QtTabWidget* value)
{
    Q_D(QtCustomTabWidget);
    d->customTabWidget = value;
}

bool QtCustomTabWidget::processHitTest()
{
    Q_D(QtCustomTabWidget);
    auto point = d->customTabBar->mapFromGlobal(QCursor::pos());
    return d->customTabBar->tabAt(point) < 0;
}

void QtCustomTabWidget::closeEvent(QCloseEvent* event)
{
    Q_D(QtCustomTabWidget);

    // 1. Chặn tín hiệu để tránh gọi nhầm hide() hay thay đổi index đột ngột
    // d->customTabWidget->blockSignals(true);
    // d->customTabBar->blockSignals(true);

    // // 2. GỠ SẠCH CÁC TAB RA KHỎI QDIALOG TRƯỚC KHI NÓ BỊ ĐÓNG
    // int count = d->customTabWidget->count();
    // qDebug() << "count" << count;
    // for (int i = count - 1; i >= 0; --i)
    // {
    //     QWidget* tabWidget = d->customTabWidget->widget(i);
    //     if (tabWidget)
    //     {
    //         // Kiểm tra xem tab này có cần trả về cửa sổ gốc không
    //         QtTabWidget* originTabWidget =
    //         tabWidget->property("QtOriginTabWidget").value<QtTabWidget*>();

    //         if (originTabWidget && originTabWidget != d->customTabWidget)
    //         {
    //             QIcon icon = d->customTabWidget->tabIcon(i);
    //             QString text = d->customTabWidget->tabText(i);

    //             // Gỡ khỏi cửa sổ nổi và trả về cửa sổ chính an toàn
    //             d->customTabWidget->removeTab(i);
    //             originTabWidget->addTab(tabWidget, icon, text);
    //             originTabWidget->setCurrentWidget(tabWidget);
    //         }
    //         else
    //         {
    //             // Nếu là tab độc lập, gỡ ra và hủy an toàn bằng deleteLater
    //             d->customTabWidget->removeTab(i);
    //             tabWidget->deleteLater();
    //         }
    //     }
    // }

    // 3. Chấp nhận đóng và hủy QDialog an toàn qua hàng đợi
    // event->accept();
    // this->deleteLater();
}