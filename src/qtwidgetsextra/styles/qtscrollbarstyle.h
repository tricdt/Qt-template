#pragma once

#include <QObject>
#include <QProxyStyle>
#include "widgets/qtscrollbar.h"
class QtScrollBar;
class QtScrollBarStylePrivate;
class QtScrollBarStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(bool isExpand READ isExpand WRITE setIsExpand NOTIFY isExpandChanged)
    Q_PROPERTY(QtScrollBar* scrollBar READ scrollBar WRITE setScrollBar NOTIFY scrollBarChanged)
    Q_PROPERTY(
        qreal sliderExtent READ sliderExtent WRITE setSliderExtent NOTIFY sliderExtentChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    QtScrollBarStyle(QStyle* style = nullptr);
    ~QtScrollBarStyle();

    bool isExpand() const;
    void setIsExpand(bool newIsExpand);

    QtScrollBar* scrollBar() const;
    void setScrollBar(QtScrollBar* newScrollBar);

    qreal sliderExtent() const;
    void setSliderExtent(qreal newSliderExtent);

    qreal opacity() const;
    void setOpacity(qreal newOpacity);

    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr,
        QStyleHintReturn* returnData = nullptr) const override;
    void startExpandAnimation(bool isExpand);

Q_SIGNALS:
    void isExpandChanged();
    void scrollBarChanged();
    void sliderExtentChanged();
    void opacityChanged();

private:

    QScopedPointer<class QtScrollBarStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollBarStyle)
};