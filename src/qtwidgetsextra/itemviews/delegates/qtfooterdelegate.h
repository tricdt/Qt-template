#pragma once

#include "itemviews/views/qtbaselistview.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qabstractitemmodel.h>
#include <qstyleditemdelegate.h>

class QtFooterDelegatePrivate;
class QTWIDGETSEXTRA_EXPORT QtFooterDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QtFooterDelegate(QObject* parent = nullptr);
    ~QtFooterDelegate();
    void navigationNodeStateChange(QVariantMap data);

    QtBaseListView* qtListView() const;
    void setQtListView(QtBaseListView* value);

    QModelIndex pressIndex() const;
    void setPressIndex(QModelIndex value);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QScopedPointer<class QtFooterDelegatePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFooterDelegate)
};
