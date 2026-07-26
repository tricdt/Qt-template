#include "qtpromotioncard.h"
#include "qttheme.h"
#include <qcoreevent.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qvariantanimation.h>

class QtPromotionCardPrivate
{
    Q_DECLARE_PUBLIC(QtPromotionCard)
public:
    int borderRadius;
    QPixmap cardPixmap;
    QString cardTitle;
    QString promotionTitle;
    QString title;
    QString subTitle;
    QColor cardTitleColor;
    QColor promotionTitleColor;
    QColor promotionTitleBaseColor;
    QColor titleColor;
    QColor subTitleColor;
    int cardTitlePixelSize;
    int promotionTitlePixelSize;
    int titlePixelSize;
    int subTitlePixelSize;
    qreal horizontalCardPixmapRatio;
    qreal verticalCardPixmapRatio;
    QtPromotionCard* q_ptr;
    qreal pressRadius;
    qreal hoverOpacity;
    qreal pressOpacity;
    int shadowBorderWidth{ 6 };
    QRadialGradient* hoverGradient{ nullptr };
    QRadialGradient* pressGradient{ nullptr };
    bool isPressAnimationFinished{ true };
    QtPromotionCardPrivate(QtPromotionCard* q);
    ~QtPromotionCardPrivate();
    qreal longestDistance(QPoint point);
    qreal distance(QPoint point1, QPoint point2);
    void startHoverOpacityAnimation(bool isVisible);
};

QtPromotionCardPrivate::QtPromotionCardPrivate(QtPromotionCard* q)
    : q_ptr(q)
{
}
QtPromotionCardPrivate::~QtPromotionCardPrivate()
{
}

qreal QtPromotionCardPrivate::longestDistance(QPoint point)
{
    Q_Q(QtPromotionCard);
    qreal topLeftDis = distance(point, QPoint(0, 0));
    qreal topRightDis = distance(point, QPoint(q->width(), 0));
    qreal bottomLeftDis = distance(point, QPoint(0, q->height()));
    qreal bottomRightDis = distance(point, QPoint(q->width(), q->height()));
    QList<qreal> disList{ topLeftDis, topRightDis, bottomLeftDis, bottomRightDis };
    std::sort(disList.begin(), disList.end());
    return disList[3];
}
qreal QtPromotionCardPrivate::distance(QPoint point1, QPoint point2)
{
    Q_Q(QtPromotionCard);
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) +
                     (point1.y() - point2.y()) * (point1.y() - point2.y()));
}
void QtPromotionCardPrivate::startHoverOpacityAnimation(bool isVisible)
{
    Q_Q(QtPromotionCard);
    QVariantAnimation* opacityAnimation = new QVariantAnimation(q);
    QObject::connect(opacityAnimation, &QVariantAnimation::valueChanged, q,
        [this](const QVariant& value) { 
            hoverOpacity = value.toReal();
            q_ptr->update(); });
    opacityAnimation->setDuration(250);
    opacityAnimation->setStartValue(hoverOpacity);
    opacityAnimation->setEndValue(isVisible ? 1.0 : 0.0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

QtPromotionCard::QtPromotionCard(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtPromotionCardPrivate(this))
{
    Q_D(QtPromotionCard);
    d->borderRadius = 5;
    d->pressRadius = 0.0;
    d->hoverOpacity = 0.0;
    d->horizontalCardPixmapRatio = 1;
    d->verticalCardPixmapRatio = 1;
    d->cardTitle = "";
    d->promotionTitle = "";
    d->title = "";
    d->subTitle = "";
    d->cardTitleColor = Qt::white;
    d->promotionTitleColor = Qt::white;
    d->titleColor = Qt::white;
    d->subTitleColor = Qt::white;
    d->promotionTitleBaseColor = QColor(0, 0, 0, 120);
    d->cardTitlePixelSize = 22;
    d->promotionTitlePixelSize = 12;
    d->titlePixelSize = 25;
    d->subTitlePixelSize = 16;
    setMouseTracking(true);
    setObjectName("QtPromotionCard");
    setStyleSheet("#QtPromotionCard{background-color:transparent;}");
    d->hoverGradient = new QRadialGradient();
    d->hoverGradient->setRadius(170);
    d->hoverGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 40));
    d->hoverGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 0));

    d->pressGradient = new QRadialGradient();
    d->pressGradient->setRadius(170);
    d->pressGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 0));
    d->pressGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 40));
}

