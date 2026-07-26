#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>
#include <qtmetamacros.h>

class QtListViewStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtListViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged);
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged);

public:
    explicit QtListViewStyle(QStyle* style = nullptr);
    ~QtListViewStyle();

    int itemHeight() const;
    void setItemHeight(int value);

    bool isTransparent() const;
    void setIsTransparent(bool value);

Q_SIGNALS:
    void itemHeightChanged();
    void isTransparentChanged();

protected:
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;

private:
    QScopedPointer<class QtListViewStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtListViewStyle)
};
