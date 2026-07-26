#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtDrawerAreaPrivate;
class QTWIDGETSEXTRA_EXPORT QtDrawerArea : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int headerHeight READ headerHeight WRITE setHeaderHeight NOTIFY headerHeightChanged)
public:
    explicit QtDrawerArea(QWidget* parent = nullptr);
    ~QtDrawerArea();
    void setDrawerHeader(QWidget* widget);

    void addDrawer(QWidget* widget);
    void removeDrawer(QWidget* widget);

    void expand();
    void collapse();

    bool isExpand() const;

    int borderRadius() const;
    void setBorderRadius(int value);

    int headerHeight() const;
    void setHeaderHeight(int value);

Q_SIGNALS:
    void expandStateChanged(bool isExpand);

    void borderRadiusChanged();
    void headerHeightChanged();

private:
    QScopedPointer<class QtDrawerAreaPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDrawerArea)
};
