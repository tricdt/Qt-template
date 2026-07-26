#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtSliderStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtSliderStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtSliderStyle(QStyle* style = nullptr);
    ~QtSliderStyle();

protected:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

private:
    QScopedPointer<class QtSliderStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSliderStyle)
};
