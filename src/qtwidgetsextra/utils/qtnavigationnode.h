#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
class QtNavigationNodePrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QtNavigationNode*> childrenNodes READ childrenNodes WRITE setChildrenNodes
            NOTIFY childrenNodesChanged)
    Q_PROPERTY(
        QtNavigationNode* parentNode READ parentNode WRITE setParentNode NOTIFY parentNodeChanged)
    Q_PROPERTY(QtIconType::IconName awesome READ awesome WRITE setAwesome NOTIFY awesomeChanged)
    Q_PROPERTY(QModelIndex modelIndex READ modelIndex WRITE setModelIndex NOTIFY modelIndexChanged)
    Q_PROPERTY(int keyPoints READ keyPoints WRITE setKeyPoints NOTIFY keyPointsChanged)
    Q_PROPERTY(int depth READ depth WRITE setDepth NOTIFY depthChanged)
    Q_PROPERTY(bool isRootNode READ isRootNode WRITE setIsRootNode NOTIFY isRootNodeChanged)
    Q_PROPERTY(bool isFooterNode READ isFooterNode WRITE setIsFooterNode NOTIFY isFooterNodeChanged)
    Q_PROPERTY(bool isHasFooterPage READ isHasFooterPage WRITE setIsHasFooterPage NOTIFY
            isHasFooterPageChanged)
    Q_PROPERTY(bool isExpanderNode READ isExpanderNode WRITE setIsExpanderNode NOTIFY
            isExpanderNodeChanged)
    Q_PROPERTY(bool isCategoryNode READ isCategoryNode WRITE setIsCategoryNode NOTIFY
            isCategoryNodeChanged)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged)
    Q_PROPERTY(QString nodeTitle READ nodeTitle WRITE setNodeTitle NOTIFY nodeTitleChanged)
public:
    explicit QtNavigationNode(const QString& nodeTitle, QtNavigationNode* parent = nullptr);
    ~QtNavigationNode();

    QString nodeKey() const;

    void setIsExpanded(bool isExpanded);
    bool isExpanded() const;

    void setChildVisible(bool isVisible);
    bool isHasChild() const;
    bool isHasPageChild() const;

    void appendChildNode(QtNavigationNode* childNode);
    void removeChildNode(QtNavigationNode* childNode);

    bool isChildHasKeyPoints() const;

    QtNavigationNode* originalNode();
    bool isChildNode(QtNavigationNode* node);

    int row() const;
    int rowExceptCategoryNodes() const;

    QList<QtNavigationNode*> exceptCategoryNodes();

    QList<QtNavigationNode*> childrenNodes() const;
    void setChildrenNodes(QList<QtNavigationNode*> value);

    QtNavigationNode* parentNode() const;
    void setParentNode(QtNavigationNode* value);

    QtIconType::IconName awesome() const;
    void setAwesome(QtIconType::IconName value);

    const QModelIndex& modelIndex() const;
    void setModelIndex(QModelIndex& value);

    int keyPoints() const;
    void setKeyPoints(int value);

    int depth() const;
    void setDepth(int value);

    bool isRootNode() const;
    void setIsRootNode(bool value);

    bool isFooterNode() const;
    void setIsFooterNode(bool value);

    bool isHasFooterPage() const;
    void setIsHasFooterPage(bool value);

    bool isExpanderNode() const;
    void setIsExpanderNode(bool value);

    bool isCategoryNode() const;
    void setIsCategoryNode(bool value);

    bool isVisible() const;
    void setIsVisible(bool value);

    QString nodeTitle() const;
    void setNodeTitle(QString value);

Q_SIGNALS:
    void childrenNodesChanged();
    void parentNodeChanged();
    void awesomeChanged();
    void modelIndexChanged();
    void keyPointsChanged();
    void depthChanged();
    void isRootNodeChanged();
    void isFooterNodeChanged();
    void isHasFooterPageChanged();
    void isExpanderNodeChanged();
    void isCategoryNodeChanged();
    void isVisibleChanged();
    void nodeTitleChanged();

private:
    QScopedPointer<class QtNavigationNodePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationNode)
};
