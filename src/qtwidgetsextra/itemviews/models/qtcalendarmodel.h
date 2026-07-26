#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qtmetamacros.h>

enum QtCalendarType
{
    YearMode = 0x0001,
    MonthMode = 0x0002,
    DayMode = 0x0003,
};

struct QtCalendarData : public QObjectData
{
public:
    QtCalendarData() {};
    ~QtCalendarData() {};
    QtCalendarData(int year, int month, int day, QString desText = "")
        : year(year)
        , month(month)
        , day(day)
        , desText(desText) {};
    QtCalendarData(const QtCalendarData& other)
    {
        year = other.year;
        month = other.month;
        day = other.day;
        desText = other.desText;
    }
    int year = 1924;
    int month = 1;
    int day = 1;
    QString desText{ "" };
};
Q_DECLARE_METATYPE(QtCalendarData);

class QtCalendarModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QtCalendarModel(QObject* parent = nullptr);
    ~QtCalendarModel();

    QDate minimumDate() const;
    void setMinimumDate(QDate value);

    QDate maximumDate() const;
    void setMaximumDate(QDate value);

    void setDisplayMode(QtCalendarType displayType);
    QtCalendarType displayMode() const;

    QModelIndex indexFromDate(QDate date);
    QDate dateFromIndex(const QModelIndex& index) const;
    virtual QVariant data(const QModelIndex& index, int role) const override;

Q_SIGNALS:
    void displayModeChanged();

protected:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    QScopedPointer<class QtCalendarModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendarModel)
};
