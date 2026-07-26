#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>

class QtBreadcrumbBarModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtBreadcrumbBarModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QtBreadcrumbBarModel(QWidget* parent = nullptr);
    ~QtBreadcrumbBarModel();

    void appendBreadcrumb(QString breadcrumb);
    void removeBreadcrumb(QString breadcrumb);
    void removeBreadcrumb(int index);

    void setBreadcrumbList(QStringList breadcrumbList);

    int breadcrumbListCount();
    QStringList breadcrumbList();

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QScopedPointer<class QtBreadcrumbBarModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtBreadcrumbBarModel)
};
