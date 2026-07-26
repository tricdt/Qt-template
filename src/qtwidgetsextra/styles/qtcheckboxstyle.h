#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtCheckBoxStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtCheckBoxStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtCheckBoxStyle(QStyle* style = nullptr);
    ~QtCheckBoxStyle();

protected:
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtCheckBoxStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCheckBoxStyle)
};
