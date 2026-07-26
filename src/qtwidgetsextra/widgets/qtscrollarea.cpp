#include "qtscrollarea.h"
#include "widgets/qtscrollbar.h"
#include <QScroller>
class QtScrollAreaPrivate
{
    Q_DECLARE_PUBLIC(QtScrollArea)
public:
    QtScrollArea* q_ptr;
    QtScrollAreaPrivate(QtScrollArea* q);
    ~QtScrollAreaPrivate();
};

QtScrollAreaPrivate::QtScrollAreaPrivate(QtScrollArea* q)
    : q_ptr(q)
{
}
QtScrollAreaPrivate::~QtScrollAreaPrivate()
{
}

QtScrollArea::QtScrollArea(QWidget* parent)
    : QScrollArea{ parent }
    , d_ptr(new QtScrollAreaPrivate(this))
{
    Q_D(QtScrollArea);
    d->q_ptr = this;
    setObjectName("QtScrollArea");
    setStyleSheet("#QtScrollArea{background-color:transparent;border:0px;}");
    setHorizontalScrollBar(new QtScrollBar(this));
    setVerticalScrollBar(new QtScrollBar(this));
    QScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QtScrollArea::~QtScrollArea()
{
}

void QtScrollArea::setIsGrabGesture(bool isEnable, qreal mousePressEventDelay)
{
    if(isEnable)
    {
        QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
        QScrollerProperties properties =
            QScroller::scroller(this->viewport())->scrollerProperties();
        properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, mousePressEventDelay);
        properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
        properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
        properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
    else
    {
        QScroller::ungrabGesture(this->viewport());
    }
}

void QtScrollArea::setIsOverShoot(Qt::Orientation orientation, bool isEnable)
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    properties.setScrollMetric(orientation == Qt::Horizontal
                                   ? QScrollerProperties::HorizontalOvershootPolicy
                                   : QScrollerProperties::VerticalOvershootPolicy,
        isEnable ? QScrollerProperties::OvershootAlwaysOn
                 : QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(this->viewport())->setScrollerProperties(properties);
}

bool QtScrollArea::isOverShoot(Qt::Orientation orientation) const
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    return properties
        .scrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy
                                                    : QScrollerProperties::VerticalOvershootPolicy)
        .toBool();
}

void QtScrollArea::setIsAnimation(Qt::Orientation orientation, bool isAnimation)
{
    if(orientation == Qt::Horizontal)
    {
        dynamic_cast<QtScrollBar*>(this->horizontalScrollBar())->setIsAnimation(isAnimation);
    }
    else
    {
        dynamic_cast<QtScrollBar*>(this->verticalScrollBar())->setIsAnimation(isAnimation);
    }
}

bool QtScrollArea::isAnimation(Qt::Orientation orientation) const
{
    if(orientation == Qt::Horizontal)
    {
        return dynamic_cast<QtScrollBar*>(this->horizontalScrollBar())->isAnimation();
    }
    else
    {
        return dynamic_cast<QtScrollBar*>(this->verticalScrollBar())->isAnimation();
    }
}
