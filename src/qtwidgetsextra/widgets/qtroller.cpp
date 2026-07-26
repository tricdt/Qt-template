#include "qtroller.h"
#include "qttheme.h"
#include <QVariantAnimation.h>
#include <qevent.h>
#include <qpainterpath.h>
#include <qtimer.h>
#include <qvariant.h>
#include <qvariantanimation.h>

class QtRollerPrivate
{
    Q_DECLARE_PUBLIC(QtRoller)
public:
    int borderRadius;
    QStringList itemList;
    int itemHeight;
    int maxVisibleItems;
    int currentIndex;
    bool isContainer;
    bool isEnableLoop;
    QtRoller* q_ptr;
    qreal scrollOffset;
    QtThemeType::ThemeMode themeMode;
    QVariantAnimation* scrollAnimation{ nullptr };
    int penBorderWidth{ 1 };
    int indicatorMargin{ 3 };
    bool isUpArrowPress{ false };
    bool isDownArrowPress{ false };
    QRect upArrowRect;
    QRect downArrowRect;
    QTimer* pressSustainTimer{ nullptr };
    QTimer* repeatScrollTimer{ nullptr };
    qreal targetScrollOffset{ 0.0 };
    QPoint mousePoint;
    QtRollerPrivate(QtRoller* q);
    ~QtRollerPrivate();
    void scroll(int delta);
};

QtRollerPrivate::QtRollerPrivate(QtRoller* q)
    : q_ptr(q)
{
}
QtRollerPrivate::~QtRollerPrivate()
{
}

void QtRollerPrivate::scroll(int delta)
{
    int steps = delta / 120;
    targetScrollOffset -= steps * itemHeight;
    targetScrollOffset = qRound(targetScrollOffset / itemHeight) * itemHeight;
    if(!isEnableLoop)
    {
        if(targetScrollOffset < 0)
        {
            targetScrollOffset = 0.0;
        }
        if(targetScrollOffset > (itemList.size() - 1) * itemHeight)
        {
            targetScrollOffset = (itemList.size() - 1) * itemHeight;
        }
    }
    scrollAnimation->stop();
    scrollAnimation->setStartValue(scrollOffset);
    scrollAnimation->setEndValue(targetScrollOffset);
    scrollAnimation->start();
}

QtRoller::QtRoller(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtRollerPrivate(this))
{
    Q_D(QtRoller);
    d->scrollOffset = 0;
    d->itemHeight = 35;
    d->borderRadius = 3;
    d->maxVisibleItems = 5;
    d->currentIndex = 0;
    d->isEnableLoop = true;
    d->isContainer = false;
    setFixedWidth(90);
    setFixedSize(90, 175);
    setMouseTracking(true);
    setObjectName("QtRoller");
    setStyleSheet("#QtRoller{background-color:transparent;}");
    QFont font = this->font();
    font.setPixelSize(16);
    setFont(font);

    d->scrollAnimation = new QVariantAnimation(this);
    connect(
        d->scrollAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
            d_ptr->scrollOffset = value.toReal();
            update();
        });
    connect(d->scrollAnimation, &QVariantAnimation::finished, this, [this]() {
        while(d_ptr->scrollOffset < -d_ptr->itemList.size() * d_ptr->itemHeight)
        {
            d_ptr->scrollOffset += d_ptr->itemList.size() * d_ptr->itemHeight;
        }
        while(d_ptr->scrollOffset >= d_ptr->itemList.size() * d_ptr->itemHeight)
        {
            d_ptr->scrollOffset -= d_ptr->itemList.size() * d_ptr->itemHeight;
        }
        d_ptr->targetScrollOffset = d_ptr->scrollOffset;
        int currentIndex = 0;
        if(d_ptr->scrollOffset >= 0)
        {
            currentIndex = d_ptr->scrollOffset / d_ptr->itemHeight;
        }
        else if(d_ptr->scrollOffset <= -d_ptr->itemHeight)
        {
            currentIndex = d_ptr->itemList.count() + d_ptr->scrollOffset / d_ptr->itemHeight;
        }
        if(currentIndex != d_ptr->currentIndex)
        {
            d_ptr->currentIndex = currentIndex;
            Q_EMIT currentIndexChanged();
            Q_EMIT currentDataChanged(currentData());
        }
        update();
    });

    d->scrollAnimation->setDuration(300);
    d->scrollAnimation->setEasingCurve(QEasingCurve::OutCubic);

    d->pressSustainTimer = new QTimer(this);
    d->pressSustainTimer->setInterval(300);
    connect(d->pressSustainTimer, &QTimer::timeout, this,
        [this]() { d_ptr->repeatScrollTimer->start(); });

    d->repeatScrollTimer = new QTimer(this);
    d->repeatScrollTimer->setInterval(50);
    connect(d->repeatScrollTimer, &QTimer::timeout, this, [this]() {
        if(d_ptr->isUpArrowPress)
        {
            d_ptr->scroll(120);
        }
        if(d_ptr->isDownArrowPress)
        {
            d_ptr->scroll(-120);
        }
    });

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtRoller::~QtRoller()
{
}

