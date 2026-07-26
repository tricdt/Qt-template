#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtTreeViewStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtTreeViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int headerMargin READ headerMargin WRITE setHeaderMargin NOTIFY headerMarginChanged)
public:
    explicit QtTreeViewStyle(QStyle* style = nullptr);
    ~QtTreeViewStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    QRect subElementRect(
        SubElement element, const QStyleOption* option, const QWidget* widget) const override;

    int itemHeight() const;
    void setItemHeight(int value);

    int headerMargin() const;
    void setHeaderMargin(int value);

Q_SIGNALS:
    void itemHeightChanged();
    void headerMarginChanged();

private:
    QScopedPointer<class QtTreeViewStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTreeViewStyle)
};
