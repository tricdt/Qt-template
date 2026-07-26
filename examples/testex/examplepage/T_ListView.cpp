#include "T_ListView.h"
#include "modelview/T_ListViewModel.h"
#include "widgets/qtscrollbar.h"
#include "widgets/qttext.h"
#include <qboxlayout.h>

T_ListView::T_ListView(QWidget* parent)
    : T_BasePage{ parent }
{
    setWindowTitle("QtListView");

    createCustomWidget("列表视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    QtText* listText = new QtText("QtListView", this);
    listText->setTextPixelSize(18);
    _listView = new QtListView(this);
    _listView->setFixedHeight(450);
    // _listView->setAlternatingRowColors(true);
    _listView->setModel(new T_ListViewModel(this));
    QtScrollBar* listViewFloatScrollBar =
        new QtScrollBar(_listView->verticalScrollBar(), _listView);
    listViewFloatScrollBar->setIsAnimation(true);
    QHBoxLayout* listViewLayout = new QHBoxLayout();
    listViewLayout->setContentsMargins(0, 0, 10, 0);
    listViewLayout->addWidget(_listView);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("QtView");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(listText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(listViewLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_ListView::~T_ListView()
{
}