#pragma once

#include "T_BasePage.h"
#include "widgets/qtlistview.h"
#include <QObject>

class T_ListView : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_ListView(QWidget* parent = nullptr);
    ~T_ListView() override;

private:
    QtListView* _listView{ nullptr };
Q_SIGNALS:
};
