#pragma once

#include <QWidget>
#include <qproxystyle.h>
#include "qtwidgetsextra.h"
class QtWindowStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtWindowStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit QtWindowStyle(QStyle* style = nullptr);
    ~QtWindowStyle();

private:
    QScopedPointer<class QtWindowStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWindowStyle)
};
