#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qtmetamacros.h>

class T_Setting: public T_BasePage
{
    Q_OBJECT

public:
    explicit T_Setting(QWidget *parent = nullptr);

Q_SIGNALS:
};
