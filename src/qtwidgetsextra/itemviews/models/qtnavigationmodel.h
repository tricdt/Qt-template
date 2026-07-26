#pragma once

#include "qtwidgetsextra.h"
#include "utils/qtnavigationnode.h"
#include <QWidget>
#include <qabstractitemmodel.h>

class QtNavigationModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit QtNavigationModel(QObject* parent = nullptr);
    ~QtNavigationModel();

    // void setIsMaximalMode(bool isMaximal);
    // bool isMaximalMode();

    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(
        int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QtNavigationType::NodeResult addExpanderNode(
        const QString& expanderTitle, QString& expanderKey, QtIconType::IconName awesome);
    QtNavigationType::NodeResult addExpanderNode(const QString& expanderTitle,
        QString& expanderKey, const QString& targetExpanderKey, QtIconType::IconName awesome);
    QtNavigationType::NodeResult addPageNode(
        const QString& pageTitle, QString& pageKey, QtIconType::IconName awesome);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QString& pageKey,
        const QString& targetExpanderKey, QtIconType::IconName awesome);
    QtNavigationType::NodeResult addPageNode(
        const QString& pageTitle, QString& pageKey, int keyPoints, QtIconType::IconName awesome);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QString& pageKey,
        const QString& targetExpanderKey, int keyPoints, QtIconType::IconName awesome);
    QtNavigationType::NodeResult addCategoryNode(
        const QString& categoryTitle, QString& categoryKey);
    QtNavigationType::NodeResult addCategoryNode(
        const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey);

    QStringList removeNavigationNode(const QString& nodeKey);

    QtNavigationNode* navigationNode(const QString& nodeKey) const;
    QList<QtNavigationNode*> rootExpanderNodes() const;
    QList<QtNavigationNode*> rootExpandedNodes() const;

    QList<QtNavigationNode*> rootCategoryNodes() const;

    QtNavigationNode* selectedNode() const;
    void setSelectedNode(QtNavigationNode* value);

    QtNavigationNode* selectedExpandedNode() const;
    void setSelectedExpandedNode(QtNavigationNode* value);

private:
    QScopedPointer<class QtNavigationModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationModel)
};
