#pragma once

#include "itemviews/models/qtcalendarmodel.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qstyleditemdelegate.h>

class QtCalendarDelegatePrivate;
class QTWIDGETSEXTRA_EXPORT QtCalendarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QtCalendarDelegate(QtCalendarModel* calendarModel, QObject* parent = nullptr);
    ~QtCalendarDelegate();

    bool isTransparent() const;
    void setIsTransparent(bool value);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QScopedPointer<class QtCalendarDelegatePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCalendarDelegate)
};
