#include "qtnavigationnode.h"
#include "qtdef.h"
#include <qabstractitemmodel.h>
#include <quuid.h>

class QtNavigationNodePrivate
{
    Q_DECLARE_PUBLIC(QtNavigationNode)
public:
    QList<QtNavigationNode*> childrenNodes;
    QtNavigationNode* parentNode;
    QtIconType::IconName awesome;
    QModelIndex modelIndex;
    int keyPoints;
    int depth;
    bool isRootNode;
    bool isFooterNode;
    bool isHasFooterPage;
    bool isExpanderNode;
    bool isCategoryNode;
    bool isVisible;
    QString nodeTitle;
    QtNavigationNode* q_ptr;
    QString nodeKey;
    bool isExpanded;
    QtNavigationNodePrivate(QtNavigationNode* q);
    ~QtNavigationNodePrivate();
};

QtNavigationNodePrivate::QtNavigationNodePrivate(QtNavigationNode* q)
    : q_ptr(q)
    , nodeKey("")
    , isExpanded(false)
{
}
QtNavigationNodePrivate::~QtNavigationNodePrivate()
{
}

QtNavigationNode::QtNavigationNode(const QString& nodeTitle, QtNavigationNode* parent)
    : QObject{ parent }
    , d_ptr(new QtNavigationNodePrivate(this))
{
    Q_D(QtNavigationNode);
    d->depth = 0;
    d->keyPoints = 0;
    d->nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    d->nodeTitle = nodeTitle;
    d->isRootNode = false;
    d->isFooterNode = false;
    d->isHasFooterPage = false;
    d->parentNode = parent;
    d->isExpanderNode = false;
    d->isVisible = false;
    d->isCategoryNode = false;
    d->awesome = QtIconType::None;
}

QtNavigationNode::~QtNavigationNode()
{
}

QString QtNavigationNode::nodeKey() const
{
    Q_D(const QtNavigationNode);
    return d->nodeKey;
}

void QtNavigationNode::setIsExpanded(bool isExpanded)
{
    Q_D(QtNavigationNode);
    d->isExpanded = isExpanded;
    setChildVisible(isExpanded);
}
bool QtNavigationNode::isExpanded() const
{
    Q_D(const QtNavigationNode);
    return d->isExpanded;
}

void QtNavigationNode::setChildVisible(bool isVisible)
{
    Q_D(QtNavigationNode);
    if(isVisible)
    {
        for(auto node : d->childrenNodes)
        {
            node->setIsVisible(isVisible);
            if(node->isExpanderNode() && !node->isExpanded())
            {
                continue;
            }
            node->setChildVisible(isVisible);
        }
    }
    else
    {
        for(auto node : d->childrenNodes)
        {
            node->setChildVisible(isVisible);
            node->setIsVisible(isVisible);
        }
    }
}
bool QtNavigationNode::isHasChild() const
{
    Q_D(const QtNavigationNode);
    if(d->childrenNodes.count() > 0)
    {
        return true;
    }
    return false;
}
bool QtNavigationNode::isHasPageChild() const
{
    Q_D(const QtNavigationNode);
    if(d->childrenNodes.count() == 0)
    {
        return false;
    }
    for(auto childNode : d->childrenNodes)
    {
        if(!childNode->isExpanderNode())
        {
            return true;
        }
        if(childNode->isHasPageChild())
        {
            return true;
        }
    }
    return false;
}

void QtNavigationNode::appendChildNode(QtNavigationNode* childNode)
{
    Q_D(QtNavigationNode);
    if(d->isExpanderNode)
    {
        d->childrenNodes.append(childNode);
    }
}
void QtNavigationNode::removeChildNode(QtNavigationNode* childNode)
{
    Q_D(QtNavigationNode);
    if(d->isExpanderNode)
    {
        d->childrenNodes.removeOne(childNode);
    }
}

bool QtNavigationNode::isChildHasKeyPoints() const
{
    Q_D(const QtNavigationNode);
    for(auto childNnode : d->childrenNodes)
    {
        if(childNnode->keyPoints())
        {
            return true;
        }
        if(childNnode->isChildHasKeyPoints())
        {
            return true;
        }
    }
    return false;
}

QtNavigationNode* QtNavigationNode::originalNode()
{
    Q_D(QtNavigationNode);
    if(this->parentNode()->isRootNode())
    {
        return this;
    }
    else
    {
        QtNavigationNode* originalNode = this->parentNode();
        while(!originalNode->isRootNode() && !originalNode->parentNode()->isRootNode())
        {
            originalNode = originalNode->parentNode();
        }
        return originalNode;
    }
}
bool QtNavigationNode::isChildNode(QtNavigationNode* node)
{
    Q_D(QtNavigationNode);
    if(d->childrenNodes.count() > 0)
    {
        if(d->childrenNodes.contains(node))
        {
            return true;
        }
        for(auto childNnode : d->childrenNodes)
        {
            if(childNnode->isChildNode(node))
            {
                return true;
            }
        }
    }
    return false;
}

