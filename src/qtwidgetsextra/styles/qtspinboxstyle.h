#pragma once

#include "qtdef.h"

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>
class QtSpinBoxStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtSpinBoxStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(QtSpinBoxType::ButtonMode buttonMode READ buttonMode WRITE setButtonMode NOTIFY
            buttonModeChanged);

public:
    explicit QtSpinBoxStyle(QStyle* parent = nullptr);
    ~QtSpinBoxStyle();

    QtSpinBoxType::ButtonMode buttonMode() const;
    void setButtonMode(QtSpinBoxType::ButtonMode buttonMode);

Q_SIGNALS:
    void buttonModeChanged(QtSpinBoxType::ButtonMode buttonMode);

protected:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc,
        const QWidget* widget) const override;
    QStyle::SubControl hitTestComplexControl(ComplexControl control,
        const QStyleOptionComplex* option, const QPoint& pt, const QWidget* widget) const override;

private:
    QScopedPointer<class QtSpinBoxStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSpinBoxStyle)
};
