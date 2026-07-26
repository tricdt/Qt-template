#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qspinbox.h>
#include <qtmetamacros.h>

class QtDoubleSpinBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_PROPERTY(QtSpinBoxType::ButtonMode buttonMode READ buttonMode WRITE setButtonMode NOTIFY
            buttonModeChanged)
public:
    explicit QtDoubleSpinBox(QWidget* parent = nullptr);
    ~QtDoubleSpinBox();

    QtSpinBoxType::ButtonMode buttonMode() const;
    void setButtonMode(QtSpinBoxType::ButtonMode value);

    Q_SIGNALS:
    void buttonModeChanged();

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    QScopedPointer<class QtDoubleSpinBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDoubleSpinBox)
};
