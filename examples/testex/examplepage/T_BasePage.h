#pragma once

#include "widgets/qtscrollpage.h"
#include <QObject>

class T_BasePage : public QtScrollPage
{
    Q_OBJECT

public:
    explicit T_BasePage(QWidget* parent = nullptr);
    ~T_BasePage();

protected:
    void createCustomWidget(QString desText);
Q_SIGNALS:
};
