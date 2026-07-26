#pragma once

#include <QObject>
#include "widgets/qtscrollpage.h"
class T_BasePage: public QtScrollPage
{
    Q_OBJECT

public:
    explicit T_BasePage(QWidget *parent = nullptr);

Q_SIGNALS:
};
