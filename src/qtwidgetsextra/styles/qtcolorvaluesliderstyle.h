#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtColorValueSliderStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtColorValueSliderStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtColorValueSliderStyle(QStyle* style = nullptr);
    ~QtColorValueSliderStyle();

    QColor baseColor() const;
    void setBaseColor(QColor value);

protected:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

private:
    QScopedPointer<class QtColorValueSliderStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorValueSliderStyle)
};
