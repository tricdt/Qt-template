#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtColorPickerPrivate;
class QTWIDGETSEXTRA_EXPORT QtColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit QtColorPicker(QWidget* parent = nullptr);
    ~QtColorPicker();

    void setSelectedColor(QColor color);
    QColor selectedColor() const;

Q_SIGNALS:
    void selectedColorChanged(QColor selectedColor);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtColorPickerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorPicker)
};
