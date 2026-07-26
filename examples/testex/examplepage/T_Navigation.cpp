#include "T_Navigation.h"
#include "widgets/qtpushbutton.h"
#include "widgets/qtscrollpagearea.h"
#include "widgets/qttext.h"
#include <qboxlayout.h>
T_Navigation::T_Navigation(QWidget* parent)
    : T_BasePage{ parent }
{
    // 预览窗口标题
    setWindowTitle("QtNavigation");

    // 顶部元素
    createCustomWidget("一些导航组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    // QtBreadcrumbBar
    QtText* breadcrumbBarText = new QtText("QtBreadcrumbBar", this);
    breadcrumbBarText->setTextPixelSize(18);
    _breadcrumbBar = new QtBreadcrumbBar(this);
    QStringList breadcrumbBarList;
    for(int i = 0; i < 20; i++)
    {
        breadcrumbBarList << QString("Item%1").arg(i + 1);
    }
    _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);

    QtPushButton* resetButton = new QtPushButton("还原", this);
    resetButton->setFixedSize(60, 32);
    connect(resetButton, &QtPushButton::clicked, this,
        [this, breadcrumbBarList]() { _breadcrumbBar->setBreadcrumbList(breadcrumbBarList); });

    QHBoxLayout* breadcrumbBarTextLayout = new QHBoxLayout();
    breadcrumbBarTextLayout->addWidget(breadcrumbBarText);
    breadcrumbBarTextLayout->addSpacing(15);
    breadcrumbBarTextLayout->addWidget(resetButton);
    breadcrumbBarTextLayout->addStretch();

    QtScrollPageArea* breadcrumbBarArea = new QtScrollPageArea(this);
    QVBoxLayout* breadcrumbBarLayout = new QVBoxLayout(breadcrumbBarArea);
    breadcrumbBarLayout->addWidget(_breadcrumbBar);

    QtText* pivotText = new QtText("QtPivot", this);
    pivotText->setTextPixelSize(18);
    _pivot = new QtPivot(this);
    _pivot->setPivotSpacing(8);
    _pivot->setMarkWidth(75);
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->setCurrentIndex(0);

    QtScrollPageArea* pivotArea = new QtScrollPageArea(this);
    QVBoxLayout* pivotLayout = new QVBoxLayout(pivotArea);
    pivotLayout->addWidget(_pivot);

    QtText* tabWidgetText = new QtText("QtTabWidget", this);
    tabWidgetText->setTextPixelSize(18);
    _tabWidget = new QtTabWidget(this);
    _tabWidget->setFixedHeight(600);
    _tabWidget->setIsTabTransparent(true);
    QtText* page1 = new QtText("新标签页", this);
    page1->setTextPixelSize(32);
    page1->setAlignment(Qt::AlignCenter);
    _tabWidget->addTab(page1, QIcon(":/include/Cirno.png"), "新标签页");
    for(int i = 0; i < 5; i++)
    {
        QtText* page = new QtText(QString("新标签页%1").arg(i), this);
        page->setTextPixelSize(32);
        page->setAlignment(Qt::AlignCenter);
        _tabWidget->addTab(page, QString("新标签页%1").arg(i));
    }

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("QtNavigation");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(breadcrumbBarTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(breadcrumbBarArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(pivotText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(pivotArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(tabWidgetText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(_tabWidget);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Navigation::~T_Navigation()
{
}