#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtTableWidgetStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtTableWidgetStyle : public QProxyStyle
{
    Q_OBJECT

    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int headerMargin READ headerMargin WRITE setHeaderMargin NOTIFY headerMarginChanged)
    Q_PROPERTY(int currentHoverRow READ currentHoverRow WRITE setCurrentHoverRow NOTIFY
            currentHoverRowChanged)
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged)

public:
    explicit QtTableWidgetStyle(QStyle* style = nullptr);
    ~QtTableWidgetStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

    int itemHeight() const;
    void setItemHeight(int value);

    int headerMargin() const;
    void setHeaderMargin(int value);

    int currentHoverRow() const;
    void setCurrentHoverRow(int value);
    
    bool isTransparent() const;
    void setIsTransparent(bool value);

Q_SIGNALS:
    void itemHeightChanged();
    void headerMarginChanged();
    void currentHoverRowChanged();
    void isTransparentChanged();

private:
    QScopedPointer<class QtTableWidgetStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTableWidgetStyle)
};
