#include "qtpivotview.h"
#include "widgets/qtscrollbar.h"
#include <qevent.h>
#include <qpainter.h>
#include <qpropertyanimation.h>

class QtPivotViewPrivate
{
    Q_DECLARE_PUBLIC(QtPivotView)
public:
    int markX;
    int markWidth;
    int markAnimationWidth;
    QtPivotStyle* pivotStyle;
    QModelIndex currentIndex;
    QRect currentIndexRect;
    bool isAnimationFinished;
    QtPivotView* q_ptr;
    QtPivotViewPrivate(QtPivotView* q);
    ~QtPivotViewPrivate();
};

QtPivotViewPrivate::QtPivotViewPrivate(QtPivotView* q)
    : q_ptr(q)
{
}
QtPivotViewPrivate::~QtPivotViewPrivate()
{
}

QtPivotView::QtPivotView(QWidget* parent)
    : QListView{ parent }
    , d_ptr(new QtPivotViewPrivate(this))
{
    Q_D(QtPivotView);
    d->markX = 0;
    d->markWidth = 40;
    d->markAnimationWidth = 0;
    d->currentIndex = QModelIndex();
    d->currentIndexRect = QRect();
    d->isAnimationFinished = true;
    setObjectName("QtPivotView");
    setStyleSheet("#QtPivotView{background-color:transparent;}");
    setMouseTracking(true);
    setVerticalScrollBar(new QtScrollBar(this));
    setHorizontalScrollBar(new QtScrollBar(this));
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

QtPivotView::~QtPivotView()
{
}

int QtPivotView::markX() const
{
    Q_D(const QtPivotView);
    return d->markX;
}
void QtPivotView::setMarkX(int value)
{
    Q_D(QtPivotView);
    d->markX = value;
}

int QtPivotView::markWidth() const
{
    Q_D(const QtPivotView);
    return d->markWidth;
}
void QtPivotView::setMarkWidth(int value)
{
    Q_D(QtPivotView);
    d->markWidth = value;
}

int QtPivotView::markAnimationWidth() const
{
    Q_D(const QtPivotView);
    return d->markAnimationWidth;
}
void QtPivotView::setMarkAnimationWidth(int value)
{
    Q_D(QtPivotView);
    d->markAnimationWidth = value;
}

QtPivotStyle* QtPivotView::pivotStyle() const
{
    Q_D(const QtPivotView);
    return d->pivotStyle;
}
void QtPivotView::setPivotStyle(QtPivotStyle* value)
{
    Q_D(QtPivotView);
    d->pivotStyle = value;
}

QModelIndex QtPivotView::currentIndex() const
{
    Q_D(const QtPivotView);
    return d->currentIndex;
}
void QtPivotView::setCurrentIndex(QModelIndex value)
{
    Q_D(QtPivotView);
    d->currentIndex = value;
}

QRect QtPivotView::currentIndexRect() const
{
    Q_D(const QtPivotView);
    return d->currentIndexRect;
}
void QtPivotView::setCurrentIndexRect(QRect value)
{
    Q_D(QtPivotView);
    d->currentIndexRect = value;
}

bool QtPivotView::isAnimationFinished() const
{
    Q_D(const QtPivotView);
    return d->isAnimationFinished;
}
void QtPivotView::setIsAnimationFinished(bool value)
{
    Q_D(QtPivotView);
    d->isAnimationFinished = value;
}

void QtPivotView::doCurrentIndexChangedAnimation(const QModelIndex& index)
{
    Q_D(QtPivotView);
    int xOffset = visualRect(d->currentIndex).x() - d->currentIndexRect.x();
    if(index.row() >= 0)
    {
        d->currentIndexRect = visualRect(index);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "markX");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
        connect(markAnimation, &QPropertyAnimation::finished, this, [this]() {
            d_ptr->isAnimationFinished = true;
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if(d->pivotStyle->currentIndex() >= 0)
        {
            markAnimation->setStartValue(d->markX + xOffset);
            markAnimation->setEndValue(d->currentIndexRect.center().x() - d->markWidth / 2);
        }
        else
        {
            markAnimation->setStartValue(d->currentIndexRect.center().x());
            markAnimation->setEndValue(d->currentIndexRect.center().x() - d->markWidth / 2);

            QPropertyAnimation* markWidthAnimation =
                new QPropertyAnimation(this, "markAnimationWidth");
            markWidthAnimation->setDuration(300);
            markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
            markWidthAnimation->setStartValue(0);
            markWidthAnimation->setEndValue(d->markWidth);
            markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        d->currentIndexRect = visualRect(model()->index(d->pivotStyle->currentIndex(), 0));
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "markX");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
        connect(markAnimation, &QPropertyAnimation::finished, this, [this]() {
            d_ptr->isAnimationFinished = true;
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->currentIndexRect.center().x() - d->markWidth / 2);
        markAnimation->setEndValue(d->currentIndexRect.center().x());
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* markWidthAnimation =
            new QPropertyAnimation(this, "markAnimationWidth");
        markWidthAnimation->setDuration(300);
        markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markWidthAnimation->setStartValue(d->markAnimationWidth);
        markWidthAnimation->setEndValue(0);
        markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    d->currentIndex = index;
    d->isAnimationFinished = false;
    setCurrentIndex(index);
}

void QtPivotView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_D(QtPivotView);
    d->pivotStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QListView::mouseDoubleClickEvent(event);
}

void QtPivotView::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtPivotView);
    if(event->button() == Qt::LeftButton)
    {
        d->pivotStyle->setPressIndex(QModelIndex());
        viewport()->update();
    }
    QListView::mouseReleaseEvent(event);
}

void QtPivotView::wheelEvent(QWheelEvent* event)
{
    Q_D(QtPivotView);
    QListView::wheelEvent(event);
    event->accept();
}

void QtPivotView::paintEvent(QPaintEvent* event)
{
    Q_D(QtPivotView);
    QPainter painter(viewport());
    QRect viewPortRect = viewport()->rect();
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->pivotStyle->markColor());
    QRect currentIndexRect = visualRect(d->currentIndex);
    if(d->isAnimationFinished)
    {
        painter.drawRoundedRect(QRect(currentIndexRect.center().x() - d->markWidth / 2,
                                    viewPortRect.bottom() - 4, d->markAnimationWidth, 3),
            3, 3);
    }
    else
    {
        int xOffset = currentIndexRect.x() - d->currentIndexRect.x();
        painter.drawRoundedRect(
            QRect(d->markX + xOffset, viewPortRect.bottom() - 4, d->markAnimationWidth, 3), 3, 3);
    }
    painter.restore();
    QListView::paintEvent(event);
}