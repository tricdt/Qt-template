#pragma once

#include "T_BasePage.h"
#include <QObject>
#include <qwidget.h>
#include "widgets/qttableview.h"
class T_TableView: public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_TableView(QWidget *parent = nullptr);
    ~T_TableView();

private:
    QtTableView* _tableView{nullptr};
Q_SIGNALS:
};
