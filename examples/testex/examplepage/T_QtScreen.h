#pragma once

#include "T_BasePage.h"
#include <QWidget>
#include <qtmetamacros.h>

class T_QtScreen: public T_BasePage
{
    Q_OBJECT

public:
    explicit T_QtScreen(QWidget *parent = nullptr);

Q_SIGNALS:
};
