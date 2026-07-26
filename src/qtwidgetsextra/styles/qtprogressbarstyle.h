#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtProgressBarStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtProgressBarStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(int busyStartValue READ busyStartValue WRITE setBusyStartValue)
    Q_PROPERTY(int busyEndValue READ busyEndValue WRITE setBusyEndValue)

public:
    explicit QtProgressBarStyle(QStyle* style = nullptr);
    ~QtProgressBarStyle();

    int busyStartValue() const;
    void setBusyStartValue(int busyStartValue);

    int busyEndValue() const;
    void setBusyEndValue(int busyEndValue);

protected:
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    QRect subElementRect(
        SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    QScopedPointer<class QtProgressBarStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtProgressBarStyle)
};
