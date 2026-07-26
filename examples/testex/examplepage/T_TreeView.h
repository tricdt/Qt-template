#pragma once

#include "T_BasePage.h"
#include "widgets/qttreeview.h"
#include <QObject>

class T_TreeView : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_TreeView(QWidget* parent = nullptr);

private:
    QtTreeView* _treeView{ nullptr };
Q_SIGNALS:
};
