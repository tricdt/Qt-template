#pragma once

#include "T_BasePage.h"
#include "widgets/qtcheckbox.h"
#include "widgets/qtcombobox.h"
#include "widgets/qtmessagebutton.h"
#include "widgets/qtmultiselectcombobox.h"
#include "widgets/qtprogressbar.h"
#include "widgets/qtprogressring.h"
#include "widgets/qtradiobutton.h"
#include "widgets/qtslider.h"
#include "widgets/qtspinbox.h"
#include "widgets/qttogglebutton.h"
#include "widgets/qttoggleswitch.h"
#include <QObject>


class T_BaseComponents : public T_BasePage
{
    Q_OBJECT

public:
    explicit T_BaseComponents(QWidget* parent = nullptr);
    ~T_BaseComponents();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    QtToggleSwitch* _toggleSwitch{ nullptr };
    QtToggleButton* _toggleButton{ nullptr };
    QtComboBox* _comboBox{ nullptr };
    QtMultiSelectComboBox* _multiSelectComboBox{ nullptr };
    QtMessageButton* _messageButton{ nullptr };
    QtMessageButton* _infoMessageButton{ nullptr };
    QtMessageButton* _warningMessageButton{ nullptr };
    QtMessageButton* _errorMessageButton{ nullptr };
    QtCheckBox* _checkBox{ nullptr };
    QtSpinBox* _spinBox{ nullptr };
    QtSlider* _slider{ nullptr };
    QtRadioButton* _radioButton{ nullptr };
    QtProgressBar* _progressBar{ nullptr };
    QtProgressRing* _progressRing{ nullptr };
    QtProgressRing* _progressPercentRing{ nullptr };
    QtProgressRing* _progressBusyRing{ nullptr };
    QtProgressRing* _progressBusyTransparentRing{ nullptr };

Q_SIGNALS:
};
