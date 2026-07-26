#include "T_NewComponents2.h"
#include <qboxlayout.h>
#include <qheaderview.h>
T_NewComponents2::T_NewComponents2(QWidget* parent)
    : T_BasePage{ parent }
{
    setWindowTitle("QtTableWidget");

    createCustomWidget("表格视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    
}
T_NewComponents2::~T_NewComponents2()
{
}
