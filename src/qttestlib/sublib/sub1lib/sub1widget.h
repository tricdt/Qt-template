#pragma once

#include <QWidget>
#include "testlib.h"

class QTTESTLIB_EXPORT QtSub1Widget: public QWidget
{
    Q_OBJECT

public:
    explicit QtSub1Widget(QWidget *parent = nullptr);
    ~QtSub1Widget();
Q_SIGNALS:
};
