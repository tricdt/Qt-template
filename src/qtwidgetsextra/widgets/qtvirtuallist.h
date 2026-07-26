#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
class QtVirtualListPrivate;
class QTWIDGETSEXTRA_EXPORT QtVirtualList : public QWidget
{
    Q_OBJECT
Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged)
    Q_PROPERTY(bool isAlternatingRowColors READ isAlternatingRowColors WRITE setIsAlternatingRowColors NOTIFY isAlternatingRowColorsChanged)
public:
    explicit QtVirtualList(QWidget* parent = nullptr);
    ~QtVirtualList();

    int itemHeight() const;
    void setItemHeight(int value);

    bool isTransparent() const;
    void setIsTransparent(bool value);

    bool isAlternatingRowColors() const;
    void setIsAlternatingRowColors(bool value);

Q_SIGNALS:
    void itemHeightChanged();
    void isTransparentChanged();
    void isAlternatingRowColorsChanged();

private:
    QScopedPointer<class QtVirtualListPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtVirtualList)
};
