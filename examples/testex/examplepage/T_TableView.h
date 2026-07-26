#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qwidget.h>

class T_TableView: public T_BasePage
{
    Q_OBJECT

public:
    explicit T_TableView(QWidget *parent = nullptr);

Q_SIGNALS:
};
