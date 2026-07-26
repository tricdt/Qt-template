#include "qtcolordisplaymodel.h"
#include <qobject.h>

class QtColorDisplayModelPrivate
{
    Q_DECLARE_PUBLIC(QtColorDisplayModel)
public:
    QtColorDisplayModel* q_ptr;
    QList<QColor> displayColorList;
    QtColorDisplayModelPrivate(QtColorDisplayModel* q);
    ~QtColorDisplayModelPrivate();
};

QtColorDisplayModelPrivate::QtColorDisplayModelPrivate(QtColorDisplayModel* q)
    : q_ptr(q)
{
}
QtColorDisplayModelPrivate::~QtColorDisplayModelPrivate()
{
}

QtColorDisplayModel::QtColorDisplayModel(QObject* parent)
    : QAbstractListModel{ parent }
    , d_ptr(new QtColorDisplayModelPrivate(this))
{
}

QtColorDisplayModel::~QtColorDisplayModel()
{
}

void QtColorDisplayModel::appendDisplayColor(QList<QColor> colorList)
{
    Q_D(QtColorDisplayModel);
    beginResetModel();
    d->displayColorList.append(colorList);
    endResetModel();
}
void QtColorDisplayModel::appendDisplayColor(QColor color)
{
    Q_D(QtColorDisplayModel);
    beginResetModel();
    d->displayColorList.append(color);
    endResetModel();
}
void QtColorDisplayModel::removeDisplayColor(int index)
{
    Q_D(QtColorDisplayModel);
    if(index < 0 || index >= d->displayColorList.count())
    {
        return;
    }
    beginResetModel();
    d->displayColorList.removeAt(index);
    endResetModel();
}
void QtColorDisplayModel::replaceDisplayColor(QColor color, int index)
{
    Q_D(QtColorDisplayModel);
    if(index < 0 || index >= d->displayColorList.count())
    {
        return;
    }
    beginResetModel();
    d->displayColorList[index] = color;
    endResetModel();
}
QList<QColor> QtColorDisplayModel::displayColorList() const
{
    Q_D(const QtColorDisplayModel);
    return d->displayColorList;
}
QColor QtColorDisplayModel::displayColor(int index) const
{
    Q_D(const QtColorDisplayModel);
    if(index < 0 || index >= d->displayColorList.count())
    {
        return QColor();
    }
    return d->displayColorList[index];
}
int QtColorDisplayModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const QtColorDisplayModel);
    return d->displayColorList.count();
}
QVariant QtColorDisplayModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QtColorDisplayModel);
    if(role == Qt::UserRole)
    {
        return QVariant::fromValue(d->displayColorList[index.row()]);
    }
    return QVariant();
}