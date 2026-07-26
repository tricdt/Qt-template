#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qspinbox.h>

class QtSpinBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_PROPERTY(QtSpinBoxType::ButtonMode buttonMode READ buttonMode WRITE setButtonMode NOTIFY
            buttonModeChanged)
public:
    explicit QtSpinBox(QWidget* parent = nullptr);
    ~QtSpinBox();

    QtSpinBoxType::ButtonMode buttonMode() const;
    void setButtonMode(QtSpinBoxType::ButtonMode mode);

Q_SIGNALS:
    void buttonModeChanged(QtSpinBoxType::ButtonMode mode);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    QScopedPointer<class QtSpinBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSpinBox)
};
