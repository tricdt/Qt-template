#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>
#include "qtdef.h"
#include "utils/qtnavigationnode.h"
class QtFooterModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtFooterModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QtFooterModel(QObject* parent = nullptr);
    ~QtFooterModel();

    QtNavigationType::NodeResult addFooterNode(QString footerTitle, QString& footerKey,
        bool isHasFooterPage, int keyPoints = 0, QtIconType::IconName awesome = QtIconType::None);
    int footerNodeCount() const;
    QtNavigationNode* navigationNode(QString footerKey);
    void removeNavigationNode(QString footerKey);

    QtNavigationNode* selectedNode() const;
    void setSelectedNode(QtNavigationNode* value);

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QScopedPointer<class QtFooterModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFooterModel)
};
