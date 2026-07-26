#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>
#include <qtmetamacros.h>

class QtLCDNumberStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtLCDNumberStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged);

public:
    explicit QtLCDNumberStyle(QStyle* style = nullptr);
    ~QtLCDNumberStyle();

    bool isTransparent() const;
    void setIsTransparent(bool value);

Q_SIGNALS:
    void isTransparentChanged();

protected:
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtLCDNumberStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLCDNumberStyle)
};
