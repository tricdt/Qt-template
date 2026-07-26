#include "qtcentralstackedwidget.h"
#include "qttheme.h"

#include <qapplication.h>
#include <qboxlayout.h>
#include <qgraphicseffect.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>
#include <qtimer.h>

class QtCentralStackedWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtCentralStackedWidget)
public:
    int popupAnimationYOffset;
    qreal scaleAnimationRatio;
    qreal scaleAnimationPixOpacity;
    qreal flipAnimationRatio;
    int blurAnimationRadius;
    int lastTargetIndex;
    QtCentralStackedWidget* q_ptr;
    QtWindowType::StackSwitchMode stackSwitchMode{ QtWindowType::StackSwitchMode::Popup };
    QtThemeType::ThemeMode themeMode;
    QPixmap targetStackPix;
    QPixmap currentStackPix;
    QGraphicsBlurEffect* blurEffect{ nullptr };
    bool isTransparent{ false };
    QVBoxLayout* mainLayout{ nullptr };
    QWidget* customWidget{ nullptr };
    QStackedWidget* containerStackedWidget{ nullptr };
    bool isHasRadius{ true };
    bool isDrawNewPix{ false };

    QtCentralStackedWidgetPrivate(QtCentralStackedWidget* q);
    ~QtCentralStackedWidgetPrivate();

    void getTargetStackPix();
    void getCurrentStackPix();
};

QtCentralStackedWidgetPrivate::QtCentralStackedWidgetPrivate(QtCentralStackedWidget* q)
    : q_ptr(q)
{
}
QtCentralStackedWidgetPrivate::~QtCentralStackedWidgetPrivate()
{
}

void QtCentralStackedWidgetPrivate::getTargetStackPix()
{
    targetStackPix = QPixmap();
    bool isTransparent = isTransparent;
    isTransparent = true;
    targetStackPix = containerStackedWidget->grab();
    this->isTransparent = isTransparent;
}
void QtCentralStackedWidgetPrivate::getCurrentStackPix()
{
    targetStackPix = QPixmap();
    bool isTransparent = isTransparent;
    isTransparent = true;
    containerStackedWidget->currentWidget()->setVisible(true);
    currentStackPix = containerStackedWidget->grab();
    containerStackedWidget->currentWidget()->setVisible(false);
    this->isTransparent = isTransparent;
}
QtCentralStackedWidget::QtCentralStackedWidget(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCentralStackedWidgetPrivate(this))
{
    Q_D(QtCentralStackedWidget);
    d->popupAnimationYOffset = 0;
    d->scaleAnimationRatio = 1;
    d->scaleAnimationPixOpacity = 1;
    d->flipAnimationRatio = 1;
    d->blurAnimationRadius = 0;
    d->lastTargetIndex = 0;

    setObjectName("QtCentralStackedWidget");
    setStyleSheet("#QtCentralStackedWidget{background-color:transparent;}");

    d->containerStackedWidget = new QStackedWidget(this);
    d->containerStackedWidget->setObjectName("QtCentralStackedWidget");
    d->containerStackedWidget->setStyleSheet(
        "#QtCentralStackedWidget{background-color:transparent;}");

    d->blurEffect = new QGraphicsBlurEffect(d->containerStackedWidget);
    d->blurEffect->setBlurHints(QGraphicsBlurEffect::BlurHint::QualityHint);
    d->blurEffect->setBlurRadius(0);
    d->blurEffect->setEnabled(false);
    d->containerStackedWidget->setGraphicsEffect(d->blurEffect);

    d->mainLayout = new QVBoxLayout(this);
    d->mainLayout->setSpacing(0);
    d->mainLayout->setContentsMargins(0, 0, 0, 0);
    d->mainLayout->addWidget(d->containerStackedWidget);

    d->themeMode = qtTheme->getThemeMode();
    connect(
        qtTheme, &QtTheme::themeModeChanged, this, &QtCentralStackedWidget::onThemeModeChanged);
}

QtCentralStackedWidget::~QtCentralStackedWidget()
{
}

QStackedWidget* QtCentralStackedWidget::containerStackedWidget() const
{
    Q_D(const QtCentralStackedWidget);
    return d->containerStackedWidget;
}

