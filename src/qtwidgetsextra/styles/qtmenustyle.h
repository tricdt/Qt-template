#pragma once

#include <QObject>
#include <QProxyStyle>
#include "qtdef.h"
class QtMenuStylePrivate;
class QtMenuStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(int menuItemHeight READ menuItemHeight WRITE setMenuItemHeight NOTIFY menuItemHeightChanged);
public:
    QtMenuStyle(QStyle* style = nullptr);
    ~QtMenuStyle();

    int menuItemHeight() const;
    void setMenuItemHeight(int height);

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;
Q_SIGNALS:
    void menuItemHeightChanged();
private:

    QScopedPointer<class QtMenuStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMenuStyle)
};