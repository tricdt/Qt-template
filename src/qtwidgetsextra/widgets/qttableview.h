#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtableview.h>

class QtTableViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtTableView : public QTableView
{
    Q_OBJECT
    Q_PROPERTY(int headerMargin READ headerMargin WRITE setHeaderMargin NOTIFY headerMarginChanged)
public:
    explicit QtTableView(QWidget* parent = nullptr);
    ~QtTableView();

    int headerMargin() const;
    void setHeaderMargin(int value);

Q_SIGNALS:
    void headerMarginChanged();
    void tableViewShow();
    void tableViewHide();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    QScopedPointer<class QtTableViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTableView)
};
