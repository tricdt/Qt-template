#pragma once

#include "T_BasePage.h"
#include "widgets/qtlcdnumber.h"
#include "widgets/qtpromotioncard.h"
#include "widgets/qtpromotionview.h"
#include <QWidget>

class T_Card : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_Card(QWidget* parent = nullptr);
    ~T_Card();

private:
    QtLCDNumber* _lcdNumber{ nullptr };
    QtPromotionCard* _promotionCard{ nullptr };
    QtPromotionView* _promotionView{ nullptr };

Q_SIGNALS:
};
