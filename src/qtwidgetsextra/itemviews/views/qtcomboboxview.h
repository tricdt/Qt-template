#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlistview.h>

class QtComboBoxViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtComboBoxView : public QListView
{
    Q_OBJECT

public:
    explicit QtComboBoxView(QWidget* parent = nullptr);
    ~QtComboBoxView();

Q_SIGNALS:
    void itemPressed(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QScopedPointer<class QtComboBoxViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtComboBoxView)
};
