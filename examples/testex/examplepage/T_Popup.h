#pragma once

#include "T_BasePage.h"
#include "widgets/qtcalendar.h"
#include "widgets/qtcalendarpicker.h"
#include "widgets/qtcolordialog.h"
#include "widgets/qtdrawerarea.h"
#include "widgets/qtkeybinder.h"
#include "widgets/qtmessagedialog.h"
#include "widgets/qtroller.h"
#include "widgets/qttoolbutton.h"
#include <QWidget>
#include <qtmetamacros.h>
#include "widgets/qtrollerpicker.h"

class T_Popup : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_Popup(QWidget* parent = nullptr);
    ~T_Popup();
Q_SIGNALS:

private:
    QtToolButton* _toolButton{ nullptr };
    QtColorDialog* _colorDialog{ nullptr };
    QtCalendar* _calendar{ nullptr };
    QtCalendarPicker* _calendarPicker{ nullptr };
    QtKeyBinder* _keyBinder{ nullptr };
    QtRoller* _roller{ nullptr };
    QtRollerPicker* _timeRollerPicker{ nullptr };
    QtRollerPicker* _clockRollerPicker{ nullptr };
    QtDrawerArea* _drawer{ nullptr };
    QtMessageDialog* _messageDialog{ nullptr };
};
