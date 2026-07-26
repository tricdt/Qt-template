#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>
#include <qtmetamacros.h>

class QtTabBarStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtTabBarStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(QSize tabSize READ tabSize WRITE setTabSize NOTIFY tabSizeChanged FINAL)
public:
    explicit QtTabBarStyle(QStyle* style = nullptr);
    ~QtTabBarStyle();

    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p,
        const QWidget* w) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;
    QRect subElementRect(
        SubElement element, const QStyleOption* option, const QWidget* widget) const override;

    QSize tabSize() const;
    void setTabSize(QSize value);

Q_SIGNALS:
    void tabSizeChanged();

private:
    QScopedPointer<class QtTabBarStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTabBarStyle)
};
