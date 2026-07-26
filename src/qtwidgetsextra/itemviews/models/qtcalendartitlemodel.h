#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>

class QtCalendarTitleModelPrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendarTitleModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QtCalendarTitleModel(QObject* parent = nullptr);
    ~QtCalendarTitleModel();

protected:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

private:
    QScopedPointer<class QtCalendarTitleModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendarTitleModel)
};
