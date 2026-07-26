#include "qtpromotionview.h"
#include "qtpromotioncard.h"
#include "qttheme.h"
#include <qevent.h>
#include <qobject.h>
#include <qtimer.h>
#include <qvariant.h>
#include <qvariantanimation.h>
class QtPromotionViewPrivate
{
    Q_DECLARE_PUBLIC(QtPromotionView)
public:
    int cardExpandWidth;
    int cardCollapseWidth;
    int currentIndex;
    bool isAutoScroll;
    int autoScrollInterval;
    QtPromotionView* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QTimer* autoScrollTimer{ nullptr };
    int leftPadding{ 40 };
    int cardSpacing{ 5 };
    int bottomMargin{ 30 };
    int indicatorSpacing{ 12 };
    bool isAllowSwitch{ true };
    QList<QtPromotionCard*> promotionCardList;

    QtPromotionViewPrivate(QtPromotionView* q);
    ~QtPromotionViewPrivate();
    void startCardGeometryAnimation(QtPromotionCard* card, QRect start, QRect end);
    void startHorizontalCardPixmapRatioAnimation(QtPromotionCard* card, qreal start, qreal end);
    void updatePromotionCardGeometry();
    int adjacentIndex(Qt::LayoutDirection direction, int index);
    int rightLimitX();
    void onPromotionCardClicked(QtPromotionCard* clickedCard);
};

QtPromotionViewPrivate::QtPromotionViewPrivate(QtPromotionView* q)
    : q_ptr(q)
{
}
QtPromotionViewPrivate::~QtPromotionViewPrivate()
{
}

