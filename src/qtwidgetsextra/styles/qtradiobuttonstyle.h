#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>
#include <qstyle.h>

class QtRadioButtonStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtRadioButtonStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtRadioButtonStyle(QStyle* parent = nullptr);
    ~QtRadioButtonStyle();

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtRadioButtonStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRadioButtonStyle)
};
