#include "T_Popup.h"
#include "widgets/qtcheckbox.h"
#include "widgets/qtinputdialog.h"
#include "widgets/qtmenu.h"
#include "widgets/qtpushbutton.h"
#include "widgets/qtscrollpagearea.h"
#include "widgets/qttext.h"
#include "widgets/qttoggleswitch.h"

T_Popup::T_Popup(QWidget* parent)
    : T_BasePage{ parent }
{
    setWindowTitle("QtPopup");

    createCustomWidget("Some commonly used popup components are placed here. You can experience "
                       "their effects on this interface and add them to your project as needed.");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("QtPopup");

    _toolButton = new QtToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _toolButton->setText("QtToolButton");

    QtMenu* menu = new QtMenu(this);
    menu->addQtIconAction(QtIconType::JackOLantern, "JackOLantern");
    menu->addQtIconAction(QtIconType::LacrosseStick, "LacrosseStick");
    _toolButton->setMenu(menu);
    _toolButton->setQtIcon(QtIconType::Broom);

    QtScrollPageArea* toolButtonArea = new QtScrollPageArea(this);
    QHBoxLayout* toolButtonLayout = new QHBoxLayout(toolButtonArea);
    QtText* toolButtonText = new QtText("QtToolButton", this);
    toolButtonText->setTextPixelSize(15);
    toolButtonLayout->addWidget(toolButtonText);
    toolButtonLayout->addWidget(_toolButton);
    toolButtonLayout->addStretch();

    _colorDialog = new QtColorDialog(this);
    QtText* colorText = new QtText(_colorDialog->currentColorRGB(), this);
    colorText->setTextPixelSize(15);
    QtPushButton* colorDialogButton = new QtPushButton(this);
    colorDialogButton->setFixedSize(35, 35);
    colorDialogButton->setLightDefaultColor(_colorDialog->currentColor());
    colorDialogButton->setLightHoverColor(_colorDialog->currentColor());
    colorDialogButton->setLightPressColor(_colorDialog->currentColor());
    colorDialogButton->setDarkDefaultColor(_colorDialog->currentColor());
    colorDialogButton->setDarkHoverColor(_colorDialog->currentColor());
    colorDialogButton->setDarkPressColor(_colorDialog->currentColor());
    connect(colorDialogButton, &QtPushButton::clicked, this, [this]() { _colorDialog->exec(); });
    connect(_colorDialog, &QtColorDialog::colorSelected, this,
        [this, colorDialogButton, colorText](const QColor& color) {
            colorDialogButton->setLightDefaultColor(color);
            colorDialogButton->setLightHoverColor(color);
            colorDialogButton->setLightPressColor(color);
            colorDialogButton->setDarkDefaultColor(color);
            colorDialogButton->setDarkHoverColor(color);
            colorDialogButton->setDarkPressColor(color);
            colorText->setText(_colorDialog->currentColorRGB());
        });

    QtScrollPageArea* colorDialogArea = new QtScrollPageArea(this);
    QHBoxLayout* colorDialogLayout = new QHBoxLayout(colorDialogArea);
    QtText* colorDialogText = new QtText("QtColorDialog", this);
    colorDialogText->setTextPixelSize(15);
    colorDialogLayout->addWidget(colorDialogText);
    colorDialogLayout->addWidget(colorDialogButton);
    colorDialogLayout->addWidget(colorText);
    colorDialogLayout->addStretch();

    QtScrollPageArea* inputDialogArea = new QtScrollPageArea(this);
    QHBoxLayout* inputDialogLayout = new QHBoxLayout(inputDialogArea);
    QtText* inputDialogText = new QtText("QtInputDialog", this);
    inputDialogText->setTextPixelSize(15);
    inputDialogLayout->addWidget(inputDialogText);

    QtPushButton* textInputButton = new QtPushButton("Text", this);
    textInputButton->setFixedSize(80, 38);
    connect(textInputButton, &QtPushButton::clicked, this, [this]() {
        bool ok;
        QString text = QtInputDialog::textInput(
            this, "Input Dialog", "Please enter your information", "Name:", "", &ok);
        if(ok && !text.isEmpty())
        {
            qDebug() << "textInput:" << text;
        }
    });
    inputDialogLayout->addWidget(textInputButton);

    QtPushButton* intInputButton = new QtPushButton("Int", this);
    intInputButton->setFixedSize(80, 38);
    connect(intInputButton, &QtPushButton::clicked, this, [this]() {
        bool ok;
        int value = QtInputDialog::intInput(
            this, "Integer Input", "Please enter the value", "Value:", 18, 0, 150, 1, &ok);
        if(ok)
        {
            qDebug() << "Integer input:" << value;
        }
    });
    inputDialogLayout->addWidget(intInputButton);

    QtPushButton* doubleInputButton = new QtPushButton("Double", this);
    doubleInputButton->setFixedSize(80, 38);
    connect(doubleInputButton, &QtPushButton::clicked, this, [this]() {
        bool ok;
        double value = QtInputDialog::doubleInput(
            this, "Double Input", "Product Pricing System", "Price:", 99.99, 0.0, 9999.99, 2, &ok);
        if(ok)
        {
            qDebug() << "Double input:" << value;
        }
    });
    inputDialogLayout->addWidget(doubleInputButton);

    QtPushButton* multiLineInputButton = new QtPushButton("Multi Line", this);
    multiLineInputButton->setFixedSize(80, 38);
    connect(multiLineInputButton, &QtPushButton::clicked, this, [this]() {
        bool ok;
        QString text = QtInputDialog::multiLineTextInput(
            this, "Multi-line Input", "Please enter the description", "Description:", "", &ok);
        if(ok && !text.isEmpty())
        {
            qDebug() << "Multi-line input:" << text;
        }
    });
    inputDialogLayout->addWidget(multiLineInputButton);
    inputDialogLayout->addStretch();

    _calendar = new QtCalendar(this);

    _calendarPicker = new QtCalendarPicker(this);
    QtScrollPageArea* calendarPickerArea = new QtScrollPageArea(this);
    QHBoxLayout* calendarPickerLayout = new QHBoxLayout(calendarPickerArea);
    QtText* calendarPickerText = new QtText("QtCalendarPicker", this);
    calendarPickerText->setTextPixelSize(15);
    calendarPickerLayout->addWidget(calendarPickerText);
    calendarPickerLayout->addWidget(_calendarPicker);
    calendarPickerLayout->addStretch();

    _keyBinder = new QtKeyBinder(this);
    QtScrollPageArea* keyBinderArea = new QtScrollPageArea(this);
    QHBoxLayout* keyBinderLayout = new QHBoxLayout(keyBinderArea);
    QtText* keyBinderText = new QtText("QtKeyBinder", this);
    keyBinderText->setTextPixelSize(15);
    keyBinderLayout->addWidget(keyBinderText);
    keyBinderLayout->addWidget(_keyBinder);
    keyBinderLayout->addStretch();

    _roller = new QtRoller(this);
    QStringList rollerItemList;
    for(int i = 0; i < 100; i++)
    {
        rollerItemList.append(QString::number(i + 1));
    }
    _roller->setItemList(rollerItemList);
    QtScrollPageArea* rollerArea = new QtScrollPageArea(this);
    rollerArea->setFixedHeight(220);
    QHBoxLayout* rollerLayout = new QHBoxLayout(rollerArea);
    QtText* rollerText = new QtText("QtRoller", this);
    rollerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerText);
    rollerLayout->addWidget(_roller);
    rollerLayout->addSpacing(30);

    QtText* rollerPickerText = new QtText("QtRollerPicker", this);
    rollerPickerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerPickerText);

    QTime currentTime = QTime::currentTime();
    QString currentHour = QString("%1").arg(currentTime.hour(), 2, 10, QChar('0'));
    QString currentMinute = QString("%1").arg(currentTime.minute(), 2, 10, QChar('0'));
    _timeRollerPicker = new QtRollerPicker(this);
    QStringList hourItemList;
    for(int i = 0; i < 24; i++)
    {
        hourItemList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }
    QStringList minuteList;
    for(int i = 0; i < 61; i++)
    {
        minuteList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }
    _timeRollerPicker->addRoller(hourItemList);
    _timeRollerPicker->addRoller(minuteList);
    _timeRollerPicker->addRoller({ "AM", "PM" }, false);
    _timeRollerPicker->setCurrentData(
        { currentHour, currentMinute, currentTime.hour() >= 12 ? "PM" : "AM" });
    _clockRollerPicker = new QtRollerPicker(this);
    _clockRollerPicker->addRoller(hourItemList);
    _clockRollerPicker->addRoller(minuteList);
    _clockRollerPicker->setRollerWidth(0, 135);
    _clockRollerPicker->setRollerWidth(1, 135);
    _clockRollerPicker->setCurrentData({ currentHour, currentMinute });

    QVBoxLayout* rollerPickerLayout = new QVBoxLayout();
    rollerPickerLayout->addWidget(_timeRollerPicker);
    rollerPickerLayout->addWidget(_clockRollerPicker);
    rollerLayout->addLayout(rollerPickerLayout);
    rollerLayout->addStretch();

    _drawer = new QtDrawerArea(this);
    QWidget* drawerHeader = new QWidget(this);
    QHBoxLayout* drawerHeaderLayout = new QHBoxLayout(drawerHeader);
    QtText* drawerIcon = new QtText(this);
    drawerIcon->setTextPixelSize(15);
    drawerIcon->setQtIcon(QtIconType::MessageArrowDown);
    drawerIcon->setFixedSize(25, 25);
    QtText* drawerText = new QtText("QtDrawer", this);
    drawerText->setTextPixelSize(15);

    QtToggleSwitch* drawerSwitch = new QtToggleSwitch(this);
    QtText* drawerSwitchText = new QtText("关", this);
    drawerSwitchText->setTextPixelSize(15);
    connect(drawerSwitch, &QtToggleSwitch::toggled, this, [this, drawerSwitchText](bool toggled) {
        if(toggled)
        {
            drawerSwitchText->setText("开");
            _drawer->expand();
        }
        else
        {
            drawerSwitchText->setText("关");
            _drawer->collapse();
        }
    });
    connect(_drawer, &QtDrawerArea::expandStateChanged, this,
        [this, drawerSwitch](bool isExpand) { drawerSwitch->setIsToggled(isExpand); });

    drawerHeaderLayout->addWidget(drawerIcon);
    drawerHeaderLayout->addWidget(drawerText);
    drawerHeaderLayout->addStretch();
    drawerHeaderLayout->addWidget(drawerSwitchText);
    drawerHeaderLayout->addWidget(drawerSwitch);

    _drawer->setDrawerHeader(drawerHeader);
    QWidget* drawerWidget1 = new QWidget(this);
    drawerWidget1->setFixedHeight(75);
    QHBoxLayout* drawerWidget1Layout = new QHBoxLayout(drawerWidget1);
    QtCheckBox* drawerCheckBox1 = new QtCheckBox("测试窗口1", this);
    drawerWidget1Layout->addSpacing(60);
    drawerWidget1Layout->addWidget(drawerCheckBox1);

    QWidget* drawerWidget2 = new QWidget(this);
    drawerWidget2->setFixedHeight(75);
    QHBoxLayout* drawerWidget2Layout = new QHBoxLayout(drawerWidget2);
    QtCheckBox* drawerCheckBox2 = new QtCheckBox("测试窗口2", this);
    drawerWidget2Layout->addSpacing(60);
    drawerWidget2Layout->addWidget(drawerCheckBox2);

    QWidget* drawerWidget3 = new QWidget(this);
    drawerWidget3->setFixedHeight(75);
    QHBoxLayout* drawerWidget3Layout = new QHBoxLayout(drawerWidget3);
    QtCheckBox* drawerCheckBox3 = new QtCheckBox("测试窗口3", this);
    drawerWidget3Layout->addSpacing(60);
    drawerWidget3Layout->addWidget(drawerCheckBox3);

    _drawer->addDrawer(drawerWidget1);
    _drawer->addDrawer(drawerWidget2);
    _drawer->addDrawer(drawerWidget3);

    _messageDialog = new QtMessageDialog(this);
    _messageDialog->setTitle("标题");
    _messageDialog->setContent("左眼用来忘记你、右眼用来记忆你。");
    _messageDialog->setFixedSize(280, 150);
    _messageDialog->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    _messageDialog->setAttribute(Qt::WA_TranslucentBackground);

    connect(_messageDialog, &QtMessageDialog::confirmed, this,
        [this]() { qDebug() << "确认按钮被点击"; });
    connect(_messageDialog, &QtMessageDialog::cancelled, this,
        [this]() { qDebug() << "取消按钮被点击"; });

    QtScrollPageArea* messageDialogArea = new QtScrollPageArea(this);
    QHBoxLayout* messageDialogLayout = new QHBoxLayout(messageDialogArea);
    QtText* messageDialogText = new QtText("QtMessageDialog", this);
    messageDialogText->setTextPixelSize(15);
    messageDialogLayout->addWidget(messageDialogText);

    QtPushButton* showMessageDialogButton = new QtPushButton("显示对话框", this);
    showMessageDialogButton->setFixedSize(120, 38);
    connect(showMessageDialogButton, &QtPushButton::clicked, this, [this, showMessageDialogButton]() {
        QPoint globalPos =
            showMessageDialogButton->mapToGlobal(QPoint(0, showMessageDialogButton->height() +
            5));
        _messageDialog->move(globalPos);
        _messageDialog->show();
    });
    messageDialogLayout->addWidget(showMessageDialogButton);
    messageDialogLayout->addStretch();

    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addWidget(colorDialogArea);
    centerVLayout->addWidget(inputDialogArea);
    centerVLayout->addWidget(calendarPickerArea);
    centerVLayout->addWidget(_calendar);
    centerVLayout->addWidget(keyBinderArea);
    centerVLayout->addWidget(_drawer);
    centerVLayout->addWidget(messageDialogArea);
    centerVLayout->addWidget(rollerArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup()
{
}
