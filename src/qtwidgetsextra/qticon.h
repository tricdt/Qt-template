#pragma once
#include <QObject>
#include "qtwidgetsextra.h"
#include "qtsingleton.h"
#include "qtdef.h"
class QTWIDGETSEXTRA_EXPORT QtIcon
{
    Q_SINGLETON_CREATE_H(QtIcon)
private:
    explicit QtIcon();
    ~QtIcon();

public:
    QIcon getQtIcon(QtIconType::IconName awesome);
    QIcon getQtIcon(QtIconType::IconName awesome, QColor iconColor);
    QIcon getQtIcon(QtIconType::IconName awesome, int pixelSize);
    QIcon getQtIcon(QtIconType::IconName awesome, int pixelSize, QColor iconColor);
    QIcon getQtIcon(QtIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight);
    QIcon getQtIcon(QtIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);
};
