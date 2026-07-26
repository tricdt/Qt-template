#pragma once

#include "T_BasePage.h"
#include <QObject>

class T_ListView: public T_BasePage
{
    Q_OBJECT

public:
    explicit T_ListView(QWidget *parent = nullptr);

Q_SIGNALS:
};
