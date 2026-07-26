#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlistview.h>

class QtBaseListViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtBaseListView : public QListView
{
    Q_OBJECT

public:
    explicit QtBaseListView(QWidget* parent = nullptr);
    ~QtBaseListView();

Q_SIGNALS:
    void mousePress(const QModelIndex& index);
    void mouseRelease(const QModelIndex& index);
    void mouseDoubleClick(const QModelIndex& index);

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QScopedPointer<class QtBaseListViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtBaseListView)
};
