#pragma once

#include "widgets/qtcheckbox.h"
#include "widgets/qtcombobox.h"
#include "widgets/qtmessagebutton.h"
#include "widgets/qtmultiselectcombobox.h"
#include "widgets/qtprogressbar.h"
#include "widgets/qtradiobutton.h"
#include "widgets/qtslider.h"
#include "widgets/qtspinbox.h"
#include "widgets/qttogglebutton.h"
#include "widgets/qttoggleswitch.h"
#include "widgets/qtwidget.h"
#include "widgets/qtprogressring.h"

#include <QObject>

class Widget : public QtWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget();

private:
    QtThemeType::ThemeMode _themeMode;
    QtToggleSwitch* _toggleSwitch;
    QtToggleButton* _toggleButton;
    QtComboBox* _comboBox;
    QtMultiSelectComboBox* _multiSelectComboBox;
    QtMessageButton* _messageButton;
    QtMessageButton* _infoMessageButton;
    QtMessageButton* _warningMessageButton;
    QtMessageButton* _errorMessageButton;
    QtCheckBox* _checkBox;
    QtSpinBox* _spinBox;
    QtSlider* _slider;
    QtRadioButton* _radioButton;
    QtProgressBar* _progressBar;
    QtProgressRing* _progressRing;
    QtProgressRing* _progressPercentRing;
    QtProgressRing* _progressBusyRing;
    QtProgressRing* _progressBusyTransparentRing;

Q_SIGNALS:
};