QtPromotionCard::~QtPromotionCard()
{    Q_D(const QtPromotionCard);
    delete d->hoverGradient;
    delete d->pressGradient;
}

int QtPromotionCard::borderRadius() const
{
    Q_D(const QtPromotionCard);
    return d->borderRadius;
}
void QtPromotionCard::setBorderRadius(int value)
{
    Q_D(QtPromotionCard);
    d->borderRadius = value;
}
QPixmap QtPromotionCard::cardPixmap() const
{
    Q_D(const QtPromotionCard);
    return d->cardPixmap;
}
void QtPromotionCard::setCardPixmap(QPixmap value)
{
    Q_D(QtPromotionCard);
    d->cardPixmap = value;
}
QString QtPromotionCard::cardTitle() const
{
    Q_D(const QtPromotionCard);
    return d->cardTitle;
}
void QtPromotionCard::setCardTitle(QString value)
{
    Q_D(QtPromotionCard);
    d->cardTitle = value;
}
QString QtPromotionCard::promotionTitle() const
{
    Q_D(const QtPromotionCard);
    return d->promotionTitle;
}
void QtPromotionCard::setPromotionTitle(QString value)
{
    Q_D(QtPromotionCard);
    d->promotionTitle = value;
}
QString QtPromotionCard::title() const
{
    Q_D(const QtPromotionCard);
    return d->title;
}
void QtPromotionCard::setTitle(QString value)
{
    Q_D(QtPromotionCard);
    d->title = value;
}
QString QtPromotionCard::subTitle() const
{
    Q_D(const QtPromotionCard);
    return d->subTitle;
}
void QtPromotionCard::setSubTitle(QString value)
{
    Q_D(QtPromotionCard);
    d->subTitle = value;
}
QColor QtPromotionCard::cardTitleColor() const
{
    Q_D(const QtPromotionCard);
    return d->cardTitleColor;
}
void QtPromotionCard::setCardTitleColor(QColor value)
{
    Q_D(QtPromotionCard);
    d->cardTitleColor = value;
}
QColor QtPromotionCard::promotionTitleColor() const
{
    Q_D(const QtPromotionCard);
    return d->promotionTitleColor;
}
void QtPromotionCard::setPromotionTitleColor(QColor value)
{
    Q_D(QtPromotionCard);
    d->promotionTitleColor = value;
}
QColor QtPromotionCard::promotionTitleBaseColor() const
{
    Q_D(const QtPromotionCard);
    return d->promotionTitleBaseColor;
}
void QtPromotionCard::setPromotionTitleBaseColor(QColor value)
{
    Q_D(QtPromotionCard);
    d->promotionTitleBaseColor = value;
}
QColor QtPromotionCard::titleColor() const
{
    Q_D(const QtPromotionCard);
    return d->titleColor;
}
void QtPromotionCard::setTitleColor(QColor value)
{
    Q_D(QtPromotionCard);
    d->titleColor = value;
}
QColor QtPromotionCard::subTitleColor() const
{
    Q_D(const QtPromotionCard);
    return d->subTitleColor;
}
void QtPromotionCard::setSubTitleColor(QColor value)
{
    Q_D(QtPromotionCard);
    d->subTitleColor = value;
}
int QtPromotionCard::cardTitlePixelSize() const
{
    Q_D(const QtPromotionCard);
    return d->cardTitlePixelSize;
}
void QtPromotionCard::setCardTitlePixelSize(int value)
{
    Q_D(QtPromotionCard);
    d->cardTitlePixelSize = value;
}
int QtPromotionCard::promotionTitlePixelSize() const
{
    Q_D(const QtPromotionCard);
    return d->promotionTitlePixelSize;
}
void QtPromotionCard::setPromotionTitlePixelSize(int value)
{
    Q_D(QtPromotionCard);
    d->promotionTitlePixelSize = value;
}
int QtPromotionCard::titlePixelSize() const
{
    Q_D(const QtPromotionCard);
    return d->titlePixelSize;
}
void QtPromotionCard::setTitlePixelSize(int value)
{
    Q_D(QtPromotionCard);
    d->titlePixelSize = value;
}
int QtPromotionCard::subTitlePixelSize() const
{
    Q_D(const QtPromotionCard);
    return d->subTitlePixelSize;
}
void QtPromotionCard::setSubTitlePixelSize(int value)
{
    Q_D(QtPromotionCard);
    d->subTitlePixelSize = value;
}
qreal QtPromotionCard::horizontalCardPixmapRatio() const
{
    Q_D(const QtPromotionCard);
    return d->horizontalCardPixmapRatio;
}
void QtPromotionCard::setHorizontalCardPixmapRatio(qreal value)
{
    Q_D(QtPromotionCard);
    if(value > 0 && value <= 1)
    {
        Q_EMIT horizontalCardPixmapRatioChanged();
        d->horizontalCardPixmapRatio = value;
    }
}
qreal QtPromotionCard::verticalCardPixmapRatio() const
{
    Q_D(const QtPromotionCard);
    return d->verticalCardPixmapRatio;
}
void QtPromotionCard::setVerticalCardPixmapRatio(qreal value)
{
    Q_D(QtPromotionCard);
    if(value > 0 && value <= 1)
    {
        Q_EMIT verticalCardPixmapRatioChanged();
        d->verticalCardPixmapRatio = value;
    }
}

