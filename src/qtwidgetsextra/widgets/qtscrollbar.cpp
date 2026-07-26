#include "qtscrollbar.h"
#include <QAbstractScrollArea>
#include <QApplication>
#include <QEvent>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QProxyStyle>
#include <QScrollBar>
#include <QStyleOptionSlider>
#include <QTimer>
#include <QVariant>
#include <QVariantAnimation>
#include <QWheelEvent>
#include <qpropertyanimation.h>
#include <qscrollbar.h>
#include <qtpreprocessorsupport.h>
#include "styles/qtscrollbarstyle.h"

class QtScrollBarPrivate
{
public:
    Q_DECLARE_PUBLIC(QtScrollBar);
    bool isAnimation;
    qreal speedLimit;
    int targetMaximum;

    QtScrollBar* q_ptr;
    QScrollBar* originScrollBar{ nullptr };
    QAbstractScrollArea* originScrollArea{ nullptr };
    QTimer* expandTimer{ nullptr };
    bool isExpand{ false };
    int scrollValue{ -1 };
    QPropertyAnimation* slideSmoothAnimation{ nullptr };
    QtScrollBarPrivate(QtScrollBar* q);
    void scroll(Qt::KeyboardModifiers modifiers, int value);
    void onRangeChanged(int min, int max);
    void initAllConfig();
    void handleScrollBarValueChanged(QScrollBar* scrollBar, int value);
    void handleScrollBarRangeChanged(int min, int max);
    void handleScrollBarGeometry();
};
QtScrollBarPrivate::QtScrollBarPrivate(QtScrollBar* q)
    :isAnimation(false), q_ptr(q)
{
}

void QtScrollBarPrivate::scroll(Qt::KeyboardModifiers modifiers, int delta)
{
    Q_Q(QtScrollBar);
    int stepsToScroll = 0;
    qreal offset = qreal(delta) / 120;
    int pageStep = 10;
    int singleStep = q->singleStep();
    if((modifiers & Qt::ControlModifier) || (modifiers & Qt::ShiftModifier))
    {
        stepsToScroll = qBound(-pageStep, int(offset * pageStep), pageStep);
    }
    else
    {
        stepsToScroll = QApplication::wheelScrollLines() * offset * singleStep;
    }
    if(abs(scrollValue - q->value()) > abs(stepsToScroll * speedLimit))
    {
        scrollValue = q->value();
    }
    scrollValue -= stepsToScroll;
    slideSmoothAnimation->stop();
    slideSmoothAnimation->setStartValue(q->value());
    slideSmoothAnimation->setEndValue(scrollValue);
    slideSmoothAnimation->start();
}

void QtScrollBarPrivate::onRangeChanged(int min, int max)
{
    Q_Q(QtScrollBar);
    Q_UNUSED(min)
    if(q->isVisible() && isAnimation && max != 0)
    {
        QVariantAnimation* rangeSmoothAnimation = new QVariantAnimation(q);
        QObject::connect(rangeSmoothAnimation, &QVariantAnimation::finished,
            [=]() { Q_EMIT q->rangeAnimationFinished(); });
        QObject::connect(
            rangeSmoothAnimation, &QVariantAnimation::valueChanged, q,[q, this](const QVariant& value) {
                targetMaximum = value.toInt();
                q->blockSignals(true);
                q->setMaximum(value.toUInt());
                q->blockSignals(false);
                q->update();
            });
        rangeSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
        rangeSmoothAnimation->setDuration(250);
        rangeSmoothAnimation->setStartValue(targetMaximum);
        rangeSmoothAnimation->setEndValue(max);
        rangeSmoothAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        if(max == 0)
        {
            scrollValue = -1;
        }
        targetMaximum = max;
    }
}

void QtScrollBarPrivate::initAllConfig()
{
    Q_Q(QtScrollBar);
    handleScrollBarRangeChanged(originScrollBar->minimum(), originScrollBar->maximum());
    q->setSingleStep(originScrollBar->singleStep());
    q->setPageStep(originScrollBar->pageStep());
}

void QtScrollBarPrivate::handleScrollBarValueChanged(QScrollBar* scrollBar, int value)
{
    scrollBar->setValue(value);
}
void QtScrollBarPrivate::handleScrollBarRangeChanged(int min, int max)
{
    Q_Q(QtScrollBar);
    q->setRange(min, max);
    if(max <= 0)
    {
        q->setVisible(false);
    }
    else
    {
        q->setVisible(true);
    }
}
void QtScrollBarPrivate::handleScrollBarGeometry()
{
    Q_Q(QtScrollBar);
    q->raise();
    q->setSingleStep(originScrollBar->singleStep());
    q->setPageStep(originScrollBar->pageStep());
    if(q->orientation() == Qt::Horizontal)
    {
        q->setGeometry(0, originScrollArea->height() - 10, originScrollArea->width(), 10);
    }
    else
    {
        q->setGeometry(originScrollArea->width() - 10, 0, 10, originScrollArea->height());
    }
}

