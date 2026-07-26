#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtCalendarPickerContainerPrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendarPickerContainer : public QWidget
{
    Q_OBJECT

public:
    explicit QtCalendarPickerContainer(QWidget* parent = nullptr);
    ~QtCalendarPickerContainer();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtCalendarPickerContainerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendarPickerContainer)
};
