#pragma once

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
};
