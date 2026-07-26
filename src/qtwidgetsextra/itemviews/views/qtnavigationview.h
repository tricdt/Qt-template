#pragma once

#include "qtwidgetsextra.h"
#include "widgets/qtnavigationbar.h"
#include <QWidget>
#include <qtreeview.h>

class QtNavigationViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationView : public QTreeView
{
    Q_OBJECT

public:
    explicit QtNavigationView(QWidget* parent = nullptr);
    ~QtNavigationView();

    QtNavigationBar* navigationBar() const;
    void setNavigationBar(QtNavigationBar*);

    void navigationNodeStateChange(QVariantMap data);

    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);

Q_SIGNALS:
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    Q_SIGNAL void navigationOpenNewWindow(QString nodeKey);

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QScopedPointer<class QtNavigationViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationView)
};
