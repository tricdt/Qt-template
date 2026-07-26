#include "qtbreadcrumbbarmodel.h"
#include <qabstractitemmodel.h>

class QtBreadcrumbBarModelPrivate
{
    Q_DECLARE_PUBLIC(QtBreadcrumbBarModel)
public:
    QtBreadcrumbBarModel* q_ptr;
    QStringList breadcrumbList;
    QtBreadcrumbBarModelPrivate(QtBreadcrumbBarModel* q);
    ~QtBreadcrumbBarModelPrivate();
};

QtBreadcrumbBarModelPrivate::QtBreadcrumbBarModelPrivate(QtBreadcrumbBarModel* q)
    : q_ptr(q)
{
}
QtBreadcrumbBarModelPrivate::~QtBreadcrumbBarModelPrivate()
{
}

QtBreadcrumbBarModel::QtBreadcrumbBarModel(QWidget* parent)
    : QAbstractListModel{ parent }
    , d_ptr(new QtBreadcrumbBarModelPrivate(this))
{
}

QtBreadcrumbBarModel::~QtBreadcrumbBarModel()
{
}

void QtBreadcrumbBarModel::appendBreadcrumb(QString breadcrumb)
{
    Q_D(QtBreadcrumbBarModel);
    if(!breadcrumb.isEmpty())
    {
        if(d->breadcrumbList.count() && d->breadcrumbList.last() == breadcrumb)
        {
            return;
        }
        if(d->breadcrumbList.count() >= 2 &&
            d->breadcrumbList[d->breadcrumbList.count() - 2] == breadcrumb)
        {
            beginResetModel();
            d->breadcrumbList.removeLast();
            endResetModel();
            return;
        }
        beginResetModel();
        d->breadcrumbList.append(breadcrumb);
        endResetModel();
    }
}

void QtBreadcrumbBarModel::removeBreadcrumb(QString breadcrumb)
{
    Q_D(QtBreadcrumbBarModel);
    if(d->breadcrumbList.contains(breadcrumb))
    {
        beginResetModel();
        d->breadcrumbList.removeAt(d->breadcrumbList.lastIndexOf(breadcrumb));
        endResetModel();
    }
}

void QtBreadcrumbBarModel::removeBreadcrumb(int index)
{
    Q_D(QtBreadcrumbBarModel);
    if(index >= d->breadcrumbList.count())
    {
        return;
    }
    beginResetModel();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    d->breadcrumbList.remove(index, d->breadcrumbList.count() - index);
#else
    for(int i = _breadcrumbList.count() - 1; i >= 0; i--)
    {
        if(i < index)
        {
            break;
        }
        _breadcrumbList.removeAt(i);
    }
#endif
    endResetModel();
}

void QtBreadcrumbBarModel::setBreadcrumbList(QStringList breadcrumbList)
{
    Q_D(QtBreadcrumbBarModel);
    beginResetModel();
    d->breadcrumbList = breadcrumbList;
    endResetModel();
}

int QtBreadcrumbBarModel::breadcrumbListCount()
{
    Q_D(QtBreadcrumbBarModel);
    return d->breadcrumbList.count();
}

QStringList QtBreadcrumbBarModel::breadcrumbList()
{
    Q_D(QtBreadcrumbBarModel);
    return d->breadcrumbList;
}

int QtBreadcrumbBarModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const QtBreadcrumbBarModel);
    if(!d->breadcrumbList.isEmpty())
    {
        return d->breadcrumbList.count() * 2 - 1;
    }
    return 0;
}

QVariant QtBreadcrumbBarModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QtBreadcrumbBarModel);
    if(role == Qt::DisplayRole)
    {
        if(index.row() % 2 == 0)
        {
            return d->breadcrumbList[index.row() / 2];
        }
        else
        {
            return ">";
        }
    }
    else if(role == Qt::UserRole)
    {
        if(index.row() == d->breadcrumbList.count() * 2 - 2 ||
            index.row() == d->breadcrumbList.count() * 2 - 3)
        {
            return QString("LastBreadcrumb");
        }
    }
    return QVariant();
}