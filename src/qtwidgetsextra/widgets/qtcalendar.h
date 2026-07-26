#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtCalendarPrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(
        QDate selectedDate READ selectedDate WRITE setSelectedDate NOTIFY selectedDateChanged)
    Q_PROPERTY(QDate minimumDate READ minimumDate WRITE setMinimumDate NOTIFY minimumDateChanged)
    Q_PROPERTY(QDate maximumDate READ maximumDate WRITE setMaximumDate NOTIFY maximumDateChanged)
public:
    explicit QtCalendar(QWidget* parent = nullptr);
    ~QtCalendar();

    int borderRadius() const;
    void setBorderRadius(int value);

    QDate selectedDate() const;
    void setSelectedDate(QDate value);

    QDate minimumDate() const;
    void setMinimumDate(QDate value);

    QDate maximumDate() const;
    void setMaximumDate(QDate value);

Q_SIGNALS:
    void borderRadiusChanged();
    void selectedDateChanged();
    void minimumDateChanged();
    void maximumDateChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtCalendarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendar)
};
