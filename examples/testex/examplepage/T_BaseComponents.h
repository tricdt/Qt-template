#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qtmetamacros.h>

class T_BaseComponents: public T_BasePage
{
    Q_OBJECT

public:
    explicit T_BaseComponents(QWidget *parent = nullptr);

Q_SIGNALS:
};
