#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qtmetamacros.h>

class T_Home: public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_Home(QWidget *parent = nullptr);

Q_SIGNALS:
};
