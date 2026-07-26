#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>
#include <qtmetamacros.h>

class QtTableViewStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtTableViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(int headerMargin READ headerMargin WRITE setHeaderMargin NOTIFY headerMarginChanged)
    Q_PROPERTY(int currentHoverRow READ currentHoverRow WRITE setCurrentHoverRow NOTIFY
            currentHoverRowChanged)
public:
    explicit QtTableViewStyle(QStyle* style = nullptr);
    ~QtTableViewStyle();

    int headerMargin() const;
    void setHeaderMargin(int value);

    int currentHoverRow() const;
    void setCurrentHoverRow(int value);

Q_SIGNALS:
    void headerMarginChanged();
    void currentHoverRowChanged();

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtTableViewStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTableViewStyle)
};
