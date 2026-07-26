#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qproxystyle.h>

class QtPivotStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtPivotStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtPivotStyle(QStyle* style = nullptr);
    ~QtPivotStyle();

    QModelIndex pressIndex() const;
    void setPressIndex(QModelIndex);

    int currentIndex() const;
    void setCurrentIndex(int value);

    int pivotSpacing() const;
    void setPivotSpacing(int value);

    const QColor& markColor();

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtPivotStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPivotStyle)
};
