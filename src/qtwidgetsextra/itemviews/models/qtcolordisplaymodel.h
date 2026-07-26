#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qobject.h>

class QtColorDisplayModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtColorDisplayModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QtColorDisplayModel(QObject* parent = nullptr);
    ~QtColorDisplayModel();

    void appendDisplayColor(QList<QColor> colorList);
    void appendDisplayColor(QColor color);
    void removeDisplayColor(int index);
    void replaceDisplayColor(QColor color, int index);

    QList<QColor> displayColorList() const;
    QColor displayColor(int index) const;

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QScopedPointer<class QtColorDisplayModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorDisplayModel)
};
