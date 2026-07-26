#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qtmetamacros.h>

class T_About: public T_BasePage
{
    Q_OBJECT

public:
    explicit T_About(QWidget *parent = nullptr);

Q_SIGNALS:
};