QtScrollBar::QtScrollBar(QWidget* parent)
    : QScrollBar(parent)
    , d_ptr(new QtScrollBarPrivate(this))
{
    Q_D(QtScrollBar);
    setSingleStep(1);
    setObjectName("QtScrollBar");
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    d->speedLimit = 20;
    d->targetMaximum = 0;
    connect(this, &QtScrollBar::rangeChanged,
        [this](int min, int max) { d_ptr->onRangeChanged(min, max); });
    QtScrollBarStyle* scrollBarStyle = new QtScrollBarStyle(style());
    scrollBarStyle->setScrollBar(this);
    setStyle(scrollBarStyle);
    d->slideSmoothAnimation = new QPropertyAnimation(this, "value");
    d->slideSmoothAnimation->setEasingCurve(QEasingCurve::OutSine);
    d->slideSmoothAnimation->setDuration(300);
    connect(
        d->slideSmoothAnimation, &QVariantAnimation::finished, [d, this]() { d->scrollValue = value(); });
    d->expandTimer = new QTimer(this);
    connect(d->expandTimer, &QTimer::timeout, [d, scrollBarStyle]() {
        d->expandTimer->stop();
        d->isExpand = d->q_ptr->underMouse();
        scrollBarStyle->startExpandAnimation(d->isExpand);
    });
}
QtScrollBar::QtScrollBar(Qt::Orientation orientation, QWidget* parent)
    : QtScrollBar(parent)
{
    Q_D(QtScrollBar);
    Q_UNUSED(d)
    setOrientation(orientation);
}
QtScrollBar::QtScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent)
    : QtScrollBar(parent)
{
    Q_D(QtScrollBar);
    if(!originScrollBar || !parent)
    {
        qCritical() << "Invalid origin or parent!";
        return;
    }
    d->originScrollArea = parent;
    Qt::Orientation orientation = originScrollBar->orientation();
    setOrientation(orientation);
    orientation == Qt::Horizontal ? parent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff)
                                  : parent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    parent->installEventFilter(this);

    d->originScrollBar = originScrollBar;
    d->initAllConfig();

    connect(d->originScrollBar, &QScrollBar::valueChanged, this,
        [this](int value) { d_ptr->handleScrollBarValueChanged(this, value); });
    connect(this, &QScrollBar::valueChanged, this,
        [this](int value) { d_ptr->handleScrollBarValueChanged(d_ptr->originScrollBar, value); });
    connect(d->originScrollBar, &QScrollBar::rangeChanged, this,
        [this](int min, int max) { d_ptr->handleScrollBarRangeChanged(min, max); });
}
QtScrollBar::~QtScrollBar()
{
    delete style();
}

bool QtScrollBar::isAnimation() const
{
    Q_D(const QtScrollBar);
    return d->isAnimation;
}
void QtScrollBar::setIsAnimation(bool value)
{
    Q_D(QtScrollBar);
    d->isAnimation = value;
}

qreal QtScrollBar::speedLimit() const
{
    Q_D(const QtScrollBar);
    return d->speedLimit;
}
void QtScrollBar::setSpeedLimit(qreal value)
{
    Q_D(QtScrollBar);
    d->speedLimit = value;
}

bool QtScrollBar::event(QEvent* event)
{
    Q_D(QtScrollBar);
    switch(event->type())
    {
        case QEvent::Enter: {
            d->expandTimer->stop();
            if(!d->isExpand)
            {
                d->expandTimer->start(350);
            }
            break;
        }
        case QEvent::Leave: {
            d->expandTimer->stop();
            if(d->isExpand)
            {
                d->expandTimer->start(350);
            }
            break;
        }
        default: {
            break;
        }
    }
    return QScrollBar::event(event);
}
bool QtScrollBar::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtScrollBar);
    switch(event->type())
    {
        case QEvent::Show:
        case QEvent::Resize:
        case QEvent::LayoutRequest: {
            d->handleScrollBarGeometry();
            break;
        }
        default: {
            break;
        }
    }
    return QScrollBar::eventFilter(watched, event);
}
void QtScrollBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtScrollBar);
    d->slideSmoothAnimation->stop();
    QScrollBar::mousePressEvent(event);
    d->scrollValue = value();
}
void QtScrollBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtScrollBar);
    d->slideSmoothAnimation->stop();
    QScrollBar::mouseReleaseEvent(event);
    d->scrollValue = value();
}
void QtScrollBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtScrollBar);
    d->slideSmoothAnimation->stop();
    QScrollBar::mouseMoveEvent(event);
    d->scrollValue = value();
}
void QtScrollBar::wheelEvent(QWheelEvent* event)
{
    Q_D(QtScrollBar);
    if(d->isAnimation)
    {
        int verticalDelta = event->angleDelta().y();
        if(d->slideSmoothAnimation->state() == QAbstractAnimation::Stopped)
        {
            d->scrollValue = value();
        }
        if(verticalDelta != 0)
        {
            if((value() == minimum() && verticalDelta > 0) ||
                (value() == maximum() && verticalDelta < 0))
            {
                QScrollBar::wheelEvent(event);
                return;
            }
            d->scroll(event->modifiers(), verticalDelta);
        }
        else
        {
            int horizontalDelta = event->angleDelta().x();
            if((value() == minimum() && horizontalDelta > 0) ||
                (value() == maximum() && horizontalDelta < 0))
            {
                QScrollBar::wheelEvent(event);
                return;
            }
            d->scroll(event->modifiers(), horizontalDelta);
        }
    }
    else
    {
        QScrollBar::wheelEvent(event);
    }

    event->accept();
}
void QtScrollBar::contextMenuEvent(QContextMenuEvent* event)
{
    QScrollBar::contextMenuEvent(event);
}

