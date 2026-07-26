#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>

class QtPivotModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtPivotModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QtPivotModel(QObject* parent = nullptr);
    ~QtPivotModel();

    void appendPivot(QString pivot);
    void removePivot(QString pivot);

    int pivotListCount() const;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QScopedPointer<class QtPivotModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPivotModel)
};
