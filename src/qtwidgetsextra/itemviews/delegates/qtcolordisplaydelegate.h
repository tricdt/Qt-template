#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qstyleditemdelegate.h>

class QtColorDisplayDelegatePrivate;
class QTWIDGETSEXTRA_EXPORT QtColorDisplayDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QtColorDisplayDelegate(QObject* parent = nullptr);
    ~QtColorDisplayDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QScopedPointer<class QtColorDisplayDelegatePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorDisplayDelegate)
};
