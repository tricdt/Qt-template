#include "qtcalendarmodel.h"
#include <qdatetime.h>

class QtCalendarModelPrivate
{
    Q_DECLARE_PUBLIC(QtCalendarModel)
public:
    QtCalendarModel* q_ptr;
    int offset{ 0 };
    QDate minimumDate;
    QDate maximumDate;
    QtCalendarType displayMode{ QtCalendarType::DayMode };
    int dayRowCount{ 0 };
    QtCalendarModelPrivate(QtCalendarModel* q);
    ~QtCalendarModelPrivate();
    void initRowCount();
    int currentDay(int row) const;
};

QtCalendarModelPrivate::QtCalendarModelPrivate(QtCalendarModel* q)
    : q_ptr(q)
{
}
QtCalendarModelPrivate::~QtCalendarModelPrivate()
{
}

void QtCalendarModelPrivate::initRowCount()
{
    dayRowCount = minimumDate.daysTo(maximumDate);
    offset = minimumDate.dayOfWeek();
    dayRowCount += offset + 1;
}

int QtCalendarModelPrivate::currentDay(int row) const
{
    return minimumDate.addDays(row - offset).day();
}

QtCalendarModel::QtCalendarModel(QObject* parent)
    : QAbstractListModel{ parent }
    , d_ptr(new QtCalendarModelPrivate(this))
{
    Q_D(QtCalendarModel);
    d->minimumDate.setDate(1924, 1, 1);
    d->maximumDate.setDate(2124, 12, 31);
    d->initRowCount();
}

QtCalendarModel::~QtCalendarModel()
{
}

QDate QtCalendarModel::minimumDate() const
{
    Q_D(const QtCalendarModel);
    return d->minimumDate;
}
void QtCalendarModel::setMinimumDate(QDate value)
{
    Q_D(QtCalendarModel);
    beginResetModel();
    d->minimumDate = value;
    d->initRowCount();
    endResetModel();
}
QDate QtCalendarModel::maximumDate() const
{
    Q_D(const QtCalendarModel);
    return d->maximumDate;
}
void QtCalendarModel::setMaximumDate(QDate value)
{
    Q_D(QtCalendarModel);
    beginResetModel();
    d->maximumDate = value;
    d->initRowCount();
    endResetModel();
}

void QtCalendarModel::setDisplayMode(QtCalendarType displayMode)
{
    Q_D(QtCalendarModel);
    beginResetModel();
    d->displayMode = displayMode;
    endResetModel();
    Q_EMIT displayModeChanged();
}
QtCalendarType QtCalendarModel::displayMode() const
{
    Q_D(const QtCalendarModel);
    return d->displayMode;
}

QModelIndex QtCalendarModel::indexFromDate(QDate date)
{
    Q_D(QtCalendarModel);
    switch(d->displayMode)
    {
        case YearMode: {
            return this->index(date.year() - d->minimumDate.year());
        }
        case MonthMode: {
            return this->index((date.year() - d->minimumDate.year()) * 12 + date.month() - 1);
        }
        case DayMode: {
            return this->index(d->minimumDate.daysTo(date) + d->offset);
        }
    }
    return QModelIndex();
}
QDate QtCalendarModel::dateFromIndex(const QModelIndex& index) const
{
    Q_D(const QtCalendarModel);
    if(!index.isValid() || index.row() < d->offset)
    {
        return QDate();
    }
    return d->minimumDate.addDays(index.row() - d->offset);
}

int QtCalendarModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const QtCalendarModel);
    switch(d->displayMode)
    {
        case YearMode: {
            return d->maximumDate.year() - d->minimumDate.year() + 1;
        }
        case MonthMode: {
            return (d->maximumDate.year() - d->minimumDate.year() + 1) * 12;
        }
        case DayMode: {
            return d->dayRowCount;
        }
    }
    return 0;
}

QVariant QtCalendarModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QtCalendarModel);
    if(role == Qt::UserRole)
    {
        switch(d->displayMode)
        {
            case YearMode: {
                return QVariant::fromValue<QtCalendarData>(
                    QtCalendarData(d->minimumDate.year() + index.row(), 1, 1));
            }
            case MonthMode: {
                int year = d->minimumDate.year() + index.row() / 12;
                int month = index.row() % 12 + 1;
                if(month == 1)
                {
                    return QVariant::fromValue<QtCalendarData>(
                        QtCalendarData(year, month, 1, QString::number(year)));
                }
                else
                {
                    return QVariant::fromValue<QtCalendarData>(
                        QtCalendarData(d->minimumDate.year() + index.row() / 12, month, 1));
                }
            }
            case DayMode: {
                if(index.row() >= d->offset)
                {
                    QDate date = dateFromIndex(index);
                    if(date.day() == 1)
                    {
                        return QVariant::fromValue<QtCalendarData>(QtCalendarData(date.year(),
                            date.month(), date.day(), QString("%1月").arg(date.month())));
                    }
                    else
                    {
                        return QVariant::fromValue<QtCalendarData>(
                            QtCalendarData(date.year(), date.month(), date.day()));
                    }
                }
            }
        }
    }
    return QVariant();
}