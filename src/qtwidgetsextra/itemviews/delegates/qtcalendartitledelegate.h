#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qstyleditemdelegate.h>

class QtCalendarTitleDelegatePrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendarTitleDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QtCalendarTitleDelegate(QObject* parent = nullptr);
    ~QtCalendarTitleDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QScopedPointer<class QtCalendarTitleDelegatePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendarTitleDelegate)
};
