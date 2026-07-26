#include "qtbreadcrumbbar.h"
#include "itemviews/delegates/qtbreadcrumbbardelegate.h"
#include "itemviews/models/qtbreadcrumbbarmodel.h"
#include "itemviews/views/qtbaselistview.h"
#include <qboxlayout.h>
#include <qtmetamacros.h>
#include <QScroller>

class QtBreadcrumbBarPrivate
{
    Q_DECLARE_PUBLIC(QtBreadcrumbBar)
public:
    int textPixelSize;
    bool isAutoRemove;
    QtBreadcrumbBar* q_ptr;
    QtBreadcrumbBarModel* listModel{ nullptr };
    QtBaseListView* listView{ nullptr };
    QtBreadcrumbBarDelegate* listDelegate{ nullptr };
    QtBreadcrumbBarPrivate(QtBreadcrumbBar* q);
    ~QtBreadcrumbBarPrivate();
};

QtBreadcrumbBarPrivate::QtBreadcrumbBarPrivate(QtBreadcrumbBar* q)
    : q_ptr(q)
{
}
QtBreadcrumbBarPrivate::~QtBreadcrumbBarPrivate()
{
}

QtBreadcrumbBar::QtBreadcrumbBar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtBreadcrumbBarPrivate(this))
{
    Q_D(QtBreadcrumbBar);
    d->textPixelSize = 18;
    d->isAutoRemove = true;
    setFixedHeight(37);
    setObjectName("QtBreadcrumbBar");
    setStyleSheet("#QtBreadcrumbBar{background-color:transparent;}");
    setMouseTracking(true);

    d->listView = new QtBaseListView(this);
    d->listView->setMinimumHeight(0);
    d->listView->setFlow(QListView::LeftToRight);
    d->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->listModel = new QtBreadcrumbBarModel(this);
    d->listView->setModel(d->listModel);
    d->listDelegate = new QtBreadcrumbBarDelegate(this);
    d->listView->setItemDelegate(d->listDelegate);
    connect(d->listView, &QListView::clicked, this, [this](const QModelIndex& index) {
        if(d_ptr->isAutoRemove)
        {
            if(d_ptr->listModel->breadcrumbListCount() != 1 &&
                index.row() != d_ptr->listModel->breadcrumbListCount() * 2 - 2 &&
                index.data(Qt::DisplayRole).toString() != ">")
            {
                auto breadcrumb = index.data(Qt::DisplayRole).toString();
                auto lastBreadcrumbList = d_ptr->listModel->breadcrumbList();
                d_ptr->listModel->removeBreadcrumb(index.row() / 2 + 1);
                Q_EMIT breadcrumbClicked(breadcrumb, lastBreadcrumbList);
            }
        }
        else
        {
            Q_EMIT breadcrumbClicked(
                index.data(Qt::DisplayRole).toString(), d_ptr->listModel->breadcrumbList());
        }
    });
    QFont textFont = this->font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(d->textPixelSize);
    d->listView->setFont(textFont);

    QScroller::grabGesture(d->listView->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(d->listView->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(
        QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    connect(scroller, &QScroller::stateChanged, this, [this](QScroller::State newstate) {
        if(newstate == QScroller::Pressed)
        {
            d_ptr->listDelegate->setPressIndex(
                d_ptr->listView->indexAt(d_ptr->listView->mapFromGlobal(QCursor::pos())));
            d_ptr->listView->viewport()->update();
        }
        else if(newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            d_ptr->listDelegate->setPressIndex(QModelIndex());
        }
    });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->listView);
}

QtBreadcrumbBar::~QtBreadcrumbBar()
{
}

void QtBreadcrumbBar::setBreadcrumbList(QStringList breadcrumbList)
{
    Q_D(QtBreadcrumbBar);
    d->listModel->setBreadcrumbList(breadcrumbList);
}
QStringList QtBreadcrumbBar::appendBreadcrumb(QString breadcrumb)
{
    Q_D(QtBreadcrumbBar);
    d->listModel->appendBreadcrumb(breadcrumb);
    return d->listModel->breadcrumbList();
}
QStringList QtBreadcrumbBar::removeBreadcrumb(QString breadcrumb)
{
    Q_D(QtBreadcrumbBar);
    d->listModel->removeBreadcrumb(breadcrumb);
    return d->listModel->breadcrumbList();
}

int QtBreadcrumbBar::breadcrumbListCount() const
{
    Q_D(const QtBreadcrumbBar);
    return d->listModel->breadcrumbListCount();
}
QStringList QtBreadcrumbBar::breadcrumbList() const
{
    Q_D(const QtBreadcrumbBar);
    return d->listModel->breadcrumbList();
}

int QtBreadcrumbBar::textPixelSize() const
{
    Q_D(const QtBreadcrumbBar);
    return d->textPixelSize;
}
void QtBreadcrumbBar::setTextPixelSize(int value)
{
    Q_D(QtBreadcrumbBar);
    if(value > 0)
    {
        d->textPixelSize = value;
        QFont textFont = this->font();
        textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
        textFont.setPixelSize(d->textPixelSize);
        d->listView->setFont(textFont);
        Q_EMIT textPixelSizeChanged();
    }
}

bool QtBreadcrumbBar::isAutoRemove() const
{
    Q_D(const QtBreadcrumbBar);
    return d->isAutoRemove;
}
void QtBreadcrumbBar::setIsAutoRemove(bool value)
{
    Q_D(QtBreadcrumbBar);
    d->isAutoRemove = value;
    Q_EMIT isAutoRemoveChanged();
}