int QtNavigationNode::row() const
{
    Q_D(const QtNavigationNode);
    if(d->parentNode)
    {
        return d->parentNode->childrenNodes().indexOf(const_cast<QtNavigationNode*>(this));
    }
    return 0;
}
int QtNavigationNode::rowExceptCategoryNodes() const
{
    Q_D(const QtNavigationNode);
    if(d->parentNode)
    {
        return d->parentNode->exceptCategoryNodes().indexOf(const_cast<QtNavigationNode*>(this));
    }
    return 0;
}

QList<QtNavigationNode*> QtNavigationNode::exceptCategoryNodes()
{
    Q_D(QtNavigationNode);
    QList<QtNavigationNode*> exceptCategoryNodeList;
    for(auto node : d->childrenNodes)
    {
        if(!node->isCategoryNode())
        {
            exceptCategoryNodeList.append(node);
        }
    }
    return exceptCategoryNodeList;
}

QList<QtNavigationNode*> QtNavigationNode::childrenNodes() const
{
    Q_D(const QtNavigationNode);
    return d->childrenNodes;
}
void QtNavigationNode::setChildrenNodes(QList<QtNavigationNode*> value)
{
    Q_D(QtNavigationNode);
    d->childrenNodes = value;
}

QtNavigationNode* QtNavigationNode::parentNode() const
{
    Q_D(const QtNavigationNode);
    return d->parentNode;
}
void QtNavigationNode::setParentNode(QtNavigationNode* value)
{
    Q_D(QtNavigationNode);
    d->parentNode = value;
}

QtIconType::IconName QtNavigationNode::awesome() const
{
    Q_D(const QtNavigationNode);
    return d->awesome;
}
void QtNavigationNode::setAwesome(QtIconType::IconName value)
{
    Q_D(QtNavigationNode);
    d->awesome = value;
}

const QModelIndex& QtNavigationNode::modelIndex() const
{
    Q_D(const QtNavigationNode);
    return d->modelIndex;
}
void QtNavigationNode::setModelIndex(QModelIndex& value)
{
    Q_D(QtNavigationNode);
    d->modelIndex = value;
}

int QtNavigationNode::keyPoints() const
{
    Q_D(const QtNavigationNode);
    return d->keyPoints;
}
void QtNavigationNode::setKeyPoints(int value)
{
    Q_D(QtNavigationNode);
    d->keyPoints = value;
}

int QtNavigationNode::depth() const
{
    Q_D(const QtNavigationNode);
    return d->depth;
}
void QtNavigationNode::setDepth(int value)
{
    Q_D(QtNavigationNode);
    d->depth = value;
}

bool QtNavigationNode::isRootNode() const
{
    Q_D(const QtNavigationNode);
    return d->isRootNode;
}
void QtNavigationNode::setIsRootNode(bool value)
{
    Q_D(QtNavigationNode);
    d->isRootNode = value;
}

bool QtNavigationNode::isFooterNode() const
{
    Q_D(const QtNavigationNode);
    return d->isFooterNode;
}
void QtNavigationNode::setIsFooterNode(bool value)
{
    Q_D(QtNavigationNode);
    d->isFooterNode = value;
}

bool QtNavigationNode::isHasFooterPage() const
{
    Q_D(const QtNavigationNode);
    return d->isHasFooterPage;
}
void QtNavigationNode::setIsHasFooterPage(bool value)
{
    Q_D(QtNavigationNode);
    d->isHasFooterPage = value;
}

bool QtNavigationNode::isExpanderNode() const
{
    Q_D(const QtNavigationNode);
    return d->isExpanderNode;
}
void QtNavigationNode::setIsExpanderNode(bool value)
{
    Q_D(QtNavigationNode);
    d->isExpanderNode = value;
}

bool QtNavigationNode::isCategoryNode() const
{
    Q_D(const QtNavigationNode);
    return d->isCategoryNode;
}
void QtNavigationNode::setIsCategoryNode(bool value)
{
    Q_D(QtNavigationNode);
    d->isCategoryNode = value;
}

bool QtNavigationNode::isVisible() const
{
    Q_D(const QtNavigationNode);
    return d->isVisible;
}
void QtNavigationNode::setIsVisible(bool value)
{
    Q_D(QtNavigationNode);
    d->isVisible = value;
}
QString QtNavigationNode::nodeTitle() const
{
    Q_D(const QtNavigationNode);
    return d->nodeTitle;
}
void QtNavigationNode::setNodeTitle(QString value)
{
    Q_D(QtNavigationNode);
    d->nodeTitle = value;
}
