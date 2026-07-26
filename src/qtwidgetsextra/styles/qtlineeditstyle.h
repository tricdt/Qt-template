#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtLineEditStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtLineEditStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtLineEditStyle(QStyle* style = nullptr);
    ~QtLineEditStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtLineEditStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLineEditStyle)
};