void QtPromotionViewPrivate::startCardGeometryAnimation(
    QtPromotionCard* card, QRect start, QRect end)
{
    Q_Q(QtPromotionView);
    QVariantAnimation* geometryAnimation = new QVariantAnimation(card);
    QObject::connect(geometryAnimation, &QVariantAnimation::valueChanged, card,
        [card](const QVariant& value) { card->setGeometry(value.toRect()); });
    geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
    geometryAnimation->setDuration(650);
    geometryAnimation->setStartValue(start);
    geometryAnimation->setEndValue(end);
    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtPromotionViewPrivate::startHorizontalCardPixmapRatioAnimation(
    QtPromotionCard* card, qreal start, qreal end)
{
    Q_Q(QtPromotionView);
    QVariantAnimation* ratioAnimation = new QVariantAnimation(card);
    QObject::connect(ratioAnimation, &QVariantAnimation::valueChanged, card,
        [card](const QVariant& value) { card->setHorizontalCardPixmapRatio(value.toReal()); });
    ratioAnimation->setEasingCurve(QEasingCurve::OutCubic);
    ratioAnimation->setDuration(650);
    ratioAnimation->setStartValue(start);
    ratioAnimation->setEndValue(end);
    ratioAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtPromotionViewPrivate::updatePromotionCardGeometry()
{
    Q_Q(QtPromotionView);
    if(promotionCardList.count() < 3)
    {
        if(promotionCardList.count() == 1)
        {
            promotionCardList[0]->setGeometry(
                leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin);
        }
        else
        {
            promotionCardList[0]->setGeometry(
                leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin);
            QRect lastGeometry = promotionCardList[0]->geometry();
            promotionCardList[1]->setGeometry(QRect(lastGeometry.right() + cardSpacing,
                lastGeometry.y(), cardCollapseWidth, lastGeometry.height()));
            promotionCardList[1]->setHorizontalCardPixmapRatio(0.5);
        }
    }
    else
    {
        for(int i = 0; i < promotionCardList.count(); i++)
        {
            if(i == 0)
            {
                promotionCardList[i]->setGeometry(
                    leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin);
                promotionCardList[i]->setHorizontalCardPixmapRatio(1);
            }
            else if(i == promotionCardList.count() - 1)
            {
                promotionCardList[i]->setGeometry(-cardCollapseWidth + leftPadding, 0,
                    cardCollapseWidth, q->height() - bottomMargin);
                promotionCardList[i]->setHorizontalCardPixmapRatio(0.5);
            }
            else
            {
                QRect lastGeometry = promotionCardList[i - 1]->geometry();
                promotionCardList[i]->setGeometry(QRect(lastGeometry.right() + cardSpacing,
                    lastGeometry.y(), cardCollapseWidth, lastGeometry.height()));
                promotionCardList[i]->setHorizontalCardPixmapRatio(0.5);
            }
        }
    }
}
int QtPromotionViewPrivate::adjacentIndex(Qt::LayoutDirection direction, int index)
{
    Q_Q(QtPromotionView);
    if(direction == Qt::LeftToRight)
    {
        if(index == promotionCardList.count() - 1)
        {
            return 0;
        }
        return index + 1;
    }
    else
    {
        if(index > 0)
        {
            return index - 1;
        }
        return promotionCardList.count() - 1;
    }
}
int QtPromotionViewPrivate::rightLimitX()
{
    Q_Q(QtPromotionView);
    int count = promotionCardList.count();
    return leftPadding + cardExpandWidth + cardSpacing * (count - 1) +
           cardCollapseWidth * (count - 3);
}

void QtPromotionViewPrivate::onPromotionCardClicked(QtPromotionCard* clickedCard)
{
    Q_Q(QtPromotionView);
    int newCurrentIndex = promotionCardList.indexOf(clickedCard);
    int oldCurrentIndex = currentIndex;
    if(newCurrentIndex == oldCurrentIndex)
    {
        return;
    }
    bool isRightToLeft =
        promotionCardList[oldCurrentIndex]->x() < promotionCardList[newCurrentIndex]->x();
    int originIndex = adjacentIndex(Qt::RightToLeft, newCurrentIndex);
    for(int i = 0; i < promotionCardList.count(); i++)
    {
        QtPromotionCard* card = promotionCardList[originIndex];
        QVariantAnimation* geometryAnimation = new QVariantAnimation(card);
        QObject::connect(geometryAnimation, &QVariantAnimation::valueChanged, card,
            [card](const QVariant& value) { card->setGeometry(value.toRect()); });
        QRect geometry = card->geometry();
        geometryAnimation->setEasingCurve(QEasingCurve::OutCubic);
        geometryAnimation->setDuration(650);
        geometryAnimation->setStartValue(geometry);
        if(i == 0)
        {
            QRect targetGeometry(
                -cardCollapseWidth + leftPadding, 0, cardCollapseWidth, q->height() - bottomMargin);
            if(promotionCardList.count() > 2)
            {
                if(!isRightToLeft)
                {
                    geometryAnimation->setKeyValueAt(
                        0.7, QRect(geometry.x() + cardCollapseWidth * 0.7 + cardSpacing, 0,
                                 cardCollapseWidth, q->height() - bottomMargin));
                    geometryAnimation->setKeyValueAt(
                        0.71, QRect(-cardCollapseWidth, 0, cardCollapseWidth,
                                  q->height() - bottomMargin));
                }
                geometryAnimation->setEndValue(targetGeometry);
            }
            else
            {
                if(isRightToLeft)
                {
                    geometryAnimation->setEndValue(targetGeometry);
                }
                else
                {
                    geometryAnimation->setEndValue(
                        QRect(leftPadding + cardExpandWidth + cardSpacing * 2, 0, cardCollapseWidth,
                            q->height() - bottomMargin));
                }
            }
            startHorizontalCardPixmapRatioAnimation(card, card->horizontalCardPixmapRatio(), 0.5);
        }
        else if(i == 1)
        {
            geometryAnimation->setEndValue(
                QRect(leftPadding + cardSpacing, 0, cardExpandWidth, q->height() - bottomMargin));
            startHorizontalCardPixmapRatioAnimation(card, card->horizontalCardPixmapRatio(), 1);
        }
        else
        {
            QRect targetGeometry(
                leftPadding + cardExpandWidth + cardSpacing * i + cardCollapseWidth * (i - 2), 0,
                cardCollapseWidth, q->height() - bottomMargin);
            if(isRightToLeft && targetGeometry.x() > card->x())
            {
                QObject::connect(geometryAnimation, &QVariantAnimation::valueChanged, card,
                    [this, card, geometryAnimation, originIndex, targetGeometry]() {
                        if(card->geometry().right() <= 0)
                        {
                            geometryAnimation->pause();
                            qreal timeRatio = geometryAnimation->currentTime() / 650.0;
                            geometryAnimation->setKeyValueAt(timeRatio,
                                QRect(promotionCardList[adjacentIndex(Qt::RightToLeft, originIndex)]
                                              ->geometry()
                                              .right() +
                                          cardCollapseWidth * (1 - timeRatio),
                                    0, cardCollapseWidth, q_ptr->height() - bottomMargin));
                            geometryAnimation->setEndValue(targetGeometry);
                            geometryAnimation->resume();
                        }
                    });
                geometryAnimation->setEndValue(QRect(-cardCollapseWidth * 2 + leftPadding, 0,
                    cardCollapseWidth, q->height() - bottomMargin));
            }
            else
            {
                geometryAnimation->setEndValue(targetGeometry);
            }
            card->setHorizontalCardPixmapRatio(0.5);
        }
        originIndex = adjacentIndex(Qt::LeftToRight, originIndex);
        geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    currentIndex = newCurrentIndex;
    q->update();
    Q_EMIT q->currentIndexChanged();
}

QtPromotionView::QtPromotionView(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtPromotionViewPrivate(this))
{
    Q_D(QtPromotionView);
    d->currentIndex = 0;
    d->cardExpandWidth = 600;
    d->cardCollapseWidth = 300;
    d->isAutoScroll = false;
    d->autoScrollInterval = 5000;
    setFixedHeight(300);
    setObjectName("ElaPromotionView");
    setStyleSheet("#ElaPromotionView{background-color:transparent;}");

    d->autoScrollTimer = new QTimer(this);
    connect(d->autoScrollTimer, &QTimer::timeout, this, [this]() {
        if(isVisible() && d_ptr->promotionCardList.count() > 2)
        {
            d_ptr->onPromotionCardClicked(d_ptr->promotionCardList[d_ptr->adjacentIndex(
                Qt::LeftToRight, d_ptr->currentIndex)]);
        }
    });

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtPromotionView::~QtPromotionView()
{
    Q_D(QtPromotionView);
}

void QtPromotionView::appendPromotionCard(QtPromotionCard* card)
{
    Q_D(QtPromotionView);
    if(!card || d->promotionCardList.contains(card))
    {
        return;
    }
    card->setMinimumSize(0, 0);
    card->setMaximumSize(10000, 10000);
    card->setParent(this);
    d->promotionCardList.append(card);
    connect(card, &QtPromotionCard::promotionCardClicked, this,
        [this, card]() { d_ptr->onPromotionCardClicked(card); });
    d->updatePromotionCardGeometry();
}

int QtPromotionView::cardExpandWidth() const
{
    Q_D(const QtPromotionView);
    return d->cardExpandWidth;
}
void QtPromotionView::setCardExpandWidth(int value)
{
    Q_D(QtPromotionView);
    if(value <= 0)
    {
        return;
    }
    d->cardExpandWidth = value;
    d->updatePromotionCardGeometry();
}
int QtPromotionView::cardCollapseWidth() const
{
    Q_D(const QtPromotionView);
    return d->cardCollapseWidth;
}
void QtPromotionView::setCardCollapseWidth(int value)
{
    Q_D(QtPromotionView);
    if(value <= 0)
    {
        return;
    }
    d->cardCollapseWidth = value;
    d->updatePromotionCardGeometry();
}
int QtPromotionView::currentIndex() const
{
    Q_D(const QtPromotionView);
    return d->currentIndex;
}
void QtPromotionView::setCurrentIndex(int value)
{
    Q_D(QtPromotionView);
    if(value < 0 || value >= d->promotionCardList.count())
    {
        return;
    }
    d->onPromotionCardClicked(d->promotionCardList[value]);
}
bool QtPromotionView::isAutoScroll() const
{
    Q_D(const QtPromotionView);
    return d->isAutoScroll;
}
void QtPromotionView::setIsAutoScroll(bool value)
{
    Q_D(QtPromotionView);
    if(value)
    {
        d->autoScrollTimer->start(d->autoScrollInterval);
    }
    else
    {
        d->autoScrollTimer->stop();
    }
    d->isAutoScroll = value;
    Q_EMIT isAutoScrollChanged();
}
int QtPromotionView::autoScrollInterval() const
{
    Q_D(const QtPromotionView);
    return d->autoScrollInterval;
}
void QtPromotionView::setAutoScrollInterval(int value)
{
    Q_D(QtPromotionView);
    if(value < 400)
    {
        return;
    }
    d->autoScrollInterval = value;
    Q_EMIT autoScrollIntervalChanged();
}

void QtPromotionView::wheelEvent(QWheelEvent* event)
{
    Q_D(QtPromotionView);
    if(d->isAllowSwitch)
    {
        if(event->angleDelta().y() > 0)
        {
            d->isAllowSwitch = false;
            QTimer::singleShot(400, this, [this] { d_ptr->isAllowSwitch = true; });
            d->onPromotionCardClicked(
                d->promotionCardList[d->adjacentIndex(Qt::RightToLeft, d->currentIndex)]);
        }
        else
        {
            d->isAllowSwitch = false;
            QTimer::singleShot(400, this, [=] { d->isAllowSwitch = true; });
            d->onPromotionCardClicked(
                d->promotionCardList[d->adjacentIndex(Qt::LeftToRight, d->currentIndex)]);
        }
    }
    event->accept();
}

void QtPromotionView::paintEvent(QPaintEvent* event)
{
    Q_D(QtPromotionView);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, BasicIndicator));
    int promotionCardCount = d->promotionCardList.count();
    bool isCountOdd = promotionCardCount % 2;
    QPoint startPoint = isCountOdd
                            ? QPoint(width() / 2 - promotionCardCount / 2 * d->indicatorSpacing,
                                  height() - d->bottomMargin / 2)
                            : QPoint(width() / 2 - promotionCardCount / 2 * d->indicatorSpacing -
                                         d->indicatorSpacing / 2,
                                  height() - d->bottomMargin / 2);
    for(int i = 0; i < promotionCardCount; i++)
    {
        if(i == d->currentIndex)
        {
            painter.drawEllipse(
                QPointF(startPoint.x() + i * d->indicatorSpacing, startPoint.y()), 3.5, 3.5);
        }
        else
        {
            painter.drawEllipse(
                QPointF(startPoint.x() + i * d->indicatorSpacing, startPoint.y()), 2.5, 2.5);
        }
    }
    painter.restore();
}
