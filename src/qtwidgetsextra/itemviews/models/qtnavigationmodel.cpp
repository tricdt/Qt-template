#include "qtnavigationmodel.h"

class QtNavigationModelPrivate
{
    Q_DECLARE_PUBLIC(QtNavigationModel)
public:
    QtNavigationNode* selectedNode;
    QtNavigationNode* selectedExpandedNode;
    QtNavigationModel* q_ptr;
    bool isMaximalMode{ true };
    QMap<QString, QtNavigationNode*> nodesMap;
    QtNavigationNode* rootNode{ nullptr };
    QtNavigationModelPrivate(QtNavigationModel* q);
    ~QtNavigationModelPrivate();
};

QtNavigationModelPrivate::QtNavigationModelPrivate(QtNavigationModel* q)
    : q_ptr(q)
{
}
QtNavigationModelPrivate::~QtNavigationModelPrivate()
{
}

QtNavigationModel::QtNavigationModel(QObject* parent)
    : QAbstractItemModel{ parent }
    , d_ptr(new QtNavigationModelPrivate(this))
{
    Q_D(QtNavigationModel);
    d->rootNode = new QtNavigationNode("root");
    d->rootNode->setIsRootNode(true);
    d->rootNode->setIsExpanderNode(true);
    d->selectedNode = nullptr;
    d->selectedExpandedNode = nullptr;
}

QtNavigationModel::~QtNavigationModel()
{
}

QModelIndex QtNavigationModel::parent(const QModelIndex& child) const
{
    Q_D(const QtNavigationModel);
    if(!child.isValid())
    {
        return {};
    }
    QtNavigationNode* childNode = static_cast<QtNavigationNode*>(child.internalPointer());
    QtNavigationNode* parentNode = childNode->parentNode();
    if(parentNode == d->rootNode)
    {
        return {};
    }
    if(parentNode == nullptr)
    {
        return {};
    }
    return createIndex(parentNode->row(), 0, parentNode);
}

QModelIndex QtNavigationModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_D(const QtNavigationModel);
    if(!hasIndex(row, column, parent))
    {
        return {};
    }
    QtNavigationNode* parentNode;
    if(!parent.isValid())
    {
        parentNode = d->rootNode;
    }
    else
    {
        parentNode = static_cast<QtNavigationNode*>(parent.internalPointer());
    }
    QtNavigationNode* childNode = nullptr;
    if(parentNode->childrenNodes().count() > row)
    {
        if(parentNode == d->rootNode && !d->isMaximalMode)
        {
            childNode = parentNode->exceptCategoryNodes().at(row);
        }
        else
        {
            childNode = parentNode->childrenNodes().at(row);
        }
    }
    if(childNode)
    {
        QModelIndex index = createIndex(row, column, childNode);
        childNode->setModelIndex(index);
        return childNode->modelIndex();
    }
    return {};
}

int QtNavigationModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const QtNavigationModel);
    QtNavigationNode* parentNode;
    if(parent.column() > 0)
    {
        return 0;
    }
    if(!parent.isValid())
    {
        parentNode = d->rootNode;
    }
    else
    {
        parentNode = static_cast<QtNavigationNode*>(parent.internalPointer());
    }
    if(parentNode == d->rootNode && !d->isMaximalMode)
    {
        return d->rootNode->exceptCategoryNodes().count();
    }
    return parentNode->childrenNodes().count();
};

int QtNavigationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant QtNavigationModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

QtNavigationType::NodeResult QtNavigationModel::addExpanderNode(
    const QString& expanderTitle, QString& expanderKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationModel);
    QtNavigationNode* node = new QtNavigationNode(expanderTitle, d->rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    beginInsertRows(
        QModelIndex(), d->rootNode->childrenNodes().count(), d->rootNode->childrenNodes().count());
    d->rootNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    expanderKey = node->nodeKey();
    return QtNavigationType::NodeResult::Success;
}
QtNavigationType::NodeResult QtNavigationModel::addExpanderNode(const QString& expanderTitle,
    QString& expanderKey, const QString& targetExpanderKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationModel);
    if(!d->nodesMap.contains(targetExpanderKey))
    {
        return QtNavigationType::NodeResult::TargetNodeInvalid;
    }
    QtNavigationNode* parentNode = d->nodesMap.value(targetExpanderKey);
    if(!parentNode->isExpanderNode())
    {
        return QtNavigationType::NodeResult::TargetNodeTypeError;
    }
    if(parentNode->depth() > 10)
    {
        return QtNavigationType::NodeResult::TargetNodeDepthLimit;
    }
    QtNavigationNode* node = new QtNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->depth() + 1);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    if(parentNode->isVisible() && parentNode->isExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->modelIndex(), parentNode->childrenNodes().count(),
        parentNode->childrenNodes().count());
    parentNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    expanderKey = node->nodeKey();
    return QtNavigationType::NodeResult::Success;
}