void QtRoller::wheelEvent(QWheelEvent* event)
{
    Q_D(QtRoller);
    d->scroll(event->angleDelta().y());
    event->accept();
}

void QtRoller::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtRoller);
    if(d->isContainer && d->maxVisibleItems >= 5)
    {
        auto currentPos = event->pos();
        if(d->upArrowRect.contains(currentPos))
        {
            d->isUpArrowPress = true;
            d->scroll(120);
            d->pressSustainTimer->start();
        }
        else if(d->downArrowRect.contains(currentPos))
        {
            d->isDownArrowPress = true;
            d->scroll(-120);
            d->pressSustainTimer->start();
        }
    }
    QWidget::mousePressEvent(event);
}

void QtRoller::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtRoller);
    int centerIndex = d->maxVisibleItems / 2;
    int clickIndex = event->pos().y() / d->itemHeight;
    int jumpCount = abs(d->maxVisibleItems / 2 - clickIndex);
    if(d->isUpArrowPress || d->isDownArrowPress)
    {
        d->pressSustainTimer->stop();
        d->repeatScrollTimer->stop();
        d->isUpArrowPress = false;
        d->isDownArrowPress = false;
        update();
    }
    else
    {
        if(clickIndex > centerIndex)
        {
            for(int i = 0; i < jumpCount; i++)
            {
                d->scroll(-120);
            }
        }
        else if(clickIndex < centerIndex)
        {
            for(int i = 0; i < jumpCount; i++)
            {
                d->scroll(120);
            }
        }
    }
    update();
}

void QtRoller::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtRoller);
    d->mousePoint = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void QtRoller::leaveEvent(QEvent* event)
{
    Q_D(QtRoller);
    d->mousePoint = QPoint();
    update();
    QWidget::leaveEvent(event);
}

void QtRoller::paintEvent(QPaintEvent* event)
{
    Q_D(QtRoller);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRect foregroundRect(d->penBorderWidth, d->penBorderWidth, width() - 2 * d->penBorderWidth,
        height() - 2 * d->penBorderWidth - 1);
    if(!d->isContainer)
    {
        painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), d->penBorderWidth));
        painter.setBrush(isEnabled() ? QtThemeColor(d->themeMode, BasicBase)
                                     : QtThemeColor(d->themeMode, BasicDisable));
        painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    }
    painter.setClipRect(foregroundRect);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
    bool isEven = d->maxVisibleItems % 2 == 0;
    if(isEven)
    {
        painter.drawRoundedRect(
            QRect(d->indicatorMargin, d->itemHeight * d->maxVisibleItems / 2 + d->indicatorMargin,
                width() - 2 * d->indicatorMargin, d->itemHeight - 2 * d->indicatorMargin),
            d->borderRadius, d->borderRadius);
    }
    else
    {
        painter.drawRoundedRect(
            QRect(d->indicatorMargin, (height() - d->itemHeight) / 2 + d->indicatorMargin,
                width() - 2 * d->indicatorMargin, d->itemHeight - 2 * d->indicatorMargin),
            d->borderRadius, d->borderRadius);
    }

    int yStart = -d->itemHeight;
    int yEnd = height();
    int centerYStart =
        isEven ? d->itemHeight * d->maxVisibleItems / 2 : height() / 2 - d->itemHeight;
    int centerYEnd =
        isEven ? d->itemHeight * d->maxVisibleItems / 2 + d->itemHeight / 2 : height() / 2;
    int startIndexOffset = d->maxVisibleItems / 2;
    for(int i = 0; i < d->itemList.size(); i++)
    {
        qreal y = (i + startIndexOffset) * d->itemHeight - d->scrollOffset;
        if(d->isEnableLoop)
        {
            while(y <= yStart)
            {
                y += d->itemList.size() * d->itemHeight;
            }
            while(y >= yEnd)
            {
                y -= d->itemList.size() * d->itemHeight;
            }
        }
        if(y >= yStart && y <= yEnd)
        {
            painter.save();
            painter.translate(0, y);
            if(y >= centerYStart && y <= centerYEnd)
            {
                painter.setPen(QtThemeColor(d->themeMode, BasicTextInvert));
            }
            else
            {
                if(!d->mousePoint.isNull() &&
                    d->scrollAnimation->state() == QVariantAnimation::State::Stopped)
                {
                    QRectF itemRect = QRectF(0, y, width(), d->itemHeight);
                    if(itemRect.contains(d->mousePoint))
                    {
                        painter.setBrush(QtThemeColor(d->themeMode, BasicHover));
                        painter.drawRoundedRect(QRect(d->indicatorMargin, d->indicatorMargin,
                                                    width() - 2 * d->indicatorMargin,
                                                    d->itemHeight - 2 * d->indicatorMargin),
                            d->borderRadius, d->borderRadius);
                    }
                }
                painter.setPen(QtThemeColor(d->themeMode, BasicText));
            }
            painter.drawText(QRect(0, 0, width(), d->itemHeight),
                Qt::AlignCenter | Qt::TextSingleLine, d->itemList[i]);
            painter.restore();
        }
    }
    if(d->isContainer && d->maxVisibleItems >= 5 && underMouse())
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QtThemeColor(d->themeMode, PopupBase));
        d->upArrowRect =
            QRect(foregroundRect.x(), foregroundRect.y(), foregroundRect.width(), d->itemHeight);
        d->downArrowRect = QRect(foregroundRect.x(), foregroundRect.bottom() - d->itemHeight + 2,
            foregroundRect.width(), d->itemHeight);
        painter.drawRect(d->upArrowRect);
        painter.drawRect(d->downArrowRect);
        painter.setBrush(QtThemeColor(d->themeMode, BasicText));
        int upArrowWidth = d->isUpArrowPress ? 8 : 10; // 边长
        qreal upAdjacentEdge = upArrowWidth * qCos(30 * M_PI / 180.0);
        QPainterPath upArrowPath;
        upArrowPath.moveTo(
            d->upArrowRect.center().x(), d->upArrowRect.center().y() - upAdjacentEdge / 2);
        upArrowPath.lineTo(d->upArrowRect.center().x() + upArrowWidth / (qreal) 2,
            d->upArrowRect.center().y() + upAdjacentEdge / 2);
        upArrowPath.lineTo(d->upArrowRect.center().x() - upArrowWidth / (qreal) 2,
            d->upArrowRect.center().y() + upAdjacentEdge / 2);
        upArrowPath.closeSubpath();
        painter.drawPath(upArrowPath);
        int downArrowWidth = d->isDownArrowPress ? 8 : 10;
        qreal downAdjacentEdge = downArrowWidth * qCos(30 * M_PI / 180.0);
        QPainterPath downArrowPath;
        downArrowPath.moveTo(
            d->downArrowRect.center().x(), d->downArrowRect.center().y() + downAdjacentEdge / 2);
        downArrowPath.lineTo(d->downArrowRect.center().x() + downArrowWidth / (qreal) 2,
            d->downArrowRect.center().y() - downAdjacentEdge / 2);
        downArrowPath.lineTo(d->downArrowRect.center().x() - downArrowWidth / (qreal) 2,
            d->downArrowRect.center().y() - downAdjacentEdge / 2);
        downArrowPath.closeSubpath();
        painter.drawPath(downArrowPath);
    }
    painter.restore();
}

