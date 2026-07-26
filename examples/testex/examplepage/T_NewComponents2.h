#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qwidget.h>
#include "widgets/qttableview.h"
class T_NewComponents2: public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_NewComponents2(QWidget *parent = nullptr);
    ~T_NewComponents2();

private:
    QtTableView* _tableView{nullptr};
Q_SIGNALS:
};
