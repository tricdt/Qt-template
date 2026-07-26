#pragma once

#include "T_BasePage.h"
#include "widgets/qtbreadcrumbbar.h"
#include "widgets/qtpivot.h"
#include "widgets/qttabwidget.h"
#include <QWidget>

class T_Navigation : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_Navigation(QWidget* parent = nullptr);
    ~T_Navigation();
Q_SIGNALS:

private:
    QtBreadcrumbBar* _breadcrumbBar{ nullptr };
    QtPivot* _pivot{ nullptr };
    QtTabWidget* _tabWidget{ nullptr };
};
