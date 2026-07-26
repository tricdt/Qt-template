#pragma once

#include "widgets/qttoolbutton.h"
#include <QWidget>
#include <qtmetamacros.h>
#include "T_BasePage.h"

class T_Popup : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_Popup(QWidget* parent = nullptr);
    ~T_Popup();
Q_SIGNALS:

private:
    QtToolButton* _toolButton{ nullptr };
    // QtColorDialog* _colorDialog{ nullptr };
    // QtCalendar* _calendar{ nullptr };
    // QtCalendarPicker* _calendarPicker{ nullptr };
    // QtKeyBinder* _keyBinder{ nullptr };
    // QtRoller* _roller{ nullptr };
    // QtRollerPicker* _timeRollerPicker{ nullptr };
    // QtRollerPicker* _clockRollerPicker{ nullptr };
    // QtDrawerArea* _drawer{ nullptr };
};