bool QtPromotionCard::event(QEvent* event)
{
    Q_D(QtPromotionCard);
    switch(event->type())
    {
        case QEvent::MouseButtonPress: {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            QVariantAnimation* opacityAnimation = new QVariantAnimation(this);
            connect(opacityAnimation, &QVariantAnimation::valueChanged, this,
                [this](const QVariant& value) {
                    d_ptr->pressOpacity = value.toReal();
                    update();
                });
            connect(opacityAnimation, &QVariantAnimation::finished, this,
                [this]() { d_ptr->isPressAnimationFinished = true; });
            opacityAnimation->setDuration(300);
            opacityAnimation->setEasingCurve(QEasingCurve::InQuad);
            opacityAnimation->setStartValue(1.0);
            opacityAnimation->setEndValue(0.0);
            opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QVariantAnimation* pressAnimation = new QVariantAnimation(this);
            connect(pressAnimation, &QVariantAnimation::valueChanged, this,
                [this](const QVariant& value) {
                    d_ptr->pressRadius = value.toReal();
                    d_ptr->pressGradient->setRadius(value.toReal());
                });
            pressAnimation->setDuration(300);
            pressAnimation->setEasingCurve(QEasingCurve::InQuad);
            pressAnimation->setStartValue(30.0);
            pressAnimation->setEndValue(d->longestDistance(mouseEvent->pos()) * 1.8);
            pressAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            d->isPressAnimationFinished = false;
            d->pressGradient->setFocalPoint(mouseEvent->pos());
            d->pressGradient->setCenter(mouseEvent->pos());
            d->startHoverOpacityAnimation(false);
            break;
        }
        case QEvent::MouseButtonRelease: {
            Q_EMIT promotionCardClicked();
            break;
        }
        case QEvent::MouseMove: {
            QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if(d->hoverOpacity < 1 && d->isPressAnimationFinished)
            {
                d->startHoverOpacityAnimation(true);
            }
            if(d->isPressAnimationFinished)
            {
                d->hoverGradient->setCenter(mouseEvent->pos());
                d->hoverGradient->setFocalPoint(mouseEvent->pos());
            }
            update();
            break;
        }
        case QEvent::Enter: {
            d->startHoverOpacityAnimation(true);
            break;
        }
        case QEvent::Leave: {
            d->startHoverOpacityAnimation(false);
            break;
        }
        default: {
            break;
        }
    }
    return QWidget::event(event);
}

