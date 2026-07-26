#pragma once

#include "examplepage/T_BaseComponents.h"
#include "examplepage/T_Card.h"
#include "examplepage/T_Graphics.h"
#include "examplepage/T_Home.h"
#include "examplepage/T_Icon.h"
#include "examplepage/T_ListView.h"
#include "examplepage/T_Navigation.h"
#include "examplepage/T_Popup.h"
#include "examplepage/T_QtScreen.h"
#include "examplepage/T_Setting.h"
#include "examplepage/T_TableView.h"
#include "examplepage/T_TreeView.h"
#include "widgets/qtwindow.h"
#include <QMainWindow>
#include <qmainwindow.h>

class MainWindow : public QtWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
//     ElaContentDialog* _closeDialog{ nullptr };
//     ElaSuggestBox* _windowSuggestBox{ nullptr };
    T_Home* _homePage{ nullptr };
#ifdef Q_OS_WIN
    T_QtScreen* _qtScreenPage{ nullptr };
#endif
    T_Icon* _iconPage{ nullptr };
    T_BaseComponents* _baseComponentsPage{ nullptr };
    T_Graphics* _graphicsPage{ nullptr };
    T_Navigation* _navigationPage{ nullptr };
    T_Popup* _popupPage{ nullptr };
    T_Card* _cardPage{ nullptr };
    T_ListView* _listViewPage{ nullptr };
    T_TableView* _tableViewPage{ nullptr };
    T_TreeView* _treeViewPage{ nullptr };
//     T_About* _aboutPage{ nullptr };
    T_Setting* _settingPage{ nullptr };
    QString _qtDxgiKey{ "" };
    QString _viewKey{ "" };
    QString _aboutKey{ "" };
    QString _settingKey{ "" };
};