int QtRoller::borderRadius() const
{
    Q_D(const QtRoller);
    return d->borderRadius;
}
void QtRoller::setBorderRadius(int value)
{
    Q_D(QtRoller);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}

void QtRoller::setCurrentData(const QString& data)
{
    Q_D(QtRoller);
    if(d->itemList.contains(data))
    {
        setCurrentIndex(d->itemList.indexOf(data));
    }
}

QString QtRoller::currentData() const
{
    Q_D(const QtRoller);
    if(d->currentIndex >= d->itemList.count())
    {
        return {};
    }
    return d->itemList[d->currentIndex];
}

void QtRoller::setItemList(QStringList itemList)
{
    Q_D(QtRoller);
    d->itemList = std::move(itemList);
    update();
    Q_EMIT itemListChanged();
}

QStringList QtRoller::itemList() const
{
    Q_D(const QtRoller);
    return d->itemList;
}

void QtRoller::setItemHeight(int itemHeight)
{
    Q_D(QtRoller);
    d->itemHeight = itemHeight;
    setFixedHeight(d->maxVisibleItems * d->itemHeight);
    update();
    Q_EMIT itemHeightChanged();
}

int QtRoller::itemHeight() const
{
    Q_D(const QtRoller);
    return d->itemHeight;
}

void QtRoller::setMaxVisibleItems(int maxVisibleItems)
{
    Q_D(QtRoller);
    d->maxVisibleItems = maxVisibleItems;
    setFixedHeight(d->maxVisibleItems * d->itemHeight);
    update();
    Q_EMIT maxVisibleItemsChanged();
}

int QtRoller::maxVisibleItems() const
{
    Q_D(const QtRoller);
    return d->maxVisibleItems;
}

void QtRoller::setCurrentIndex(int currentIndex)
{
    Q_D(QtRoller);
    if(currentIndex >= d->itemList.count())
    {
        return;
    }
    d->currentIndex = currentIndex;
    d->scrollOffset = d->itemHeight * currentIndex;
    d->targetScrollOffset = d->scrollOffset;
    update();
}

int QtRoller::currentIndex() const
{
    Q_D(const QtRoller);
    return d->currentIndex;
}

bool QtRoller::isContainer() const
{
    Q_D(const QtRoller);
    return d->isContainer;
}
void QtRoller::setIsContainer(bool value)
{
    Q_D(QtRoller);
    d->isContainer = value;
    Q_EMIT isContainerChanged();
}
bool QtRoller::isEnableLoop() const
{
    Q_D(const QtRoller);
    return d->isEnableLoop;
}
void QtRoller::setIsEnableLoop(bool value)
{
    Q_D(QtRoller);
    d->isEnableLoop = value;
    Q_EMIT isEnableLoopChanged();
}