QtNavigationType::NodeResult QtNavigationModel::addPageNode(
    const QString& pageTitle, QString& pageKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationModel);
    QtNavigationNode* node = new QtNavigationNode(pageTitle, d->rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    beginInsertRows(
        QModelIndex(), d->rootNode->childrenNodes().count(), d->rootNode->childrenNodes().count());
    d->rootNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    pageKey = node->nodeKey();
    if(!d->selectedNode)
    {
        d->selectedNode = node;
    }
    return QtNavigationType::NodeResult::Success;
}

QtNavigationType::NodeResult QtNavigationModel::addPageNode(const QString& pageTitle,
    QString& pageKey, const QString& targetExpanderKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationModel);
    if(!d->nodesMap.contains(targetExpanderKey))
    {
        return QtNavigationType::NodeResult::TargetNodeInvalid;
    }
    QtNavigationNode* parentNode = d->nodesMap.value(targetExpanderKey);
    if(!parentNode->isExpanderNode())
    {
        return QtNavigationType::NodeResult::TargetNodeTypeError;
    }
    if(parentNode->depth() > 10)
    {
        return QtNavigationType::NodeResult::TargetNodeDepthLimit;
    }
    QtNavigationNode* node = new QtNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->depth() + 1);
    node->setAwesome(awesome);
    if(parentNode->isVisible() && parentNode->isExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->modelIndex(), parentNode->childrenNodes().count(),
        parentNode->childrenNodes().count());
    parentNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    pageKey = node->nodeKey();
    if(!d->selectedNode)
    {
        d->selectedNode = node;
    }
    return QtNavigationType::NodeResult::Success;
}

QtNavigationType::NodeResult QtNavigationModel::addPageNode(
    const QString& pageTitle, QString& pageKey, int keyPoints, QtIconType::IconName awesome)
{
    Q_D(QtNavigationModel);
    QtNavigationNode* node = new QtNavigationNode(pageTitle, d->rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);
    beginInsertRows(
        QModelIndex(), d->rootNode->childrenNodes().count(), d->rootNode->childrenNodes().count());
    d->rootNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    pageKey = node->nodeKey();
    if(!d->selectedNode)
    {
        d->selectedNode = node;
    }
    return QtNavigationType::NodeResult::Success;
}

QtNavigationType::NodeResult QtNavigationModel::addPageNode(const QString& pageTitle,
    QString& pageKey, const QString& targetExpanderKey, int keyPoints, QtIconType::IconName awesome)
{
    Q_D(QtNavigationModel);
    if(!d->nodesMap.contains(targetExpanderKey))
    {
        return QtNavigationType::NodeResult::TargetNodeInvalid;
    }
    QtNavigationNode* parentNode = d->nodesMap.value(targetExpanderKey);
    if(!parentNode->isExpanderNode())
    {
        return QtNavigationType::NodeResult::TargetNodeTypeError;
    }
    if(parentNode->depth() > 10)
    {
        return QtNavigationType::NodeResult::TargetNodeDepthLimit;
    }
    QtNavigationNode* node = new QtNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->depth() + 1);
    node->setAwesome(awesome);
    node->setKeyPoints(keyPoints);
    if(parentNode->isVisible() && parentNode->isExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->modelIndex(), parentNode->childrenNodes().count(),
        parentNode->childrenNodes().count());
    parentNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    pageKey = node->nodeKey();
    if(!d->selectedNode)
    {
        d->selectedNode = node;
    }
    return QtNavigationType::NodeResult::Success;
}

