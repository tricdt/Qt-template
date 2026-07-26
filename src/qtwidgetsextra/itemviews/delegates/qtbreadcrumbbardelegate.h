#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qstyleditemdelegate.h>

class QtBreadcrumbBarDelegatePrivate;
class QTWIDGETSEXTRA_EXPORT QtBreadcrumbBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QtBreadcrumbBarDelegate(QObject* parent = nullptr);
    ~QtBreadcrumbBarDelegate();

    QModelIndex pressIndex() const;
    void setPressIndex(QModelIndex value);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;

private:
    QScopedPointer<class QtBreadcrumbBarDelegatePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtBreadcrumbBarDelegate)
};