void QtPromotionCard::paintEvent(QPaintEvent* event)
{
    Q_D(QtPromotionCard);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(
        QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);
    qtTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);
    QRect foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth,
        width() - 2 * d->shadowBorderWidth, height() - 2 * d->shadowBorderWidth);
    QPainterPath path;
    path.addRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    painter.setClipPath(path);
    if(!d->cardPixmap.isNull())
    {
        QRect pixSourceRect = d->cardPixmap.rect();
        qreal horizontalOffset = d->cardPixmap.width() * (1 - d->horizontalCardPixmapRatio) / 2;
        qreal verticalOffset = d->cardPixmap.height() * (1 - d->verticalCardPixmapRatio) / 2;
        pixSourceRect.adjust(horizontalOffset, verticalOffset, -horizontalOffset, -verticalOffset);
        painter.drawPixmap(foregroundRect, d->cardPixmap, pixSourceRect);
    }
    painter.save();
    QFont font = painter.font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->cardTitlePixelSize);
    painter.setFont(font);
    painter.setPen(d->cardTitleColor);
    painter.drawText(QRect(25, 25, foregroundRect.width() - 25, foregroundRect.height()),
        Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, d->cardTitle);
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->titlePixelSize);
    painter.setFont(font);
    painter.setPen(d->titleColor);
    int titleTextHeight = painter.fontMetrics().height();
    QRect titleRect(
        25, (height() - titleTextHeight) / 2, foregroundRect.width() - 25, titleTextHeight);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignBottom | Qt::TextSingleLine, d->title);
    if(!d->promotionTitle.isEmpty())
    {
        font.setWeight(QFont::Normal);
        font.setPixelSize(d->promotionTitlePixelSize);
        painter.setFont(font);
        int promotionTitleTextWidth = painter.fontMetrics().horizontalAdvance(d->promotionTitle);
        int promotionTitleTextHeight = painter.fontMetrics().height();
        QRect promotionTitleTextRect(32, titleRect.top() - promotionTitleTextHeight - 5,
            foregroundRect.width() / 2 - 25, promotionTitleTextHeight);
        painter.setPen(Qt::NoPen);
        painter.setBrush(d->promotionTitleBaseColor);
        painter.drawRoundedRect(QRect(25, promotionTitleTextRect.top() - 2,
                                    promotionTitleTextWidth + 14, promotionTitleTextHeight + 4),
            8, 8);
        painter.setPen(d->promotionTitleColor);
        painter.drawText(promotionTitleTextRect,
            Qt::AlignLeft | Qt::AlignBottom | Qt::TextSingleLine, d->promotionTitle);
    }
    font.setWeight(QFont::Medium);
    font.setPixelSize(d->subTitlePixelSize);
    painter.setFont(font);
    painter.setPen(d->subTitleColor);
    painter.drawText(QRect(25, titleRect.bottom(), foregroundRect.width() / 2 - 25, height() / 2),
        Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, d->subTitle);

    painter.restore();
    if(d->isPressAnimationFinished)
    {
        painter.setOpacity(d->hoverOpacity);
        painter.setBrush(*d->hoverGradient);
        painter.drawEllipse(
            d->hoverGradient->center(), d->hoverGradient->radius(), d->hoverGradient->radius());
    }
    else
    {
        painter.setOpacity(d->pressOpacity);
        painter.setBrush(*d->pressGradient);
        painter.drawEllipse(d->pressGradient->center(), d->pressRadius, d->pressRadius / 1.1);
    }
    painter.restore();
}
