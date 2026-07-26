#include "qtfootermodel.h"
#include "utils/qtnavigationnode.h"
#include <qvariant.h>
class QtFooterModelPrivate
{
    Q_DECLARE_PUBLIC(QtFooterModel)
public:
    QtFooterModel* q_ptr;
    QtNavigationNode* selectedNode;
    QList<QtNavigationNode*> footerNodeList;
    QtFooterModelPrivate(QtFooterModel* q);
    ~QtFooterModelPrivate();
};

QtFooterModelPrivate::QtFooterModelPrivate(QtFooterModel* q)
    : q_ptr(q)
{
}
QtFooterModelPrivate::~QtFooterModelPrivate()
{
}

QtFooterModel::QtFooterModel(QObject* parent)
    : QAbstractListModel{ parent }
    , d_ptr(new QtFooterModelPrivate(this))
{
    Q_D(QtFooterModel);
    d->selectedNode = nullptr;
}

QtFooterModel::~QtFooterModel()
{
    Q_D(QtFooterModel);
    qDeleteAll(d->footerNodeList);
}

QtNavigationType::NodeResult QtFooterModel::addFooterNode(QString footerTitle, QString& footerKey,
    bool isHasFooterPage, int keyPoints, QtIconType::IconName awesome)
{
    Q_D(QtFooterModel);
    if(d->footerNodeList.count() >= 3)
    {
        return QtNavigationType::FooterUpperLimit;
    }
    QtNavigationNode* node = new QtNavigationNode(footerTitle);
    node->setKeyPoints(keyPoints);
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setAwesome(awesome);
    footerKey = node->nodeKey();
    beginResetModel();
    d->footerNodeList.append(node);
    endResetModel();
    QModelIndex index = this->index(d->footerNodeList.count() - 1);
    node->setModelIndex(index);
    return QtNavigationType::Success;
}

int QtFooterModel::footerNodeCount() const
{
    Q_D(const QtFooterModel);
    return d->footerNodeList.count();
}

QtNavigationNode* QtFooterModel::navigationNode(QString footerKey)
{
    Q_D(QtFooterModel);
    for(auto node : d->footerNodeList)
    {
        if(node->nodeKey() == footerKey)
        {
            return node;
        }
    }
    return nullptr;
}

void QtFooterModel::removeNavigationNode(QString footerKey)
{
    Q_D(QtFooterModel);
    for(auto node : d->footerNodeList)
    {
        if(node->nodeKey() == footerKey)
        {
            d->footerNodeList.removeOne(node);
            break;
        }
    }
}

QtNavigationNode* QtFooterModel::selectedNode() const
{
    Q_D(const QtFooterModel);
    return d->selectedNode;
}
void QtFooterModel::setSelectedNode(QtNavigationNode* value)
{
    Q_D(QtFooterModel);
    d->selectedNode = value;
}

int QtFooterModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const QtFooterModel);
    return d->footerNodeList.count();
}
QVariant QtFooterModel::data(const QModelIndex& index, int role) const
{
    Q_D(const QtFooterModel);
    if(role == Qt::UserRole)
    {
        if(index.row() < d->footerNodeList.count())
        {
            return QVariant::fromValue(d->footerNodeList[index.row()]);
        }
    }
    return QVariant();
}