QtNavigationType::NodeResult QtNavigationModel::addCategoryNode(
    const QString& categoryTitle, QString& categoryKey)
{
    Q_D(QtNavigationModel);
    QtNavigationNode* node = new QtNavigationNode(categoryTitle, d->rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsCategoryNode(true);
    beginInsertRows(
        QModelIndex(), d->rootNode->childrenNodes().count(), d->rootNode->childrenNodes().count());
    d->rootNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    categoryKey = node->nodeKey();
    return QtNavigationType::NodeResult::Success;
}
QtNavigationType::NodeResult QtNavigationModel::addCategoryNode(
    const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey)
{
    Q_D(QtNavigationModel);
    if(!d->nodesMap.contains(targetExpanderKey))
    {
        return QtNavigationType::NodeResult::TargetNodeInvalid;
    }
    QtNavigationNode* parentNode = d->nodesMap.value(targetExpanderKey);
    if(!parentNode->isExpanderNode())
    {
        return QtNavigationType::NodeResult::TargetNodeTypeError;
    }
    if(parentNode->depth() > 10)
    {
        return QtNavigationType::NodeResult::TargetNodeDepthLimit;
    }
    QtNavigationNode* node = new QtNavigationNode(categoryTitle, parentNode);
    node->setDepth(parentNode->depth() + 1);
    node->setIsCategoryNode(true);
    if(parentNode->isVisible() && parentNode->isExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->modelIndex(), parentNode->childrenNodes().count(),
        parentNode->childrenNodes().count());
    parentNode->appendChildNode(node);
    d->nodesMap.insert(node->nodeKey(), node);
    endInsertRows();
    categoryKey = node->nodeKey();
    return QtNavigationType::NodeResult::Success;
}

QStringList QtNavigationModel::removeNavigationNode(const QString& nodeKey)
{
    Q_D(QtNavigationModel);
    QList<QString> removeKeyList;
    if(!d->nodesMap.contains(nodeKey))
    {
        return removeKeyList;
    }
    QtNavigationNode* node = d->nodesMap.value(nodeKey);
    QtNavigationNode* parentNode = node->parentNode();
    if(node->isExpanderNode())
    {
        QList<QtNavigationNode*> childNodeList = node->childrenNodes();
        for(int i = 0; i < childNodeList.count(); i++)
        {
            QtNavigationNode* childNode = childNodeList[i];
            QList<QString> childRemoveKeyList = removeNavigationNode(childNode->nodeKey());
            removeKeyList.append(childRemoveKeyList);
        }
    }
    else
    {
        removeKeyList.append(node->nodeKey());
    }
    d->nodesMap.remove(node->nodeKey());
    int removeRow = parentNode->childrenNodes().indexOf(node);
    beginRemoveRows(parentNode->modelIndex(), removeRow, removeRow);
    parentNode->removeChildNode(node);
    endRemoveRows();
    node->deleteLater();
    return removeKeyList;
}

QtNavigationNode* QtNavigationModel::navigationNode(const QString& nodeKey) const
{
    Q_D(const QtNavigationModel);
    if(d->nodesMap.contains(nodeKey))
    {
        return d->nodesMap.value(nodeKey);
    }
    return nullptr;
}

QList<QtNavigationNode*> QtNavigationModel::rootExpanderNodes() const
{
    Q_D(const QtNavigationModel);
    QList<QtNavigationNode*> expandedNodeList;
    for(auto node : d->rootNode->childrenNodes())
    {
        if(node->isExpanderNode())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}

QList<QtNavigationNode*> QtNavigationModel::rootExpandedNodes() const
{
    Q_D(const QtNavigationModel);
    QList<QtNavigationNode*> expandedNodeList;
    for(auto node : d->rootNode->childrenNodes())
    {
        if(node->isExpanderNode() && node->isExpanded())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}

QList<QtNavigationNode*> QtNavigationModel::rootCategoryNodes() const
{
    Q_D(const QtNavigationModel);
    QList<QtNavigationNode*> categoryNodeList;
    for(auto node : d->rootNode->childrenNodes())
    {
        if(node->isCategoryNode())
        {
            categoryNodeList.append(node);
        }
    }
    return categoryNodeList;
}

QtNavigationNode* QtNavigationModel::selectedNode() const
{
    Q_D(const QtNavigationModel);
    return d->selectedNode;
}
void QtNavigationModel::setSelectedNode(QtNavigationNode* value)
{
    Q_D(QtNavigationModel);
    d->selectedNode = value;
}

QtNavigationNode* QtNavigationModel::selectedExpandedNode() const
{
    Q_D(const QtNavigationModel);
    return d->selectedExpandedNode;
}
void QtNavigationModel::setSelectedExpandedNode(QtNavigationNode* value)
{
    Q_D(QtNavigationModel);
    d->selectedExpandedNode = value;
}