#include "widget.h"
#include "qtdef.h"
#include "qttheme.h"
#include "styles/qtradiobuttonstyle.h"
#include "widgets/qtplaintextedit.h"
#include "widgets/qtradiobutton.h"
#include "widgets/qtscrollpagearea.h"
#include "widgets/qtspinbox.h"
#include "widgets/qttext.h"
#include "widgets/qttogglebutton.h"
#include "widgets/qttoggleswitch.h"
#include <qboxlayout.h>

Widget::Widget(QWidget* parent)
    : QtWidget{ parent }
{
    _toggleSwitch = new QtToggleSwitch(this);
    QtScrollPageArea* toggleSwitchArea = new QtScrollPageArea(this);
    QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(toggleSwitchArea);

    QtText* toggleSwitchText = new QtText("QtToggleSwitch", this);
    toggleSwitchText->setTextPixelSize(15);

    toggleSwitchLayout->addWidget(toggleSwitchText);
    toggleSwitchLayout->addWidget(_toggleSwitch);
    toggleSwitchLayout->addStretch();

    QtToggleSwitch* toggleSwitchDisable = new QtToggleSwitch(this);
    QtText* toggleSwitchDisableText = new QtText("QtToggleSwitchDisable", this);
    toggleSwitchDisableText->setTextPixelSize(15);
    connect(toggleSwitchDisable, &QtToggleSwitch::toggled, this,
        [this](bool checked) { _toggleSwitch->setDisabled(checked); });

    toggleSwitchLayout->addWidget(toggleSwitchDisableText);
    toggleSwitchLayout->addWidget(toggleSwitchDisable);
    toggleSwitchLayout->addSpacing(10);

    _toggleButton = new QtToggleButton("Toggle Button", this);
    _toggleButton->setFixedWidth(120);

    QtText* toggleButtonText = new QtText("QtToggleButton", this);
    toggleButtonText->setTextPixelSize(15);

    QtScrollPageArea* toggleButtonArea = new QtScrollPageArea(this);
    QHBoxLayout* toggleButtonLayout = new QHBoxLayout(toggleButtonArea);

    toggleButtonLayout->addWidget(toggleButtonText);
    toggleButtonLayout->addWidget(_toggleButton);
    toggleButtonLayout->addStretch();
    QtToggleSwitch* toggleButtonDisableSwitch = new QtToggleSwitch(this);
    QtText* toggleButtonDisableText = new QtText("QtToggleButtonDiable", this);
    toggleButtonDisableText->setTextPixelSize(15);
    connect(toggleButtonDisableSwitch, &QtToggleSwitch::toggled, this,
        [this](bool checked) { _toggleButton->setDisabled(checked); });
    toggleButtonLayout->addWidget(toggleButtonDisableText);
    toggleButtonLayout->addWidget(toggleButtonDisableSwitch);
    toggleButtonLayout->addSpacing(10);

    _comboBox = new QtComboBox(this);
    QStringList comboList{ "我愿投身前途未卜的群星", "潜行 步伐小心翼翼", "不留游走痕迹",
        "如同一簇幽灵", "所谓 道德加上伦理", "抱歉只能律己" };
    _comboBox->addItems(comboList);
    QtScrollPageArea* comboBoxArea = new QtScrollPageArea(this);
    QHBoxLayout* comboBoxLayout = new QHBoxLayout(comboBoxArea);
    QtText* comboBoxText = new QtText("QtComboBox", this);
    comboBoxText->setTextPixelSize(15);
    comboBoxLayout->addWidget(comboBoxText);
    comboBoxLayout->addWidget(_comboBox);
    comboBoxLayout->addStretch();
    QtToggleSwitch* comboBoxDisableSwitch = new QtToggleSwitch(this);
    QtText* comboBoxDisableText = new QtText("QtcomboBoxDiable", this);
    comboBoxDisableText->setTextPixelSize(15);
    connect(comboBoxDisableSwitch, &QtToggleSwitch::toggled, this,
        [this](bool checked) { _comboBox->setDisabled(checked); });
    comboBoxLayout->addWidget(comboBoxDisableText);
    comboBoxLayout->addWidget(comboBoxDisableSwitch);
    comboBoxLayout->addSpacing(10);

    _multiSelectComboBox = new QtMultiSelectComboBox(this);
    QStringList multiComboList{ "执念的鱼", "提着灯闯过远洋的甄选", "继续下潜", "无需誓言",
        "我的心像自沉的旧母舰", "没入深渊", "我曾凝望曾是航向的日出" };
    QStringList multiSelectComboList{ "执念的鱼", "提着灯闯过远洋的甄选", "无需誓言",
        "我的心像自沉的旧母舰" };
    _multiSelectComboBox->addItems(multiComboList);
    // _multiSelectComboBox->setCurrentSelection(multiSelectComboList);
    QtScrollPageArea* multiSelectComboBoxArea = new QtScrollPageArea(this);
    QHBoxLayout* multiSelectComboBoxLayout = new QHBoxLayout(multiSelectComboBoxArea);
    QtText* multiSelectComboBoxText = new QtText("QtMutilSelectComboBox", this);
    multiSelectComboBoxText->setTextPixelSize(15);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxText);
    multiSelectComboBoxLayout->addWidget(_multiSelectComboBox);
    multiSelectComboBoxLayout->addStretch();
    QtToggleSwitch* multiSelectComboBoxDisableSwitch = new QtToggleSwitch(this);
    QtText* multiSelectComboBoxDisableText = new QtText("禁用", this);
    multiSelectComboBoxDisableText->setTextPixelSize(15);
    connect(multiSelectComboBoxDisableSwitch, &QtToggleSwitch::toggled, this,
        [this](bool checked) { _multiSelectComboBox->setDisabled(checked); });
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxDisableSwitch);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxDisableText);
    multiSelectComboBoxLayout->addSpacing(10);

    _messageButton = new QtMessageButton("Success", this);
    _messageButton->setBarTitle("Success");
    _messageButton->setBarText(
        "点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");

    _infoMessageButton = new QtMessageButton("Info", this);
    _infoMessageButton->setBarTitle("Information");
    _infoMessageButton->setBarText(
        "点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _infoMessageButton->setMessageMode(QtMessageBarType::Information);
    _infoMessageButton->setPositionPolicy(QtMessageBarType::TopLeft);

    _warningMessageButton = new QtMessageButton("Warning", this);
    _warningMessageButton->setBarTitle("Warning");
    _warningMessageButton->setBarText(
        "点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _warningMessageButton->setMessageMode(QtMessageBarType::Warning);
    _warningMessageButton->setPositionPolicy(QtMessageBarType::BottomLeft);

    _errorMessageButton = new QtMessageButton("Error", this);
    _errorMessageButton->setBarTitle("Error");
    _errorMessageButton->setBarText(
        "点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _errorMessageButton->setMessageMode(QtMessageBarType::Error);
    _errorMessageButton->setPositionPolicy(QtMessageBarType::BottomRight);

    QtScrollPageArea* messageButtonArea = new QtScrollPageArea(this);
    QHBoxLayout* messageButtonLayout = new QHBoxLayout(messageButtonArea);
    QtText* messageButtonText = new QtText("QtMessageButton", this);
    messageButtonText->setTextPixelSize(15);
    messageButtonLayout->addWidget(messageButtonText);
    messageButtonLayout->addWidget(_messageButton);
    messageButtonLayout->addWidget(_infoMessageButton);
    messageButtonLayout->addWidget(_warningMessageButton);
    messageButtonLayout->addWidget(_errorMessageButton);
    messageButtonLayout->addStretch();
    QtToggleSwitch* messageButtonDisableSwitch = new QtToggleSwitch(this);
    QtText* messageButtonDisableText = new QtText("禁用", this);
    messageButtonDisableText->setTextPixelSize(15);
    connect(messageButtonDisableSwitch, &QtToggleSwitch::toggled, this, [this](bool checked) {
        _messageButton->setDisabled(checked);
        _infoMessageButton->setDisabled(checked);
        _warningMessageButton->setDisabled(checked);
        _errorMessageButton->setDisabled(checked);
    });
    messageButtonLayout->addWidget(messageButtonDisableSwitch);
    messageButtonLayout->addWidget(messageButtonDisableText);
    messageButtonLayout->addSpacing(10);

    _checkBox = new QtCheckBox("CheckBox", this);
    _checkBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QtScrollPageArea* checkBoxArea = new QtScrollPageArea(this);
    QtText* checkBoxText = new QtText("QtCheckBox", this);
    checkBoxText->setTextPixelSize(15);

    QHBoxLayout* checkBoxLayout = new QHBoxLayout(checkBoxArea);
    checkBoxLayout->addWidget(checkBoxText);
    checkBoxLayout->addWidget(_checkBox);
    checkBoxLayout->addStretch();
    QtToggleSwitch* checkBoxDisableSwitch = new QtToggleSwitch(this);
    QtText* checkBoxDisableText = new QtText("禁用", this);
    checkBoxDisableText->setTextPixelSize(15);
    connect(checkBoxDisableSwitch, &QtToggleSwitch::toggled, this,
        [this](bool checked) { _checkBox->setDisabled(checked); });
    checkBoxLayout->addWidget(checkBoxDisableSwitch);
    checkBoxLayout->addWidget(checkBoxDisableText);
    checkBoxLayout->addSpacing(10);

    _spinBox = new QtSpinBox(this);
    QtScrollPageArea* spinBoxArea = new QtScrollPageArea(this);
    QHBoxLayout* spinBoxLayout = new QHBoxLayout(spinBoxArea);
    QtText* spinBoxText = new QtText("QtSpinBox", this);
    spinBoxText->setTextPixelSize(15);
    spinBoxLayout->addWidget(spinBoxText);
    spinBoxLayout->addWidget(_spinBox);
    spinBoxLayout->addStretch();

    QtRadioButton* inlineButton = new QtRadioButton("Inline", this);
    QtRadioButton* compactButton = new QtRadioButton("Compact", this);
    QtRadioButton* sideButton = new QtRadioButton("Side", this);
    QtRadioButton* pmSideButton = new QtRadioButton("PMSide", this);
    inlineButton->setChecked(true);
    QHBoxLayout* buttonModeLayout = new QHBoxLayout();
    QtText* buttonModeText = new QtText("按钮模式切换", this);
    buttonModeText->setWordWrap(false);
    buttonModeText->setTextPixelSize(15);
    buttonModeLayout->addWidget(buttonModeText);
    buttonModeLayout->addWidget(inlineButton);
    buttonModeLayout->addWidget(compactButton);
    buttonModeLayout->addWidget(sideButton);
    buttonModeLayout->addWidget(pmSideButton);
    spinBoxLayout->addLayout(buttonModeLayout);

    QButtonGroup* spinButtonGroup = new QButtonGroup(this);
    spinButtonGroup->addButton(inlineButton, 0);
    spinButtonGroup->addButton(compactButton, 1);
    spinButtonGroup->addButton(sideButton, 2);
    spinButtonGroup->addButton(pmSideButton, 3);
    connect(spinButtonGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
        this, [this, spinButtonGroup](QAbstractButton* button, bool isToggled) {
            if(isToggled)
            {
                _spinBox->setButtonMode((QtSpinBoxType::ButtonMode) spinButtonGroup->id(button));
            }
        });

    _slider = new QtSlider(this);
    QtScrollPageArea* sliderArea = new QtScrollPageArea(this);
    QHBoxLayout* sliderLayout = new QHBoxLayout(sliderArea);
    QtText* sliderText = new QtText("QtSlider", this);
    sliderText->setTextPixelSize(15);
    sliderLayout->addWidget(sliderText);
    sliderLayout->addWidget(_slider);
    sliderLayout->addStretch();

    _radioButton = new QtRadioButton("RadioButton", this);
    QtScrollPageArea* radioButtonArea = new QtScrollPageArea(this);
    QHBoxLayout* radioButtonLayout = new QHBoxLayout(radioButtonArea);
    QtText* radioButtonText = new QtText("QtRadioButton", this);
    radioButtonText->setTextPixelSize(15);
    radioButtonLayout->addWidget(radioButtonText);
    radioButtonLayout->addWidget(_radioButton);
    radioButtonLayout->addStretch();

    _progressBar = new QtProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0);
    QtScrollPageArea* progressBarArea = new QtScrollPageArea(this);
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    QtText* progressBarText = new QtText("QtProgressBar", this);
    progressBarText->setTextPixelSize(15);
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);
    progressBarLayout->addStretch();

    _progressRing = new QtProgressRing(this);
    _progressRing->setValue(30);
    _progressPercentRing = new QtProgressRing(this);
    _progressPercentRing->setValue(50);
    _progressPercentRing->setValueDisplayMode(QtProgressRingType::ValueDisplayMode::Percent);
    _progressBusyRing = new QtProgressRing(this);
    _progressBusyRing->setIsBusying(true);
    _progressBusyTransparentRing = new QtProgressRing(this);
    _progressBusyTransparentRing->setIsBusying(true);
    _progressBusyTransparentRing->setIsTransparent(true);
    QtScrollPageArea* progressRingArea = new QtScrollPageArea(this);
    progressRingArea->setFixedHeight(90);
    QHBoxLayout* progressRingLayout = new QHBoxLayout(progressRingArea);
    QtText* progressRingText = new QtText("QtProgressRing", this);
    progressRingText->setTextPixelSize(15);
    progressRingLayout->addWidget(progressRingText);
    progressRingLayout->addWidget(_progressRing);
    progressRingLayout->addSpacing(10);
    progressRingLayout->addWidget(_progressPercentRing);
    progressRingLayout->addSpacing(10);
    progressRingLayout->addWidget(_progressBusyRing);
    progressRingLayout->addSpacing(10);
    progressRingLayout->addWidget(_progressBusyTransparentRing);
    progressRingLayout->addStretch();

    QtPlainTextEdit* edit = new QtPlainTextEdit(this);
    edit->setPlainText("这是一个QtPlainTextEdit  暂时放在这里");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toggleSwitchArea);
    mainLayout->addWidget(toggleButtonArea);
    mainLayout->addWidget(comboBoxArea);
    mainLayout->addWidget(multiSelectComboBoxArea);
    mainLayout->addWidget(messageButtonArea);
    mainLayout->addWidget(checkBoxArea);
    mainLayout->addWidget(spinBoxArea);
    mainLayout->addWidget(sliderArea);
    mainLayout->addWidget(radioButtonArea);
    mainLayout->addWidget(progressBarArea);
    mainLayout->addWidget(progressRingArea);
    mainLayout->addWidget(edit);
    mainLayout->addStretch(1);

    QObject::connect(this, &QtWidget::themeChangeButtonClicked, []() {
        qtTheme->setThemeMode(
            qtTheme->getThemeMode() == QtThemeType::Dark ? QtThemeType::Light : QtThemeType::Dark);
    });
}

Widget::~Widget()
{
}
