#include "qtpivot.h"
#include "itemviews/models/qtpivotmodel.h"
#include "itemviews/views/qtpivotview.h"
#include "styles/qtpivotstyle.h"
#include <qboxlayout.h>
#include <qscroller.h>
class QtPivotPrivate
{
    Q_DECLARE_PUBLIC(QtPivot)
public:
    int textPixelSize;
    QtPivot* q_ptr;
    QtPivotModel* listModel{ nullptr };
    QtPivotStyle* listStyle{ nullptr };
    QtPivotView* listView{ nullptr };
    QtPivotPrivate(QtPivot* q);
    ~QtPivotPrivate();
    void checkCurrentIndex();
};

QtPivotPrivate::QtPivotPrivate(QtPivot* q)
    : q_ptr(q)
{
}
QtPivotPrivate::~QtPivotPrivate()
{
}

void QtPivotPrivate::checkCurrentIndex()
{
    Q_Q(QtPivot);
    QModelIndex currentIndex = listView->currentIndex();
    if(currentIndex.row() != listStyle->currentIndex())
    {
        listView->doCurrentIndexChangedAnimation(currentIndex);
        listStyle->setCurrentIndex(currentIndex.row());
        Q_EMIT q->currentIndexChanged();
    }
}

QtPivot::QtPivot(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtPivotPrivate(this))
{
    Q_D(QtPivot);
    d->textPixelSize = 20;
    setFixedHeight(40);
    setObjectName("QtPivot");
    setStyleSheet("#QtPivot{background-color:transparent;}");
    setMouseTracking(true);

    d->listView = new QtPivotView(this);
    d->listView->setMinimumHeight(0);
    d->listView->setFlow(QListView::LeftToRight);
    d->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->listModel = new QtPivotModel(this);
    d->listView->setModel(d->listModel);
    d->listStyle = new QtPivotStyle(d->listView->style());
    d->listView->setStyle(d->listStyle);
    d->listView->setPivotStyle(d->listStyle);

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

    connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if(newstate == QScroller::Pressed)
        {
            d->listStyle->setPressIndex(
                d->listView->indexAt(d->listView->mapFromGlobal(QCursor::pos())));
            d->listView->viewport()->update();
        }
        else if(newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            d->listStyle->setPressIndex(QModelIndex());
            d->listView->viewport()->update();
        }
    });
    connect(d->listView, &QtPivotView::clicked, this, [this](const QModelIndex& index) {
        if(index.row() != d_ptr->listStyle->currentIndex())
        {
            Q_EMIT currentIndexChanged();
        }
        d_ptr->listView->doCurrentIndexChangedAnimation(index);
        d_ptr->listStyle->setCurrentIndex(index.row());
        Q_EMIT pivotClicked(index.row());
    });
    connect(d->listView, &QtPivotView::doubleClicked, this,
        [this](const QModelIndex& index) { Q_EMIT pivotDoubleClicked(index.row()); });
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->listView);
}

QtPivot::~QtPivot()
{
    Q_D(QtPivot);
    delete d->listStyle;
}

int QtPivot::textPixelSize() const
{
    Q_D(const QtPivot);
    return d->textPixelSize;
}
void QtPivot::setTextPixelSize(int value)
{
    Q_D(QtPivot);
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

int QtPivot::currentIndex() const
{
    Q_D(const QtPivot);
    return d->listView->currentIndex().row();
}
void QtPivot::setCurrentIndex(int value)
{
    Q_D(QtPivot);
    if(value < d->listModel->pivotListCount())
    {
        QModelIndex index = d->listModel->index(value, 0);
        d->listView->doCurrentIndexChangedAnimation(index);
        if(index.row() != d->listStyle->currentIndex())
        {
            Q_EMIT currentIndexChanged();
        }
        d->listStyle->setCurrentIndex(value);
    }
}

int QtPivot::pivotSpacing() const
{
    Q_D(const QtPivot);
    return d->listStyle->pivotSpacing();
}
void QtPivot::setPivotSpacing(int value)
{
    Q_D(QtPivot);
    if(value >= 0)
    {
        d->listStyle->setPivotSpacing(value);
        d->listView->doItemsLayout();
        Q_EMIT pivotSpacingChanged();
    }
}

int QtPivot::markWidth() const
{
    Q_D(const QtPivot);
    return d->listView->markWidth();
}
void QtPivot::setMarkWidth(int value)
{
    Q_D(QtPivot);
    if(value >= 0)
    {
        d->listView->setMarkWidth(value);
        d->listView->viewport()->update();
        Q_EMIT markWidthChanged();
    }
}

void QtPivot::appendPivot(QString pivotTitle)
{
    Q_D(QtPivot);
    d->listModel->appendPivot(pivotTitle);
    d->checkCurrentIndex();
}
void QtPivot::removePivot(QString pivotTitle)
{
    Q_D(QtPivot);
    d->listModel->removePivot(pivotTitle);
    d->checkCurrentIndex();
}
