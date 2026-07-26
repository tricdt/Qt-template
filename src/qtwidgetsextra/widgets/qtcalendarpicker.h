#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpushbutton.h>

class QtCalendarPickerPrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendarPicker : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(
        QDate selectedDate READ selectedDate WRITE setSelectedDate NOTIFY selectedDateChanged)
public:
    explicit QtCalendarPicker(QWidget* parent = nullptr);
    ~QtCalendarPicker();

    int borderRadius() const;
    void setBorderRadius(int value);

    QDate selectedDate() const;
    void setSelectedDate(QDate value);

Q_SIGNALS:
    void borderRadiusChanged();
    void selectedDateChanged(QDate date);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtCalendarPickerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendarPicker)
};
