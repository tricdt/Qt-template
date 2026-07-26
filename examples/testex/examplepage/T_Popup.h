#pragma once

#include <QWidget>
#include <qtmetamacros.h>

class T_Popup: public QWidget
{
    Q_OBJECT

public:
    explicit T_Popup(QWidget *parent = nullptr);

Q_SIGNALS:
};
