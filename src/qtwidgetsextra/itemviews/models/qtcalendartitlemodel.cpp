#include "qtcalendartitlemodel.h"

class QtCalendarTitleModelPrivate
{
    Q_DECLARE_PUBLIC(QtCalendarTitleModel)
public:
    QtCalendarTitleModel* q_ptr;
    QtCalendarTitleModelPrivate(QtCalendarTitleModel* q);
    ~QtCalendarTitleModelPrivate();
};

QtCalendarTitleModelPrivate::QtCalendarTitleModelPrivate(QtCalendarTitleModel* q) : q_ptr(q)
{
}
QtCalendarTitleModelPrivate::~QtCalendarTitleModelPrivate()
{
}

QtCalendarTitleModel::QtCalendarTitleModel(QObject* parent)
    : QAbstractListModel{ parent }
    , d_ptr(new QtCalendarTitleModelPrivate(this))
{
}

QtCalendarTitleModel::~QtCalendarTitleModel()
{
}

int QtCalendarTitleModel::rowCount(const QModelIndex& parent) const
{
    return 7;
}

QVariant QtCalendarTitleModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        switch (index.row())
        {
        case 0:
        {
            return tr("日");
        }
        case 1:
        {
            return tr("一");
        }
        case 2:
        {
            return tr("二");
        }
        case 3:
        {
            return tr("三");
        }
        case 4:
        {
            return tr("四");
        }
        case 5:
        {
            return tr("五");
        }
        case 6:
        {
            return tr("六");
        }
        }
    }
    return QVariant();
}
