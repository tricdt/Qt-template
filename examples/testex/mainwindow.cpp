#include "mainwindow.h"

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
    // qtTheme->setThemeMode(QtThemeType::Dark);
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
}
