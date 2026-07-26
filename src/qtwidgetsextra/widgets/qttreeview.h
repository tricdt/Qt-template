#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>
#include <qtreeview.h>

class QtTreeViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtTreeView : public QTreeView
{
    Q_OBJECT
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int headerMargin READ headerMargin WRITE setHeaderMargin NOTIFY headerMarginChanged)
public:
    explicit QtTreeView(QWidget* parent = nullptr);
    ~QtTreeView();

    int itemHeight() const;
    void setItemHeight(int value);

    int headerMargin() const;
    void setHeaderMargin(int value);

Q_SIGNALS:
    void itemHeightChanged();
    void headerMarginChanged();

private:
    QScopedPointer<class QtTreeViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTreeView)
};
