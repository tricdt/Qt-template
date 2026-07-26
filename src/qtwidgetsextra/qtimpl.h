#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
class QtImplPrivate;
class QTWIDGETSEXTRA_EXPORT QtImpl : public QWidget
{
    Q_OBJECT

public:
    explicit QtImpl(QWidget* parent = nullptr);
    ~QtImpl();

private:
    QScopedPointer<class QtImplPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtImpl)
};
