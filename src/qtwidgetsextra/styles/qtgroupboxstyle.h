#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtGroupBoxStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtGroupBoxStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtGroupBoxStyle(QStyle* style = nullptr);
    ~QtGroupBoxStyle();

    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtGroupBoxStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtGroupBoxStyle)
};
