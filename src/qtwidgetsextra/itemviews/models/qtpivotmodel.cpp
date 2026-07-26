#include "qtpivotmodel.h"

class QtPivotModelPrivate
{
    Q_DECLARE_PUBLIC(QtPivotModel)
public:
    QtPivotModel* q_ptr;
    QStringList pivotList;
    QtPivotModelPrivate(QtPivotModel* q);
    ~QtPivotModelPrivate();
};

QtPivotModelPrivate::QtPivotModelPrivate(QtPivotModel* q)
    : q_ptr(q)
{
}
QtPivotModelPrivate::~QtPivotModelPrivate()
{
}

QtPivotModel::QtPivotModel(QObject* parent)
    : QAbstractListModel{ parent }
    , d_ptr(new QtPivotModelPrivate(this))
{
}

QtPivotModel::~QtPivotModel()
{
}

void QtPivotModel::appendPivot(QString pivot)
{
    Q_D(QtPivotModel);
    if(!pivot.isEmpty())
    {
        beginInsertRows(QModelIndex(), d->pivotList.count(), d->pivotList.count());
        d->pivotList.append(pivot);
        endInsertRows();
        return;
    }
}
void QtPivotModel::removePivot(QString pivot)
{
    Q_D(QtPivotModel);
    if(d->pivotList.contains(pivot))
    {
        int index = d->pivotList.lastIndexOf(pivot);
        beginRemoveRows(QModelIndex(), index, index);
        d->pivotList.removeAt(index);
        endRemoveRows();
    }
}

int QtPivotModel::pivotListCount() const
{
    Q_D(const QtPivotModel);
    return d->pivotList.count();
}

int QtPivotModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const QtPivotModel);
    return d->pivotList.count();
}

QVariant QtPivotModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QtPivotModel);
    if(role == Qt::DisplayRole)
    {
        return d->pivotList[index.row()];
    }
    return QVariant();
}
