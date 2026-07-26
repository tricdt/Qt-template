#pragma once

#include "T_BasePage.h"
#include "widgets/qttablewidget.h"
#include <QObject>
#include <qwidget.h>
class T_TableWidget: public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_TableWidget(QWidget *parent = nullptr);
    ~T_TableWidget();

private:
    QtTableWidget* _tableWidget{nullptr};
Q_SIGNALS:
};
