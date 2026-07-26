#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlistview.h>

class QtListViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtListView : public QListView
{
    Q_OBJECT
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged);
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged);

public:
    explicit QtListView(QWidget* parent = nullptr);
    ~QtListView();

    int itemHeight() const;
    void setItemHeight(int value);

    bool isTransparent() const;
    void setIsTransparent(bool value);

Q_SIGNALS:
    void itemHeightChanged();
    void isTransparentChanged();

private:
    QScopedPointer<class QtListViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtListView)
};