void QtCentralStackedWidget::setCustomWidget(QWidget* widget)
{
    Q_D(QtCentralStackedWidget);
    if(!widget)
    {
        return;
    }
    if(d->customWidget)
    {
        d->mainLayout->removeWidget(d->customWidget);
    }
    d->mainLayout->insertWidget(0, widget);
    d->customWidget = widget;
}
QWidget* QtCentralStackedWidget::customWidget() const
{
    Q_D(const QtCentralStackedWidget);
    return d->customWidget;
}

void QtCentralStackedWidget::onThemeModeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_D(QtCentralStackedWidget);
    d->themeMode = themeMode;
}

void QtCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    Q_D(QtCentralStackedWidget);
    d->isTransparent = isTransparent;
    update();
}
bool QtCentralStackedWidget::isTransparent() const
{
    Q_D(const QtCentralStackedWidget);
    return d->isTransparent;
}

void QtCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    Q_D(QtCentralStackedWidget);
    d->isHasRadius = isHasRadius;
    update();
}

void QtCentralStackedWidget::doWindowStackSwitch(
    QtWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack)
{
    Q_D(QtCentralStackedWidget);
    if(d->lastTargetIndex == nodeIndex)
    {
        return;
    }
    d->lastTargetIndex = nodeIndex;
    d->stackSwitchMode = stackSwitchMode;
    switch(stackSwitchMode)
    {
        case QtWindowType::None: {
            d->containerStackedWidget->setCurrentIndex(nodeIndex);
            break;
        }
        case QtWindowType::Popup: {
            QTimer::singleShot(180, this, [this, nodeIndex]() {
                QWidget* targetWidget = d_ptr->containerStackedWidget->widget(nodeIndex);
                d_ptr->containerStackedWidget->setCurrentIndex(nodeIndex);
                d_ptr->getTargetStackPix();
                targetWidget->setVisible(false);
                QPropertyAnimation* popupAnimation =
                    new QPropertyAnimation(this, "popupAnimationYOffset");
                connect(popupAnimation, &QPropertyAnimation::valueChanged, this,
                    [this]() { update(); });
                connect(
                    popupAnimation, &QPropertyAnimation::finished, this, [this, targetWidget]() {
                        d_ptr->targetStackPix = QPixmap();
                        targetWidget->setVisible(true);
                    });
                popupAnimation->setEasingCurve(QEasingCurve::OutCubic);
                popupAnimation->setDuration(300);
                int targetWidgetY = d_ptr->containerStackedWidget->mapToParent(QPoint(0, 0)).y();
                popupAnimation->setEndValue(targetWidgetY);
                targetWidgetY += 80;
                popupAnimation->setStartValue(targetWidgetY);
                popupAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });
            break;
        }
        case QtWindowType::Scale: {
            QWidget* targetWidget = d->containerStackedWidget->widget(nodeIndex);
            d->getCurrentStackPix();
            d->containerStackedWidget->setCurrentIndex(nodeIndex);
            d->getTargetStackPix();
            targetWidget->setVisible(false);
            d->isDrawNewPix = false;
            QPropertyAnimation* currentPixZoomAnimation =
                new QPropertyAnimation(this, "scaleAnimationRatio");
            connect(currentPixZoomAnimation, &QPropertyAnimation::valueChanged, this,
                [this]() { update(); });
            connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this,
                [this, &targetWidget, &isRouteBack]() {
                    d_ptr->isDrawNewPix = true;
                    QPropertyAnimation* targetPixZoomAnimation =
                        new QPropertyAnimation(this, "scaleAnimationRatio");
                    connect(targetPixZoomAnimation, &QPropertyAnimation::valueChanged, this,
                        [this]() { update(); });
                    connect(targetPixZoomAnimation, &QPropertyAnimation::finished, this,
                        [this, &targetWidget]() {
                            d_ptr->targetStackPix = QPixmap();
                            d_ptr->currentStackPix = QPixmap();
                            targetWidget->setVisible(true);
                        });
                    if(isRouteBack)
                    {
                        targetPixZoomAnimation->setStartValue(1.5);
                        targetPixZoomAnimation->setEndValue(1);
                    }
                    else
                    {
                        targetPixZoomAnimation->setStartValue(0.85);
                        targetPixZoomAnimation->setEndValue(1);
                    }
                    targetPixZoomAnimation->setDuration(300);
                    targetPixZoomAnimation->setEasingCurve(QEasingCurve::OutCubic);
                    targetPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                });
            if(isRouteBack)
            {
                currentPixZoomAnimation->setStartValue(1);
                currentPixZoomAnimation->setEndValue(0.85);
            }
            else
            {
                currentPixZoomAnimation->setStartValue(1);
                currentPixZoomAnimation->setEndValue(1.15);
            }
            currentPixZoomAnimation->setDuration(150);
            currentPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* currentPixOpacityAnimation =
                new QPropertyAnimation(this, "scaleAnimationPixOpacity");
            connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
                QPropertyAnimation* targetPixOpacityAnimation =
                    new QPropertyAnimation(this, "scaleAnimationPixOpacity");
                targetPixOpacityAnimation->setStartValue(0);
                targetPixOpacityAnimation->setEndValue(1);
                targetPixOpacityAnimation->setDuration(300);
                targetPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            });
            currentPixOpacityAnimation->setStartValue(1);
            currentPixOpacityAnimation->setEndValue(0);
            currentPixOpacityAnimation->setDuration(150);
            currentPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case QtWindowType::Flip: {
            QWidget* targetWidget = d->containerStackedWidget->widget(nodeIndex);
            d->getCurrentStackPix();
            d->containerStackedWidget->setCurrentIndex(nodeIndex);
            d->getTargetStackPix();
            targetWidget->setVisible(false);
            QPropertyAnimation* flipAnimation = new QPropertyAnimation(this, "flipAnimationRatio");
            connect(flipAnimation, &QPropertyAnimation::valueChanged, this, [this]() { update(); });
            connect(flipAnimation, &QPropertyAnimation::finished, this, [this, &targetWidget]() {
                d_ptr->targetStackPix = QPixmap();
                d_ptr->currentStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            flipAnimation->setEasingCurve(QEasingCurve::InOutSine);
            flipAnimation->setDuration(650);
            flipAnimation->setStartValue(0);
            flipAnimation->setEndValue(isRouteBack ? -180 : 180);
            flipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case QtWindowType::Blur: {
            d->targetStackPix = QPixmap();
            d->blurEffect->setEnabled(true);
            QPropertyAnimation* blurAnimation = new QPropertyAnimation(this, "blurAnimationRadius");
            connect(blurAnimation, &QPropertyAnimation::valueChanged, this,
                [this]() { d_ptr->blurEffect->setBlurRadius(d_ptr->blurAnimationRadius); });
            connect(blurAnimation, &QPropertyAnimation::finished, this,
                [this]() { d_ptr->blurEffect->setEnabled(false); });
            blurAnimation->setEasingCurve(QEasingCurve::InOutSine);
            blurAnimation->setDuration(350);
            blurAnimation->setStartValue(40);
            blurAnimation->setEndValue(2);
            blurAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QApplication::processEvents();
            d->containerStackedWidget->setCurrentIndex(nodeIndex);
            break;
        }
    }
}

int QtCentralStackedWidget::popupAnimationYOffset() const
{
    Q_D(const QtCentralStackedWidget);
    return d->popupAnimationYOffset;
}
void QtCentralStackedWidget::setPopupAnimationYOffset(int value)
{
    Q_D(QtCentralStackedWidget);
    d->popupAnimationYOffset = value;
    Q_EMIT popupAnimationYOffsetChanged(value);
}

qreal QtCentralStackedWidget::scaleAnimationRatio() const
{
    Q_D(const QtCentralStackedWidget);
    return d->scaleAnimationRatio;
}
void QtCentralStackedWidget::setScaleAnimationRatio(qreal value)
{
    Q_D(QtCentralStackedWidget);
    d->scaleAnimationRatio = value;
    Q_EMIT scaleAnimationRatioChanged(value);
}

qreal QtCentralStackedWidget::scaleAnimationPixOpacity() const
{
    Q_D(const QtCentralStackedWidget);
    return d->scaleAnimationPixOpacity;
}
void QtCentralStackedWidget::setScaleAnimationPixOpacity(qreal value)
{
    Q_D(QtCentralStackedWidget);
    d->scaleAnimationPixOpacity = value;
    Q_EMIT scaleAnimationPixOpacityChanged(value);
}

qreal QtCentralStackedWidget::flipAnimationRatio() const
{
    Q_D(const QtCentralStackedWidget);
    return d->flipAnimationRatio;
}
void QtCentralStackedWidget::setFlipAnimationRatio(qreal value)
{
    Q_D(QtCentralStackedWidget);
    d->flipAnimationRatio = value;
    Q_EMIT flipAnimationRatioChanged(value);
}

int QtCentralStackedWidget::blurAnimationRadius() const
{
    Q_D(const QtCentralStackedWidget);
    return d->blurAnimationRadius;
}
void QtCentralStackedWidget::setBlurAnimationRadius(int value)
{
    Q_D(QtCentralStackedWidget);
    d->blurAnimationRadius = value;
    Q_EMIT blurAnimationRadiusChanged(value);
}

int QtCentralStackedWidget::lastTargetIndex() const
{
    Q_D(const QtCentralStackedWidget);
    return d->lastTargetIndex;
}
void QtCentralStackedWidget::setLastTargetIndex(int value)
{
    Q_D(QtCentralStackedWidget);
    d->lastTargetIndex = value;
    Q_EMIT lastTargetIndexChanged(value);
}

void QtCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    Q_D(QtCentralStackedWidget);
    QRect targetRect = this->rect();
    targetRect.adjust(1, 1, 10, 10);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if(!d->isTransparent)
    {
        painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBaseLine), 1.5));
        painter.setBrush(QtThemeColor(d->themeMode, WindowCentralStackBase));
        if(d->isHasRadius)
        {
            painter.drawRoundedRect(targetRect, 10, 10);
        }
        else
        {
            painter.drawRect(targetRect);
        }
    }
    if(!d->targetStackPix.isNull())
    {
        QPoint centralStackPos = d->containerStackedWidget->mapToParent(QPoint(0, 0));
        QRect centralStackRect = QRect(centralStackPos.x(), centralStackPos.y(),
            d->containerStackedWidget->width(), d->containerStackedWidget->height());
        QPainterPath clipPath;
        clipPath.addRoundedRect(centralStackRect, 10, 10);
        painter.setClipPath(clipPath);
        switch(d->stackSwitchMode)
        {
            case QtWindowType::None: {
                break;
            }
            case QtWindowType::Popup: {
                painter.drawPixmap(QRect(0, d->popupAnimationYOffset, width(),
                                       d->containerStackedWidget->height()),
                    d->targetStackPix);
                break;
            }
            case QtWindowType::Scale: {
                painter.setOpacity(d->scaleAnimationPixOpacity);
                painter.translate(d->containerStackedWidget->rect().center());
                painter.scale(d->scaleAnimationRatio, d->scaleAnimationRatio);
                painter.translate(-d->containerStackedWidget->rect().center());
                painter.drawPixmap(
                    centralStackRect, d->isDrawNewPix ? d->targetStackPix : d->currentStackPix);
                break;
            }
            case QtWindowType::Flip: {
                QTransform transform;
                transform.translate(centralStackRect.center().x(), 0);
                if(abs(d->flipAnimationRatio) >= 90)
                {
                    transform.rotate(-180 + d->flipAnimationRatio, Qt::YAxis);
                }
                else
                {
                    transform.rotate(d->flipAnimationRatio, Qt::YAxis);
                }
                transform.translate(-centralStackRect.center().x(), 0);
                painter.setTransform(transform);
                if(abs(d->flipAnimationRatio) >= 90)
                {
                    painter.drawPixmap(centralStackRect, d->targetStackPix);
                }
                else
                {
                    painter.drawPixmap(centralStackRect, d->currentStackPix);
                }
                break;
            }
            case QtWindowType::Blur: {
                break;
            }
        }
    }
    painter.restore();
}