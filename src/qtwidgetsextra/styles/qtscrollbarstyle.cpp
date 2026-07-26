#include "qtscrollbarstyle.h"
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>
#include "qttheme.h"
class QtScrollBarStylePrivate
{
    Q_DECLARE_PUBLIC(QtScrollBarStyle)
public:
    bool isExpand;
    QtScrollBar* scrollBar;
    qreal sliderExtent;
    qreal opacity;
    qreal sliderMargin{ 2.5 };
    int scrollBarExtent{ 12 };
    QtThemeType::ThemeMode themeMode;
    QtScrollBarStyle* q_ptr;
    QtScrollBarStylePrivate(QtScrollBarStyle* q)
        : isExpand(false)
        , scrollBar(nullptr)
        , sliderExtent(0.5)
        , opacity(0)
        , q_ptr(q)
    {
    }
};

QtScrollBarStyle::QtScrollBarStyle(QStyle*)
    : d_ptr(new QtScrollBarStylePrivate(this))
{
    Q_D(QtScrollBarStyle);
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [=](QtThemeType::ThemeMode themeMode) { d->themeMode = themeMode; });
}

QtScrollBarStyle::~QtScrollBarStyle()
{
}

bool QtScrollBarStyle::isExpand() const
{
    Q_D(const QtScrollBarStyle);
    return d->isExpand;
}

void QtScrollBarStyle::setIsExpand(bool value)
{
    Q_D(QtScrollBarStyle);
    if(d->isExpand == value)
        return;
    d->isExpand = value;
    Q_EMIT isExpandChanged();
}

QtScrollBar* QtScrollBarStyle::scrollBar() const
{
    Q_D(const QtScrollBarStyle);
    return d->scrollBar;
}

void QtScrollBarStyle::setScrollBar(QtScrollBar* value)
{
    Q_D(QtScrollBarStyle);
    if(d->scrollBar == value)
        return;
    d->scrollBar = value;
    Q_EMIT scrollBarChanged();
}

qreal QtScrollBarStyle::sliderExtent() const
{
    Q_D(const QtScrollBarStyle);
    return d->sliderExtent;
}

void QtScrollBarStyle::setSliderExtent(qreal value)
{
    Q_D(QtScrollBarStyle);
    if(d->sliderExtent == value)
        return;
    d->sliderExtent = value;
    Q_EMIT sliderExtentChanged();
}

qreal QtScrollBarStyle::opacity() const
{
    Q_D(const QtScrollBarStyle);
    return d->opacity;
}

void QtScrollBarStyle::setOpacity(qreal value)
{
    Q_D(QtScrollBarStyle);
    if(d->opacity == value)
        return;
    d->opacity = value;
    Q_EMIT opacityChanged();
}

void QtScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtScrollBarStyle);
    // QStyle::SC_ScrollBarGroove QStyle::SC_ScrollBarAddLine   QStyle::SC_ScrollBarSubLine上指示器
    switch(control)
    {
        case QStyle::CC_ScrollBar: {
            if(const QStyleOptionSlider* sopt =
                    qstyleoption_cast<const QStyleOptionSlider*>(option))
            {
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing);
                painter->setPen(Qt::NoPen);
                QRect scrollBarRect = sopt->rect;
                if(d->isExpand)
                {
                    // 背景绘制
                    painter->setOpacity(d->opacity);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, BasicBase));
                    painter->drawRoundedRect(scrollBarRect, 6, 6);
                    // 指示器绘制 center()在此处不适用 存在外围边距宽度 需手动计算
                    int sideLength = 8;
                    painter->setBrush(QtThemeColor(d->themeMode, ScrollBarHandle));
                    if(sopt->orientation == Qt::Horizontal)
                    {
                        QRect leftIndicatorRect =
                            subControlRect(control, sopt, QStyle::SC_ScrollBarSubLine, widget);
                        QRect rightIndicatorRect =
                            subControlRect(control, sopt, QStyle::SC_ScrollBarAddLine, widget);
                        // 左三角
                        qreal centerLeftX = leftIndicatorRect.x() + leftIndicatorRect.width() / 2;
                        qreal centerRightX =
                            rightIndicatorRect.x() + rightIndicatorRect.width() / 2;
                        qreal centerY = leftIndicatorRect.y() + leftIndicatorRect.height() / 2;
                        QPainterPath leftPath;
                        leftPath.moveTo(
                            centerLeftX - qCos(30 * M_PI / 180.0) * sideLength / 2, centerY);
                        leftPath.lineTo(centerLeftX + qCos(30 * M_PI / 180.0) * sideLength / 2,
                            centerY + sideLength / 2);
                        leftPath.lineTo(centerLeftX + qCos(30 * M_PI / 180.0) * sideLength / 2,
                            centerY - sideLength / 2);
                        leftPath.closeSubpath();
                        painter->drawPath(leftPath);

                        // 右三角
                        QPainterPath rightPath;
                        rightPath.moveTo(
                            centerRightX + qCos(30 * M_PI / 180.0) * sideLength / 2, centerY);
                        rightPath.lineTo(centerRightX - qCos(30 * M_PI / 180.0) * sideLength / 2,
                            centerY + sideLength / 2);
                        rightPath.lineTo(centerRightX - qCos(30 * M_PI / 180.0) * sideLength / 2,
                            centerY - sideLength / 2);
                        rightPath.closeSubpath();
                        painter->drawPath(rightPath);
                    }
                    else
                    {
                        QRect upIndicatorRect =
                            subControlRect(control, sopt, QStyle::SC_ScrollBarSubLine, widget);
                        QRect downIndicatorRect =
                            subControlRect(control, sopt, QStyle::SC_ScrollBarAddLine, widget);
                        qreal centerToTop = (sideLength / 2) / qCos(30 * M_PI / 180.0);
                        qreal centerToBottom = (sideLength / 2) * qTan(30 * M_PI / 180.0);
                        // 上三角
                        qreal centerX = upIndicatorRect.x() + upIndicatorRect.width() / 2.0;
                        qreal centerUpY = upIndicatorRect.center().y() + 2;
                        qreal centerDownY = downIndicatorRect.center().y() + 2;
                        QPainterPath upPath;
                        upPath.moveTo(centerX, centerUpY - centerToTop);
                        upPath.lineTo(centerX + sideLength / 2, centerUpY + centerToBottom);
                        upPath.lineTo(centerX - sideLength / 2, centerUpY + centerToBottom);
                        upPath.closeSubpath();
                        painter->drawPath(upPath);

                        // 下三角
                        QPainterPath downPath;
                        downPath.moveTo(centerX, centerDownY + centerToBottom);
                        downPath.lineTo(centerX + sideLength / 2, centerDownY - centerToTop);
                        downPath.lineTo(centerX - sideLength / 2, centerDownY - centerToTop);
                        downPath.closeSubpath();
                        painter->drawPath(downPath);
                    }
                }
                painter->setOpacity(1);
                // 滑块绘制
                QRectF sliderRect =
                    subControlRect(control, sopt, QStyle::SC_ScrollBarSlider, widget);
                painter->setBrush(QtThemeColor(d->themeMode, ScrollBarHandle));
                if(sopt->orientation == Qt::Horizontal)
                {
                    sliderRect.setRect(sliderRect.x(),
                        sliderRect.bottom() - d->sliderMargin - d->sliderExtent, sliderRect.width(),
                        d->sliderExtent);
                }
                else
                {
                    sliderRect.setRect(sliderRect.right() - d->sliderMargin - d->sliderExtent,
                        sliderRect.y(), d->sliderExtent, sliderRect.height());
                }
                painter->drawRoundedRect(sliderRect, d->sliderExtent / 2.0, d->sliderExtent / 2.0);
                painter->restore();
            }
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int QtScrollBarStyle::pixelMetric(
    PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    Q_D(const QtScrollBarStyle);
    switch(metric)
    {
        case QStyle::PM_ScrollBarExtent: {
            return d->scrollBarExtent;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int QtScrollBarStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget,
    QStyleHintReturn* returnData) const
{
    if(hint == QStyle::SH_ScrollBar_LeftClickAbsolutePosition)
    {
        return true;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void QtScrollBarStyle::startExpandAnimation(bool isExpand)
{
    Q_D(QtScrollBarStyle);
    if(isExpand)
    {
        d->isExpand = true;
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "opacity");
        connect(opacityAnimation, &QPropertyAnimation::valueChanged, this,
            [this]() { d_ptr->scrollBar->update(); });
        opacityAnimation->setDuration(250);
        opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        opacityAnimation->setStartValue(d->opacity);
        opacityAnimation->setEndValue(1);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* extentAnimation = new QPropertyAnimation(this, "sliderExtent");
        extentAnimation->setDuration(250);
        extentAnimation->setEasingCurve(QEasingCurve::InOutSine);
        extentAnimation->setStartValue(d->sliderExtent);
        extentAnimation->setEndValue(d->scrollBarExtent - 2 * d->sliderMargin);
        extentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "opacity");
        connect(
            opacityAnimation, &QPropertyAnimation::finished, this, [this]() { d_ptr->isExpand = false; });
        connect(opacityAnimation, &QPropertyAnimation::valueChanged, this,
            [this]() { d_ptr->scrollBar->update(); });
        opacityAnimation->setDuration(250);
        opacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        opacityAnimation->setStartValue(d->opacity);
        opacityAnimation->setEndValue(0);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* extentAnimation = new QPropertyAnimation(this, "sliderExtent");
        extentAnimation->setDuration(250);
        extentAnimation->setEasingCurve(QEasingCurve::InOutSine);
        extentAnimation->setStartValue(d->sliderExtent);
        extentAnimation->setEndValue(0.5);
        extentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}