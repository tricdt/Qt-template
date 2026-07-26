#include "mainwindow.h"
#include "qttheme.h"

MainWindow::MainWindow(QWidget* parent)
    : QtWindow{ parent }
{
    initWindow();
    initEdgeLayout();
    initContent();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initWindow()
{
    setFocusPolicy(Qt::StrongFocus);
    // setIsCentralStackedWidgetTransparent(true);
    setWindowIcon(QIcon(":/include/Cirno.png"));
    resize(1200, 740);
    qtTheme->setThemeMode(QtThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(QtNavigationType::Compact);
    // setWindowButtonFlag(QtAppBarType::MinimizeButtonHint, false);
    // setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
}
void MainWindow::initEdgeLayout()
{
}
void MainWindow::initContent()
{
    _homePage = new T_Home(this);
#ifdef Q_OS_WIN
    _qtScreenPage = new T_QtScreen(this);
#endif
    _iconPage = new T_Icon(this);
    _baseComponentsPage = new T_BaseComponents(this);
    _graphicsPage = new T_Graphics(this);
    _navigationPage = new T_Navigation(this);
    _popupPage = new T_Popup(this);
    _cardPage = new T_Card(this);
    _listViewPage = new T_ListView(this);
    _tableViewPage = new T_TableView(this);
    _treeViewPage = new T_TreeView(this);
    _settingPage = new T_Setting(this);

    QString testKey_1;
    QString testKey_2;
    addPageNode("HOME", _homePage, QtIconType::House);
#ifdef Q_OS_WIN
    addExpanderNode("QtDxgi", _qtDxgiKey, QtIconType::TvMusic);
    QString dxgiCategoryKey;
    addCategoryNode("Windows-DXGI", dxgiCategoryKey, _qtDxgiKey);
    addPageNode("QtScreen", _qtScreenPage, _qtDxgiKey, 3, QtIconType::ObjectGroup);
#endif
    QString controlCategoryKey;
    addCategoryNode("Controls", controlCategoryKey);
    // navigation(elaScreenWidget->property("ElaPageKey").toString());
    addPageNode("QtBaseComponents", _baseComponentsPage, QtIconType::CabinetFiling);

    addExpanderNode("QtView", _viewKey, QtIconType::CameraViewfinder);
    QString viewCategoryKey;
    addCategoryNode("View Content", viewCategoryKey, _viewKey);
    addPageNode("QtListView", _listViewPage, _viewKey, 9, QtIconType::List);
    addPageNode("QtTableView", _tableViewPage, _viewKey, QtIconType::Table);
    addPageNode("QtTreeView", _treeViewPage, _viewKey, QtIconType::ListTree);
    expandNavigationNode(_viewKey);
    addPageNode("QtGraphics", _graphicsPage, 9, QtIconType::Paintbrush);
    addPageNode("QtCard", _cardPage, QtIconType::Cards);
    QString customKey;
    addCategoryNode("Custom", customKey);
    addPageNode("ElaNavigation", _navigationPage, QtIconType::LocationArrow);
    addPageNode("ElaPopup", _popupPage, QtIconType::Envelope);
    addPageNode("ElaIcon", _iconPage, 99, QtIconType::FontCase);
    addExpanderNode("TEST_EXPAND_NODE1", testKey_1, QtIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE2", testKey_2, testKey_1, QtIconType::Acorn);
    addPageNode("TEST_NODE3", new QWidget(this), testKey_2, QtIconType::Acorn);
    for(int i = 0; i < 10; i++)
    {
        addExpanderNode(
            QString("TEST_EXPAND_NODE%1").arg(i + 4), testKey_1, testKey_2, QtIconType::Acorn);
    }
    addExpanderNode("TEST_EXPAND_NODE14", testKey_1, QtIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE5", testKey_1, QtIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE16", testKey_1, QtIconType::Acorn);

    addFooterNode("About", nullptr, _aboutKey, 0, QtIconType::User);
    //     _aboutPage = new T_About();

    //     _aboutPage->hide();
    //     connect(this, &ElaWindow::navigationNodeClicked, this,
    //         [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
    //             if(_aboutKey == nodeKey)
    //             {
    //                 _aboutPage->moveToCenter();
    //                 _aboutPage->show();
    //             }
    //         });
    addFooterNode("Setting", _settingPage, _settingKey, 0, QtIconType::GearComplex);
    //     connect(this, &MainWindow::userInfoCardClicked, this,
    //         [=]() { this->navigation(_homePage->property("ElaPageKey").toString()); });
    // #ifdef Q_OS_WIN
    //     connect(_homePage, &T_Home::elaScreenNavigation, this,
    //         [=]() { this->navigation(_elaScreenPage->property("ElaPageKey").toString()); });
    // #endif
    //     connect(_homePage, &T_Home::elaBaseComponentNavigation, this,
    //         [=]() { this->navigation(_baseComponentsPage->property("ElaPageKey").toString()); });
    //     connect(_homePage, &T_Home::elaSceneNavigation, this,
    //         [=]() { this->navigation(_graphicsPage->property("ElaPageKey").toString()); });
    //     connect(_homePage, &T_Home::elaIconNavigation, this,
    //         [=]() { this->navigation(_iconPage->property("ElaPageKey").toString()); });
    //     connect(_homePage, &T_Home::elaCardNavigation, this,
    //         [=]() { this->navigation(_cardPage->property("ElaPageKey").toString()); });

    //     _windowSuggestBox->addSuggestion(getNavigationSuggestDataList());
    //     qDebug() << "已注册的事件列表" << ElaEventBus::getInstance()->getRegisteredEventsName();
}
