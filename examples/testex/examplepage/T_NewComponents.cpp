#include "T_NewComponents.h"
#include "widgets/qtcheckbox.h"
#include "widgets/qtlineedit.h"
#include "widgets/qtpushbutton.h"
#include "widgets/qtradiobutton.h"
#include "widgets/qtratingcontrol.h"
#include "widgets/qtscrollpagearea.h"
#include "widgets/qtskeleton.h"
#include "widgets/qtslider.h"
#include "widgets/qttext.h"
#include "widgets/qttoggleswitch.h"
#include "widgets/qtdivider.h"
#include <qboxlayout.h>
#include <qbuttongroup.h>
#include <qheaderview.h>
#include <qoverload.h>
#include <qstackedwidget.h>
T_NewComponents::T_NewComponents(QWidget* parent)
    : T_BasePage{ parent }
{
    setWindowTitle("ElaNewComponents");
    createCustomWidget("新增组件的演示页面");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("QtNewComponents");

    // ========== QtInfoBadge 示例 ==========
    QtPushButton* dotTargetButton = new QtPushButton("邮件", this);
    dotTargetButton->setFixedSize(80, 36);
    _dotBadge = new QtInfoBadge(this);
    _dotBadge->setBadgeMode(QtInfoBadge::Dot);
    _dotBadge->setSeverity(QtInfoBadge::Attention);
    _dotBadge->attachTo(dotTargetButton);

    QtPushButton* valueTargetButton = new QtPushButton("通知", this);
    valueTargetButton->setFixedSize(80, 36);
    _valueBadge = new QtInfoBadge(5, this);
    _valueBadge->setSeverity(QtInfoBadge::Informational);
    _valueBadge->attachTo(valueTargetButton);

    QtPushButton* iconTargetButton = new QtPushButton("设置", this);
    iconTargetButton->setFixedSize(80, 36);
    _iconBadge = new QtInfoBadge(QtIconType::CircleExclamation, this);
    _iconBadge->setSeverity(QtInfoBadge::Caution);
    _iconBadge->attachTo(iconTargetButton);

    QtScrollPageArea* badgeArea = new QtScrollPageArea(this);
    QHBoxLayout* badgeLayout = new QHBoxLayout(badgeArea);
    QtText* badgeText = new QtText("QtInfoBadge", this);
    badgeText->setTextPixelSize(15);
    badgeLayout->addWidget(badgeText);
    badgeLayout->addSpacing(20);
    badgeLayout->addWidget(dotTargetButton);
    badgeLayout->addSpacing(20);
    badgeLayout->addWidget(valueTargetButton);
    badgeLayout->addSpacing(20);
    badgeLayout->addWidget(iconTargetButton);
    badgeLayout->addStretch();

    // InfoBadge 值控制
    QtScrollPageArea* badgeControlArea = new QtScrollPageArea(this);
    QHBoxLayout* badgeControlLayout = new QHBoxLayout(badgeControlArea);
    badgeControlLayout->addWidget(new QtText("InfoBadge 值控制", 15, this));
    QtSlider* badgeSlider = new QtSlider(this);
    badgeSlider->setRange(0, 150);
    badgeSlider->setValue(5);
    badgeSlider->setFixedWidth(200);
    QtText* badgeValueText = new QtText("5", this);
    badgeValueText->setTextPixelSize(13);
    connect(badgeSlider, &QtSlider::valueChanged, this, [this, badgeValueText](int v) {
        _valueBadge->setValue(v);
        badgeValueText->setText(QString::number(v));
    });
    badgeControlLayout->addWidget(badgeSlider);
    badgeControlLayout->addWidget(badgeValueText);
    badgeControlLayout->addStretch();

    // InfoBadge 严重等级
    QtScrollPageArea* severityArea = new QtScrollPageArea(this);
    QHBoxLayout* severityLayout = new QHBoxLayout(severityArea);
    severityLayout->addWidget(new QtText("InfoBadge 严重等级", 15, this));
    QButtonGroup* severityGroup = new QButtonGroup(this);
    QtRadioButton* attentionRadio = new QtRadioButton("Attention", this);
    QtRadioButton* infoRadio = new QtRadioButton("Informational", this);
    QtRadioButton* successRadio = new QtRadioButton("Success", this);
    QtRadioButton* cautionRadio = new QtRadioButton("Caution", this);
    QtRadioButton* criticalRadio = new QtRadioButton("Critical", this);
    severityGroup->addButton(attentionRadio, 0);
    severityGroup->addButton(infoRadio, 1);
    severityGroup->addButton(successRadio, 2);
    severityGroup->addButton(cautionRadio, 3);
    severityGroup->addButton(criticalRadio, 4);
    infoRadio->setChecked(true);
    connect(severityGroup, QOverload<int>::of(&QButtonGroup::idClicked), this,
        [this](int id) { _valueBadge->setSeverity(static_cast<QtInfoBadge::Severity>(id)); });
    severityLayout->addWidget(attentionRadio);
    severityLayout->addWidget(infoRadio);
    severityLayout->addWidget(successRadio);
    severityLayout->addWidget(cautionRadio);
    severityLayout->addWidget(criticalRadio);
    severityLayout->addStretch();

    // ========== QtExpander 示例 ==========
    QtText* expanderText = new QtText("QtExpander (向下展开)", this);
    expanderText->setTextPixelSize(15);
    _expander = new QtExpander("网络设置", this);
    _expander->setSubTitle("配置代理和网络参数");
    _expander->setHeaderIcon(QtIconType::WifiExclamation);
    QWidget* expanderContent = new QWidget(this);
    QVBoxLayout* expanderContentLayout = new QVBoxLayout(expanderContent);
    expanderContentLayout->setContentsMargins(0, 0, 0, 0);
    expanderContentLayout->addWidget(new QtCheckBox("启用代理", this));
    QtLineEdit* proxyHost = new QtLineEdit(this);
    proxyHost->setPlaceholderText("代理地址 (例如: 127.0.0.1)");
    proxyHost->setFixedHeight(35);
    QtLineEdit* proxyPort = new QtLineEdit(this);
    proxyPort->setPlaceholderText("端口 (例如: 7890)");
    proxyPort->setFixedHeight(35);
    expanderContentLayout->addWidget(proxyHost);
    expanderContentLayout->addWidget(proxyPort);
    _expander->setContentWidget(expanderContent);

    QtText* expanderUpText = new QtText("QtExpander (向上展开)", this);
    expanderUpText->setTextPixelSize(15);
    _expanderUp = new QtExpander("高级设置", this);
    _expanderUp->setExpandDirection(QtExpander::Up);
    _expanderUp->setHeaderIcon(QtIconType::GearComplex);
    QWidget* expanderUpContent = new QWidget(this);
    QVBoxLayout* expanderUpContentLayout = new QVBoxLayout(expanderUpContent);
    expanderUpContentLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* dbgLay = new QHBoxLayout();
    dbgLay->addWidget(new QtText("调试模式", 13, this));
    dbgLay->addWidget(new QtToggleSwitch(this));
    dbgLay->addStretch();
    QHBoxLayout* logLay = new QHBoxLayout();
    logLay->addWidget(new QtText("日志输出", 13, this));
    logLay->addWidget(new QtToggleSwitch(this));
    logLay->addStretch();
    expanderUpContentLayout->addLayout(dbgLay);
    expanderUpContentLayout->addLayout(logLay);
    _expanderUp->setContentWidget(expanderUpContent);

    // ========== QtTeachingTip 示例 ==========
    _teachingTip = new QtTeachingTip(this);
    _teachingTip->setTitle("截图功能升级");
    _teachingTip->setSubTitle("版本 2.0");
    _teachingTip->setContent("现在支持区域截图、延时截图和 OCR 文字识别。");
    _teachingTip->setTipIcon(QtIconType::CameraRetro);
    _teachingTip->setTailPosition(QtTeachingTip::Bottom);
    _teachingTip->addAction("跳过", [=]() { _teachingTip->closeTip(); });
    _teachingTip->addAction("了解更多", [=]() { _teachingTip->closeTip(); });

    _teachingTipTarget = new QtPushButton("显示 TeachingTip", this);
    _teachingTipTarget->setFixedSize(160, 38);
    _teachingTip->setTarget(_teachingTipTarget);
    connect(_teachingTipTarget, &QtPushButton::clicked, this, [=]() { _teachingTip->showTip(); });

    QtScrollPageArea* teachingTipArea = new QtScrollPageArea(this);
    QHBoxLayout* tipLayout = new QHBoxLayout(teachingTipArea);
    tipLayout->addWidget(new QtText("QtTeachingTip", 15, this));
    tipLayout->addWidget(_teachingTipTarget);
    tipLayout->addStretch();

    // TeachingTip 方向切换
    QtScrollPageArea* tipPosArea = new QtScrollPageArea(this);
    QHBoxLayout* tipPosLayout = new QHBoxLayout(tipPosArea);
    tipPosLayout->addWidget(new QtText("TeachingTip 箭头方向", 15, this));
    QButtonGroup* tipPosGroup = new QButtonGroup(this);
    QtRadioButton* autoRadio = new QtRadioButton("Auto", this);
    QtRadioButton* topRadio = new QtRadioButton("Top", this);
    QtRadioButton* bottomRadio = new QtRadioButton("Bottom", this);
    QtRadioButton* leftRadio = new QtRadioButton("Left", this);
    QtRadioButton* rightRadio = new QtRadioButton("Right", this);
    tipPosGroup->addButton(autoRadio, 0);
    tipPosGroup->addButton(topRadio, 1);
    tipPosGroup->addButton(bottomRadio, 2);
    tipPosGroup->addButton(leftRadio, 3);
    tipPosGroup->addButton(rightRadio, 4);
    bottomRadio->setChecked(true);
    connect(tipPosGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, [this](int id) {
        _teachingTip->setTailPosition(static_cast<QtTeachingTip::TailPosition>(id));
    });
    tipPosLayout->addWidget(autoRadio);
    tipPosLayout->addWidget(topRadio);
    tipPosLayout->addWidget(bottomRadio);
    tipPosLayout->addWidget(leftRadio);
    tipPosLayout->addWidget(rightRadio);
    tipPosLayout->addStretch();

    // ========== ElaPasswordBox 示例 ==========
    _passwordBox = new QtPasswordBox(this);
    _passwordBox->setFixedSize(280, 38);
    _passwordBox->setPlaceholderText("请输入密码...");

    QtScrollPageArea* passwordArea = new QtScrollPageArea(this);
    QHBoxLayout* pwdLayout = new QHBoxLayout(passwordArea);
    pwdLayout->addWidget(new QtText("ElaPasswordBox", 15, this));
    pwdLayout->addWidget(_passwordBox);
    pwdLayout->addStretch();

    // ========== QtNumberBox 示例 ==========
    _numberBox = new QtNumberBox(this);
    _numberBox->setMinimum(-50);
    _numberBox->setMaximum(200);
    _numberBox->setStep(5);
    _numberBox->setValue(25);

    QtNumberBox *decimalBox = new QtNumberBox(this);
    decimalBox->setMinimum(0);
    decimalBox->setMaximum(10);
    decimalBox->setStep(0.1);
    decimalBox->setDecimals(2);
    decimalBox->setValue(3.14);

    QtText *numberValue = new QtText("25", this);
    numberValue->setTextPixelSize(13);
    connect(_numberBox, qOverload<double>(&QtNumberBox::valueChanged), this, [this, numberValue](double v)
    {
    	numberValue->setText(QString::number(v, 'f', 0));
    });

    QtScrollPageArea *numberArea = new QtScrollPageArea(this);
    QHBoxLayout *numberLayout = new QHBoxLayout(numberArea);
    numberLayout->addWidget(new QtText("QtNumberBox", 15, this));
    numberLayout->addWidget(_numberBox);
    numberLayout->addWidget(numberValue);
    numberLayout->addSpacing(15);
    numberLayout->addWidget(new QtText("小数:", 13, this));
    numberLayout->addWidget(decimalBox);
    numberLayout->addStretch();

    // ========== QtTag 示例 ==========
    QtTag* defaultTag = new QtTag(this);
    defaultTag->setTagText("Default");
    QtTag* primaryTag = new QtTag(this);
    primaryTag->setTagText("Primary");
    primaryTag->setTagColor(QtTag::Primary);
    QtTag* successTag = new QtTag(this);
    successTag->setTagText("Success");
    successTag->setTagColor(QtTag::Success);
    QtTag* warningTag = new QtTag(this);
    warningTag->setTagText("Warning");
    warningTag->setTagColor(QtTag::Warning);
    QtTag* dangerTag = new QtTag(this);
    dangerTag->setTagText("Danger");
    dangerTag->setTagColor(QtTag::Danger);

    QtScrollPageArea* tagArea = new QtScrollPageArea(this);
    QHBoxLayout* tagLayout = new QHBoxLayout(tagArea);
    tagLayout->addWidget(new QtText("QtTag", 15, this));
    tagLayout->addSpacing(10);
    tagLayout->addWidget(defaultTag);
    tagLayout->addWidget(primaryTag);
    tagLayout->addWidget(successTag);
    tagLayout->addWidget(warningTag);
    tagLayout->addWidget(dangerTag);
    tagLayout->addStretch();

    // Tag 可关闭
    _closableTag = new QtTag(this);
    _closableTag->setTagText("可关闭");
    _closableTag->setIsClosable(true);
    _closableTag->setTagColor(QtTag::Primary);
    QtTag* closableTag2 = new QtTag(this);
    closableTag2->setTagText("北京");
    closableTag2->setIsClosable(true);
    QtTag* closableTag3 = new QtTag(this);
    closableTag3->setTagText("上海");
    closableTag3->setIsClosable(true);
    connect(_closableTag, &QtTag::closed, this, [=]() { _closableTag->hide(); });
    connect(closableTag2, &QtTag::closed, this, [=]() { closableTag2->hide(); });
    connect(closableTag3, &QtTag::closed, this, [=]() { closableTag3->hide(); });

    QtScrollPageArea* tagCloseArea = new QtScrollPageArea(this);
    QHBoxLayout* tagCloseLayout = new QHBoxLayout(tagCloseArea);
    tagCloseLayout->addWidget(new QtText("QtTag 可关闭", 15, this));
    tagCloseLayout->addSpacing(10);
    tagCloseLayout->addWidget(_closableTag);
    tagCloseLayout->addWidget(closableTag2);
    tagCloseLayout->addWidget(closableTag3);
    tagCloseLayout->addStretch();

    // Tag 可选中
    _checkableTag = new QtTag(this);
    _checkableTag->setTagText("周一");
    _checkableTag->setIsCheckable(true);
    QtTag* chkTag2 = new QtTag(this);
    chkTag2->setTagText("周二");
    chkTag2->setIsCheckable(true);
    chkTag2->setIsChecked(true);
    QtTag* chkTag3 = new QtTag(this);
    chkTag3->setTagText("周三");
    chkTag3->setIsCheckable(true);

    QtScrollPageArea* tagCheckArea = new QtScrollPageArea(this);
    QHBoxLayout* tagCheckLayout = new QHBoxLayout(tagCheckArea);
    tagCheckLayout->addWidget(new QtText("QtTag 可选中", 15, this));
    tagCheckLayout->addSpacing(10);
    tagCheckLayout->addWidget(_checkableTag);
    tagCheckLayout->addWidget(chkTag2);
    tagCheckLayout->addWidget(chkTag3);
    tagCheckLayout->addStretch();

    // ========== QtSplitButton 示例 ==========
    _splitButton = new QtSplitButton(this);
    _splitButton->setText("发送邮件");
    _splitButton->setQtIcon(QtIconType::Envelope);
    QtMenu* splitMenu = new QtMenu(this);
    splitMenu->addQtIconAction(QtIconType::Reply, "回复");
    splitMenu->addQtIconAction(QtIconType::ReplyAll, "全部回复");
    splitMenu->addQtIconAction(QtIconType::Share, "转发");
    _splitButton->setMenu(splitMenu);

    QtScrollPageArea* splitArea = new QtScrollPageArea(this);
    QHBoxLayout* splitLayout = new QHBoxLayout(splitArea);
    splitLayout->addWidget(new QtText("QtSplitButton", 15, this));
    splitLayout->addWidget(_splitButton);
    splitLayout->addStretch();

    // ========== QtPersonPicture 示例 ==========
    _personPicture = new QtPersonPicture(this);
    _personPicture->setPicture(QPixmap(":/include/CollapsingWorld.png"));
    _personPictureInitials = new QtPersonPicture(this);
    _personPictureInitials->setDisplayName("Ela");
    _personPictureDefault = new QtPersonPicture(this);

    QtScrollPageArea* personArea = new QtScrollPageArea(this);
    QHBoxLayout* personLayout = new QHBoxLayout(personArea);
    personLayout->addWidget(new QtText("QtPersonPicture", 15, this));
    personLayout->addSpacing(10);
    personLayout->addWidget(_personPicture);
    personLayout->addSpacing(10);
    personLayout->addWidget(_personPictureInitials);
    personLayout->addSpacing(10);
    personLayout->addWidget(_personPictureDefault);
    personLayout->addStretch();

    // ========== QtRatingControl 示例 ==========
    _ratingControl = new QtRatingControl(this);
    _ratingControl->setRating(5);
    QtText* ratingValue = new QtText("5", this);
    ratingValue->setTextPixelSize(15);
    connect(_ratingControl, qOverload<int>(&QtRatingControl::ratingChanged), this,
        [this, ratingValue](int r) { ratingValue->setText(QString::number(r)); });

    QtScrollPageArea* ratingArea = new QtScrollPageArea(this);
    QHBoxLayout* ratingLayout = new QHBoxLayout(ratingArea);
    ratingLayout->addWidget(new QtText("QtRatingControl", 15, this));
    ratingLayout->addWidget(_ratingControl);
    ratingLayout->addWidget(ratingValue);
    ratingLayout->addStretch();

    // ========== QtDivider 示例 ==========
    QtDivider *divider1 = new QtDivider(this);
    QtDivider *divider2 = new QtDivider("分隔文字", this);

    // ========== QtFlyout 示例 ==========
    _flyout = new QtFlyout(this);
    _flyout->setTitle("提示");
    _flyout->setContent("轻量级弹出面板，点击外部自动关闭。");
    QtPushButton* flyoutBtn = new QtPushButton("显示 Flyout", this);
    flyoutBtn->setFixedSize(120, 38);
    connect(flyoutBtn, &QtPushButton::clicked, this, [=]() { _flyout->showFlyout(flyoutBtn); });

    // 自定义 Widget Flyout
    QtFlyout* customFlyout = new QtFlyout(this);
    QWidget* customContent = new QWidget();
    QVBoxLayout* customFlyoutLay = new QVBoxLayout(customContent);
    customFlyoutLay->setContentsMargins(0, 0, 0, 0);
    customFlyoutLay->addWidget(new QtText("请为本次体验评分:", 13, customContent));
    customFlyoutLay->addWidget(new QtRatingControl(customContent));
    customFlyout->setContentWidget(customContent);

    QtPushButton* customFlyoutBtn = new QtPushButton("自定义 Flyout", this);
    customFlyoutBtn->setFixedSize(130, 38);
    connect(customFlyoutBtn, &QtPushButton::clicked, this,
        [=]() { customFlyout->showFlyout(customFlyoutBtn); });

    QtScrollPageArea* flyoutArea = new QtScrollPageArea(this);
    QHBoxLayout* flyoutLayout = new QHBoxLayout(flyoutArea);
    flyoutLayout->addWidget(new QtText("QtFlyout", 15, this));
    flyoutLayout->addWidget(flyoutBtn);
    flyoutLayout->addSpacing(10);
    flyoutLayout->addWidget(customFlyoutBtn);
    flyoutLayout->addStretch();

    // ========== QtSkeleton 示例 ==========
    QtSkeleton *skelCircle = new QtSkeleton(this);
    skelCircle->setSkeletonType(QtSkeleton::Circle);
    skelCircle->setFixedSize(40, 40);
    QtSkeleton *skelText1 = new QtSkeleton(this);
    skelText1->setSkeletonType(QtSkeleton::Text);
    skelText1->setFixedSize(250, 16);
    QtSkeleton *skelRect = new QtSkeleton(this);
    skelRect->setSkeletonType(QtSkeleton::Rectangle);
    skelRect->setFixedSize(200, 80);
    skelRect->setBorderRadius(8);

    QtScrollPageArea *skelArea = new QtScrollPageArea(this);
    QHBoxLayout *skelLayout = new QHBoxLayout(skelArea);
    skelLayout->addWidget(new QtText("QtSkeleton", 15, this));
    skelLayout->addSpacing(10);
    skelLayout->addWidget(skelCircle);
    skelLayout->addSpacing(10);
    skelLayout->addWidget(skelText1);
    skelLayout->addSpacing(10);
    skelLayout->addWidget(skelRect);
    skelLayout->addStretch();

    // ========== QtSteps 示例 ==========
    _steps = new QtSteps(this);
    _steps->setStepCount(4);
    _steps->setStepTitles({"选择商品", "确认订单", "支付", "完成"});
    _steps->setCurrentStep(1);
    QtPushButton *prevBtn = new QtPushButton("上一步", this);
    prevBtn->setFixedSize(80, 32);
    QtPushButton *nextBtn = new QtPushButton("下一步", this);
    nextBtn->setFixedSize(80, 32);
    connect(prevBtn, &QtPushButton::clicked, _steps, &QtSteps::previous);
    connect(nextBtn, &QtPushButton::clicked, _steps, &QtSteps::next);

    QHBoxLayout *stepsCtrl = new QHBoxLayout();
    stepsCtrl->addWidget(new QtText("QtSteps", 15, this));
    stepsCtrl->addStretch();
    stepsCtrl->addWidget(prevBtn);
    stepsCtrl->addWidget(nextBtn);

    // ========== QtCommandBar 示例 ==========
    _commandBar = new QtCommandBar(this);
    _commandBar->addItem({QtIconType::Copy, "复制"});
    _commandBar->addItem({QtIconType::Paste, "粘贴"});
    _commandBar->addItem({QtIconType::Scissors, "剪切"});
    _commandBar->addSeparator();
    _commandBar->addItem({QtIconType::ArrowRotateLeft, "撤销"});
    _commandBar->addItem({QtIconType::ArrowRotateRight, "重做"});
    _commandBar->addSeparator();
    _commandBar->addItem({QtIconType::FloppyDisk, "保存"});

    QtScrollPageArea *cmdArea = new QtScrollPageArea(this);
    QHBoxLayout *cmdLayout = new QHBoxLayout(cmdArea);
    cmdLayout->addWidget(new QtText("QtCommandBar", 15, this));
    cmdLayout->addWidget(_commandBar);
    cmdLayout->addStretch();

    // ========== QtPagination 示例 ==========
    QStackedWidget* pageStack = new QStackedWidget(this);
    pageStack->setFixedHeight(160);
    int totalPages = 50;
    for(int p = 1; p <= totalPages; p++)
    {
        QWidget* page = new QWidget(this);
        QVBoxLayout* pLay = new QVBoxLayout(page);
        pLay->setContentsMargins(15, 10, 15, 10);
        pLay->addWidget(new QtText(QString("第 %1 页").arg(p), 16, this));
        for(int i = 1; i <= 5; i++)
            pLay->addWidget(new QtText(QString("  数据项 #%1").arg((p - 1) * 4 + i), 13, this));
        pLay->addStretch();
        pageStack->addWidget(page);
    }
    _pagination = new QtPagination(this);
    _pagination->setTotalPages(totalPages);
    _pagination->setCurrentPage(1);
    _pagination->setJumperVisible(true);
    QtText* pageInfo = new QtText(QString("第 1 / %1 页").arg(totalPages), this);
    pageInfo->setTextPixelSize(13);
    connect(_pagination, qOverload<int>(&QtPagination::currentPageChanged), this,
        [this, pageStack, pageInfo, totalPages](int pg) {
            pageStack->setCurrentIndex(pg - 1);
            pageInfo->setText(QString("第 %1 / %2 页").arg(pg).arg(totalPages));
        });

    QtScrollPageArea* pgArea = new QtScrollPageArea(this);
    QHBoxLayout* pgLayout = new QHBoxLayout(pgArea);
    pgLayout->addWidget(new QtText("QtPagination", 15, this));
    pgLayout->addWidget(_pagination);
    pgLayout->addWidget(pageInfo);
    pgLayout->addStretch();

    // ========== QtTimeline 示例 ==========
    _timeline = new QtTimeline(this);
    _timeline->addItem({"项目启动", "完成需求分析和技术选型", "2026-01-15",
    QtIconType::RocketLaunch}); _timeline->addItem({"开发阶段", "完成核心模块开发", "2026-02-20",
    QtIconType::Code}); _timeline->addItem({"测试阶段", "集成测试和验收测试", "2026-03-01"});
    _timeline->addItem({"发布上线", "部署到生产环境", "2026-03-15"});

    // // ========== ElaToast 示例 ==========
    // QtPushButton *toastSuccessBtn = new QtPushButton("成功", this);
    // toastSuccessBtn->setFixedSize(70, 32);
    // connect(toastSuccessBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaToast::success("操作成功完成!", 2000, window());
    // });
    // QtPushButton *toastInfoBtn = new QtPushButton("信息", this);
    // toastInfoBtn->setFixedSize(70, 32);
    // connect(toastInfoBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaToast::info("这是一条提示信息", 2000, window());
    // });
    // QtPushButton *toastWarnBtn = new QtPushButton("警告", this);
    // toastWarnBtn->setFixedSize(70, 32);
    // connect(toastWarnBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaToast::warning("请注意检查输入", 2000, window());
    // });
    // QtPushButton *toastErrorBtn = new QtPushButton("错误", this);
    // toastErrorBtn->setFixedSize(70, 32);
    // connect(toastErrorBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaToast::error("操作失败，请重试", 2000, window());
    // });

    // QtScrollPageArea *toastArea = new QtScrollPageArea(this);
    // QHBoxLayout *toastLayout = new QHBoxLayout(toastArea);
    // toastLayout->addWidget(new QtText("ElaToast", 15, this));
    // toastLayout->addWidget(toastSuccessBtn);
    // toastLayout->addWidget(toastInfoBtn);
    // toastLayout->addWidget(toastWarnBtn);
    // toastLayout->addWidget(toastErrorBtn);
    // toastLayout->addStretch();

    // ========== ElaWizard 示例 ==========
    // _wizard = new ElaWizard(this);
    // QWidget *wizPage1 = new QWidget();
    // QVBoxLayout *wl1 = new QVBoxLayout(wizPage1);
    // wl1->addWidget(new QtText("欢迎使用安装向导", 16, wizPage1));
    // wl1->addWidget(new QtText("本向导将引导你完成软件安装过程。", 13, wizPage1));
    // wl1->addStretch();
    // _wizard->addStep("欢迎", wizPage1);

    // QWidget *wizPage2 = new QWidget();
    // QVBoxLayout *wl2 = new QVBoxLayout(wizPage2);
    // wl2->addWidget(new QtText("请选择安装路径", 16, wizPage2));
    // QtLineEdit *pathEdit = new QtLineEdit(wizPage2);
    // pathEdit->setText("/usr/local/ElaWidgetTools");
    // pathEdit->setFixedHeight(35);
    // wl2->addWidget(pathEdit);
    // wl2->addStretch();
    // _wizard->addStep("路径", wizPage2);

    // QWidget *wizPage3 = new QWidget();
    // QVBoxLayout *wl3 = new QVBoxLayout(wizPage3);
    // wl3->addWidget(new QtText("安装完成！", 16, wizPage3));
    // wl3->addWidget(new QtText("所有组件已成功安装。", 13, wizPage3));
    // wl3->addStretch();
    // _wizard->addStep("完成", wizPage3);

    // QtPushButton *wizardBtn = new QtPushButton("打开向导", this);
    // wizardBtn->setFixedSize(100, 38);
    // connect(wizardBtn, &QtPushButton::clicked, this, [=]() { _wizard->show(); });

    // QtScrollPageArea *wizardArea = new QtScrollPageArea(this);
    // QHBoxLayout *wizardLayout = new QHBoxLayout(wizardArea);
    // wizardLayout->addWidget(new QtText("ElaWizard", 15, this));
    // wizardLayout->addWidget(wizardBtn);
    // wizardLayout->addStretch();

    // ========== ElaNotificationCenter 示例 ==========
    // _notificationCenter = new ElaNotificationCenter(this);
    // _notificationCenter->addNotification({"系统更新", "ElaWidgetTools 已更新到 v2.1", "10:30"});
    // _notificationCenter->addNotification({"构建完成", "项目编译成功，耗时 12 秒", "10:25",
    // QtIconType::Check}); _notificationCenter->addNotification({"新消息", "收到来自 Qt
    // 社区的新回复", "09:45", QtIconType::Envelope});

    // QtPushButton *notifBtn = new QtPushButton("通知中心", this);
    // notifBtn->setFixedSize(100, 38);
    // connect(notifBtn, &QtPushButton::pressed, this, [=]()
    // {
    // 	if (_notificationCenter->isPanelVisible())
    // 		_notificationCenter->hidePanel();
    // 	else
    // 		_notificationCenter->showPanel(window());
    // });

    // QtScrollPageArea *notifArea = new QtScrollPageArea(this);
    // QHBoxLayout *notifLayout = new QHBoxLayout(notifArea);
    // notifLayout->addWidget(new QtText("ElaNotificationCenter", 15, this));
    // notifLayout->addWidget(notifBtn);
    // notifLayout->addStretch();

    // ========== ElaMarkdownViewer 示例 ==========
    // _markdownViewer = new ElaMarkdownViewer(this);
    // _markdownViewer->setFixedHeight(200);
    // _markdownViewer->setMarkdown(
    // 	"# ElaWidgetTools\n\n"
    // 	"一个 **FluentUI** 风格的 Qt 组件库。\n\n"
    // 	"## 特性\n\n"
    // 	"- 支持 `Light` / `Dark` 主题切换\n"
    // 	"- **90+** 个组件\n"
    // 	"- 跨平台: Windows / macOS / Linux\n\n"
    // 	"## 代码示例\n\n"
    // 	"```cpp\nElaToast::success(\"Hello World!\");\n```\n\n"
    // 	"> 更多详情请访问 [GitHub](https://github.com/RainbowCandyX/ElaWidgetTools)\n"
    // );

    // ========== ElaCaptcha 示例 ==========
    // _captcha = new ElaCaptcha(this);
    // _captcha->setInputMode(ElaCaptcha::DigitOnly);
    // QtText *captchaResult = new QtText("", this);
    // _captcha->setCodeLength(4);
    // captchaResult->setTextPixelSize(13);
    // connect(_captcha, &ElaCaptcha::codeCompleted, this, [=](const QString &code)
    // {
    // 	captchaResult->setText("输入完成: " + code);
    // });
    // connect(_captcha, &ElaCaptcha::codeChanged, this, [=](const QString &code)
    // {
    // 	if (code.length() < _captcha->getCodeLength())
    // 		captchaResult->setText("");
    // });

    // QtPushButton *captchaClearBtn = new QtPushButton("清除", this);
    // captchaClearBtn->setFixedSize(60, 32);
    // connect(captchaClearBtn, &QtPushButton::clicked, _captcha, &ElaCaptcha::clear);

    // QtScrollPageArea *captchaArea = new QtScrollPageArea(this);
    // QHBoxLayout *captchaLayout = new QHBoxLayout(captchaArea);
    // captchaLayout->addWidget(new QtText("ElaCaptcha", 15, this));
    // captchaLayout->addWidget(_captcha);
    // captchaLayout->addWidget(captchaClearBtn);
    // captchaLayout->addWidget(captchaResult);
    // captchaLayout->addStretch();

    // ========== ElaStatCard 示例 ==========
    // ElaStatCard *statCard1 = new ElaStatCard(this);
    // statCard1->setTitle("今日销售额");
    // statCard1->setValue("¥86,400");
    // statCard1->setCardIcon(QtIconType::CartShopping);
    // statCard1->setTrend(ElaStatCard::Up);
    // statCard1->setTrendText("+15.3%");
    // statCard1->setDescription("较昨日");

    // ElaStatCard *statCard2 = new ElaStatCard(this);
    // statCard2->setTitle("CPU 使用率");
    // statCard2->setValue("62%");
    // statCard2->setCardIcon(QtIconType::Gauge);
    // statCard2->setTrend(ElaStatCard::Down);
    // statCard2->setTrendText("-8.1%");
    // statCard2->setDescription("较上小时");

    // ElaStatCard *statCard3 = new ElaStatCard(this);
    // statCard3->setTitle("在线设备");
    // statCard3->setValue("1,024");
    // statCard3->setCardIcon(QtIconType::WifiExclamation);
    // statCard3->setTrend(ElaStatCard::Neutral);
    // statCard3->setTrendText("稳定");
    // statCard3->setDescription("当前在线");

    // QHBoxLayout *statLayout = new QHBoxLayout();
    // statLayout->addWidget(new QtText("ElaStatCard", 15, this));
    // statLayout->addSpacing(10);
    // statLayout->addWidget(statCard1);
    // statLayout->addWidget(statCard2);
    // statLayout->addWidget(statCard3);
    // statLayout->addStretch();

    // ========== ElaDropDownButton 示例 ==========
    // _dropDownButton = new ElaDropDownButton(this);
    // _dropDownButton->setText("排序方式");
    // _dropDownButton->setElaIcon(QtIconType::BarsSort);
    // QtMenu *dropMenu = new QtMenu(this);
    // dropMenu->addQtIconAction(QtIconType::ArrowDownAZ, "按名称");
    // dropMenu->addQtIconAction(QtIconType::Calendar, "按日期");
    // dropMenu->addQtIconAction(QtIconType::ArrowDown19, "按大小");
    // _dropDownButton->setMenu(dropMenu);

    // ElaDropDownButton *dropBtn2 = new ElaDropDownButton(this);
    // dropBtn2->setText("导出");
    // QtMenu *dropMenu2 = new QtMenu(this);
    // dropMenu2->addAction("PDF");
    // dropMenu2->addAction("Excel");
    // dropMenu2->addAction("CSV");
    // dropBtn2->setMenu(dropMenu2);

    // QtScrollPageArea *dropArea = new QtScrollPageArea(this);
    // QHBoxLayout *dropLayout = new QHBoxLayout(dropArea);
    // dropLayout->addWidget(new QtText("ElaDropDownButton", 15, this));
    // dropLayout->addWidget(_dropDownButton);
    // dropLayout->addSpacing(10);
    // dropLayout->addWidget(dropBtn2);
    // dropLayout->addStretch();

    // ========== ElaSelectorBar 示例 ==========
    // _selectorBar = new ElaSelectorBar(this);
    // _selectorBar->addItem("全部");
    // _selectorBar->addItem("未读");
    // _selectorBar->addItem("已标记");
    // _selectorBar->addItem("归档");

    // ElaSelectorBar *viewSelector = new ElaSelectorBar(this);
    // viewSelector->addItem(QtIconType::List, "列表");
    // viewSelector->addItem(QtIconType::Grid2, "网格");
    // viewSelector->addItem(QtIconType::TableCells, "详情");

    // QtText *selectorInfo = new QtText("选中: 全部", this);
    // selectorInfo->setTextPixelSize(13);
    // connect(_selectorBar, &ElaSelectorBar::currentIndexChanged, this, [=](int idx)
    // {
    // 	QStringList names = {"全部", "未读", "已标记", "归档"};
    // 	selectorInfo->setText("选中: " + names.value(idx));
    // });

    // QtScrollPageArea *selectorArea = new QtScrollPageArea(this);
    // QHBoxLayout *selectorLayout = new QHBoxLayout(selectorArea);
    // selectorLayout->addWidget(new QtText("ElaSelectorBar", 15, this));
    // selectorLayout->addWidget(_selectorBar);
    // selectorLayout->addWidget(selectorInfo);
    // selectorLayout->addSpacing(10);
    // selectorLayout->addWidget(viewSelector);
    // selectorLayout->addStretch();

    // ========== ElaVirtualList 示例 ==========
    // _virtualList = new ElaVirtualList(this);
    // QStandardItemModel *virtualModel = new QStandardItemModel(this);
    // for (int i = 0; i < 10000; ++i)
    // {
    // 	QStandardItem *item = new QStandardItem(QString("虚拟列表项 #%1 - 支持万级数据量").arg(i +
    // 1)); 	virtualModel->appendRow(item);
    // }
    // _virtualList->setModel(virtualModel);

    // QtText *virtualListInfo = new QtText("已加载 10,000 条数据", this);
    // virtualListInfo->setTextPixelSize(13);
    // virtualListInfo->setMinimumWidth(200);

    // QtScrollPageArea *virtualListArea = new QtScrollPageArea(this);
    // virtualListArea->setFixedHeight(200);
    // QVBoxLayout *virtualListLayout = new QVBoxLayout(virtualListArea);
    // QHBoxLayout *virtualListHeader = new QHBoxLayout();
    // virtualListHeader->addWidget(new QtText("ElaVirtualList", 15, this));
    // virtualListHeader->addWidget(virtualListInfo);
    // virtualListHeader->addStretch();
    // virtualListLayout->addLayout(virtualListHeader);
    // virtualListLayout->addWidget(_virtualList);

    // ========== ElaSplashScreen 示例 ==========
    // _splashScreen = new ElaSplashScreen(this);
    // _splashScreen->setTitle("ElaWidgetTools");
    // _splashScreen->setSubTitle("FluentUI 风格 Qt 组件库");
    // _splashScreen->setStatusText("正在初始化...");
    // _splashScreen->setIsShowProgressRing(true);

    // QtPushButton *splashBtn = new QtPushButton("显示启动屏", this);
    // splashBtn->setFixedSize(120, 38);
    // connect(splashBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_splashScreen->setValue(0);
    // 	_splashScreen->setStatusText("正在初始化...");
    // 	_splashScreen->show();
    // 	// 模拟加载过程
    // 	QTimer *timer = new QTimer(this);
    // 	int *progress = new int(0);
    // 	connect(timer, &QTimer::timeout, this, [=]()
    // 	{
    // 		*progress += 5;
    // 		_splashScreen->setValue(*progress);
    // 		if (*progress >= 30)
    // 			_splashScreen->setStatusText("正在加载组件...");
    // 		if (*progress >= 60)
    // 			_splashScreen->setStatusText("正在准备界面...");
    // 		if (*progress >= 100)
    // 		{
    // 			timer->stop();
    // 			timer->deleteLater();
    // 			_splashScreen->setStatusText("加载完成!");
    // 			QTimer::singleShot(500, this, [=]()
    // 			{
    // 				_splashScreen->close();
    // 				delete progress;
    // 			});
    // 		}
    // 	});
    // 	timer->start(80);
    // });

    // QtScrollPageArea *splashArea = new QtScrollPageArea(this);
    // QHBoxLayout *splashLayout = new QHBoxLayout(splashArea);
    // splashLayout->addWidget(new QtText("ElaSplashScreen", 15, this));
    // splashLayout->addWidget(splashBtn);
    // splashLayout->addStretch();

    // ========== ElaSheetPanel 示例 ==========
    // _sheetPanel = nullptr; // 延迟创建，构造时 window() 可能还未就绪

    // QtPushButton *sheetPeekBtn = new QtPushButton("Peek", this);
    // sheetPeekBtn->setFixedSize(70, 32);
    // connect(sheetPeekBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	if (!_sheetPanel)
    // 	{
    // 		_sheetPanel = new ElaSheetPanel(window());
    // 		_sheetPanel->setDragHandleVisible(true);
    // 		_sheetPanel->setCloseOnOverlayClick(true);
    // 		QWidget *sheetContent = new QWidget();
    // 		QVBoxLayout *sheetContentLayout = new QVBoxLayout(sheetContent);
    // 		sheetContentLayout->setContentsMargins(20, 10, 20, 10);
    // 		sheetContentLayout->addWidget(new QtText("Sheet 面板内容", 16, sheetContent));
    // 		sheetContentLayout->addWidget(new
    // QtText("这是一个从底部滑出的半模态面板，支持拖拽调整高度。", 13, sheetContent));
    // 		sheetContentLayout->addWidget(new QtText("可以放置表单、详情或任意内容。", 13,
    // sheetContent)); 		QtLineEdit *sheetInput = new QtLineEdit(sheetContent);
    // 		sheetInput->setPlaceholderText("在 Sheet 中输入内容...");
    // 		sheetInput->setFixedHeight(35);
    // 		sheetContentLayout->addWidget(sheetInput);
    // 		sheetContentLayout->addStretch();
    // 		_sheetPanel->setCentralWidget(sheetContent);
    // 	}
    // 	_sheetPanel->open(ElaSheetPanelType::Peek);
    // });
    // QtPushButton *sheetHalfBtn = new QtPushButton("Half", this);
    // sheetHalfBtn->setFixedSize(70, 32);
    // connect(sheetHalfBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	if (!_sheetPanel)
    // 	{
    // 		sheetPeekBtn->click();
    // 		return;
    // 	}
    // 	_sheetPanel->open(ElaSheetPanelType::Half);
    // });
    // QtPushButton *sheetFullBtn = new QtPushButton("Full", this);
    // sheetFullBtn->setFixedSize(70, 32);
    // connect(sheetFullBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	if (!_sheetPanel)
    // 	{
    // 		sheetPeekBtn->click();
    // 		return;
    // 	}
    // 	_sheetPanel->open(ElaSheetPanelType::Full);
    // });

    // QtScrollPageArea *sheetArea = new QtScrollPageArea(this);
    // QHBoxLayout *sheetLayout = new QHBoxLayout(sheetArea);
    // sheetLayout->addWidget(new QtText("ElaSheetPanel", 15, this));
    // sheetLayout->addWidget(sheetPeekBtn);
    // sheetLayout->addWidget(sheetHalfBtn);
    // sheetLayout->addWidget(sheetFullBtn);
    // sheetLayout->addStretch();

    // ========== ElaTransfer 示例 ==========
    // _transfer = new ElaTransfer(this);
    // _transfer->setFixedHeight(280);
    // _transfer->setSourceTitle("可选项目");
    // _transfer->setTargetTitle("已选项目");
    // QStringList transferItems;
    // for (int i = 1; i <= 20; ++i)
    // {
    // 	transferItems.append(QString("项目 %1").arg(i));
    // }
    // _transfer->setSourceItems(transferItems);

    // QtText *transferResult = new QtText("已选: 0 项", this);
    // transferResult->setTextPixelSize(13);
    // connect(_transfer, &ElaTransfer::transferChanged, this, [=](const QStringList &, const
    // QStringList &target)
    // {
    // 	transferResult->setText(QString("已选: %1 项").arg(target.size()));
    // });

    // QHBoxLayout *transferHeader = new QHBoxLayout();
    // transferHeader->addWidget(new QtText("ElaTransfer", 15, this));
    // transferHeader->addWidget(transferResult);
    // transferHeader->addStretch();

    // ========== ElaSpotlight 示例 ==========
    // _spotlight = nullptr; // 延迟创建

    // QtPushButton *spotlightSingleBtn = new QtPushButton("单目标聚光", this);
    // spotlightSingleBtn->setFixedSize(110, 38);
    // connect(spotlightSingleBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	if (!_spotlight)
    // 	{
    // 		_spotlight = new ElaSpotlight(window());
    // 	}
    // 	_spotlight->setTitle("ElaSplashScreen");
    // 	_spotlight->setContent("点击此按钮可以展示启动屏效果。");
    // 	_spotlight->showSpotlight(splashBtn, "我知道了");
    // });

    // QtPushButton *spotlightTourBtn = new QtPushButton("多步骤引导", this);
    // spotlightTourBtn->setFixedSize(110, 38);
    // connect(spotlightTourBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	if (!_spotlight)
    // 	{
    // 		_spotlight = new ElaSpotlight(window());
    // 	}
    // 	QList<ElaSpotlight::SpotlightStep> steps;
    // 	steps.append({splashBtn, "启动屏", "展示应用启动画面效果", false});
    // 	steps.append({sheetPeekBtn, "底部面板", "从底部滑出半模态面板", false});
    // 	steps.append({spotlightSingleBtn, "聚光灯", "高亮任意目标区域", false});
    // 	_spotlight->setSteps(steps);
    // 	_spotlight->start();
    // });

    // QtScrollPageArea *spotlightArea = new QtScrollPageArea(this);
    // QHBoxLayout *spotlightLayout = new QHBoxLayout(spotlightArea);
    // spotlightLayout->addWidget(new QtText("ElaSpotlight", 15, this));
    // spotlightLayout->addWidget(spotlightSingleBtn);
    // spotlightLayout->addWidget(spotlightTourBtn);
    // spotlightLayout->addStretch();

    // ========== ElaCountdown 示例 ==========
    // _countdown = new ElaCountdown(this);
    // _countdown->setRemainingSeconds(86400 + 3661);
    // _countdown->setIsShowDays(true);

    // QtPushButton *countdownStartBtn = new QtPushButton("开始", this);
    // countdownStartBtn->setFixedSize(60, 32);
    // QtPushButton *countdownPauseBtn = new QtPushButton("暂停", this);
    // countdownPauseBtn->setFixedSize(60, 32);
    // QtPushButton *countdownResetBtn = new QtPushButton("重置", this);
    // countdownResetBtn->setFixedSize(60, 32);
    // connect(countdownStartBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	if (_countdown->isRunning())
    // 		_countdown->resume();
    // 	else
    // 		_countdown->start();
    // });
    // connect(countdownPauseBtn, &QtPushButton::clicked, _countdown, &ElaCountdown::pause);
    // connect(countdownResetBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_countdown->stop();
    // 	_countdown->setRemainingSeconds(86400 + 3661);
    // });

    // ElaCountdown *countdownTarget = new ElaCountdown(this);
    // countdownTarget->setIsShowDays(false);
    // countdownTarget->setTargetDateTime(QDateTime::currentDateTime().addSecs(7200));
    // countdownTarget->start();

    // QtScrollPageArea *countdownArea = new QtScrollPageArea(this);
    // countdownArea->setFixedHeight(130);
    // QVBoxLayout *countdownMainLayout = new QVBoxLayout(countdownArea);
    // QHBoxLayout *countdownHeader = new QHBoxLayout();
    // countdownHeader->addWidget(new QtText("ElaCountdown", 15, this));
    // countdownHeader->addWidget(countdownStartBtn);
    // countdownHeader->addWidget(countdownPauseBtn);
    // countdownHeader->addWidget(countdownResetBtn);
    // countdownHeader->addStretch();
    // countdownMainLayout->addLayout(countdownHeader);
    // QHBoxLayout *countdownBody = new QHBoxLayout();
    // countdownBody->addWidget(_countdown);
    // countdownBody->addSpacing(20);
    // countdownBody->addWidget(new QtText("目标时间 (2小时后):", 13, this));
    // countdownBody->addWidget(countdownTarget);
    // countdownBody->addStretch();
    // countdownMainLayout->addLayout(countdownBody);

    // ========== ElaPopconfirm 示例 ==========
    // _popconfirm = new ElaPopconfirm(this);
    // _popconfirm->setTitle("确认删除");
    // _popconfirm->setContent("删除后将无法恢复，是否继续？");

    // QtPushButton *popconfirmBtn = new QtPushButton("删除项目", this);
    // popconfirmBtn->setFixedSize(100, 38);
    // connect(popconfirmBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_popconfirm->showPopconfirm(popconfirmBtn);
    // });

    // QtText *popconfirmResult = new QtText("", this);
    // popconfirmResult->setTextPixelSize(13);
    // connect(_popconfirm, &ElaPopconfirm::confirmed, this, [=]()
    // {
    // 	popconfirmResult->setText("已确认删除");
    // });
    // connect(_popconfirm, &ElaPopconfirm::cancelled, this, [=]()
    // {
    // 	popconfirmResult->setText("已取消");
    // });

    // ElaPopconfirm *customPopconfirm = new ElaPopconfirm(this);
    // customPopconfirm->setTitle("提交变更");
    // customPopconfirm->setContent("确定要提交到远程仓库吗？");
    // customPopconfirm->setConfirmButtonText("提交");
    // customPopconfirm->setCancelButtonText("再想想");
    // customPopconfirm->setIcon(QtIconType::CloudArrowUp);

    // QtPushButton *customPopconfirmBtn = new QtPushButton("提交代码", this);
    // customPopconfirmBtn->setFixedSize(100, 38);
    // connect(customPopconfirmBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	customPopconfirm->showPopconfirm(customPopconfirmBtn);
    // });

    // QtScrollPageArea *popconfirmArea = new QtScrollPageArea(this);
    // QHBoxLayout *popconfirmLayout = new QHBoxLayout(popconfirmArea);
    // popconfirmLayout->addWidget(new QtText("ElaPopconfirm", 15, this));
    // popconfirmLayout->addWidget(popconfirmBtn);
    // popconfirmLayout->addWidget(popconfirmResult);
    // popconfirmLayout->addSpacing(10);
    // popconfirmLayout->addWidget(customPopconfirmBtn);
    // popconfirmLayout->addStretch();

    // ========== ElaWatermark 示例 ==========
    // _watermark = new ElaWatermark("ElaWidgetTools", window());
    // _watermark->setVisible(false);

    // QtPushButton *watermarkShowBtn = new QtPushButton("显示", this);
    // watermarkShowBtn->setFixedSize(60, 36);
    // QtPushButton *watermarkHideBtn = new QtPushButton("隐藏", this);
    // watermarkHideBtn->setFixedSize(60, 36);
    // connect(watermarkShowBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_watermark->setVisible(true);
    // });
    // connect(watermarkHideBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_watermark->setVisible(false);
    // });

    // QtLineEdit *watermarkTextEdit = new QtLineEdit(this);
    // watermarkTextEdit->setText("ElaWidgetTools");
    // watermarkTextEdit->setFixedWidth(140);
    // connect(watermarkTextEdit, &QtLineEdit::textChanged, this, [=](const QString &text)
    // {
    // 	_watermark->setText(text);
    // });

    // QtSlider *watermarkOpacitySlider = new QtSlider(this);
    // watermarkOpacitySlider->setRange(1, 100);
    // watermarkOpacitySlider->setValue(50);
    // watermarkOpacitySlider->setFixedWidth(120);
    // connect(watermarkOpacitySlider, &QtSlider::valueChanged, this, [=](int v)
    // {
    // 	_watermark->setOpacity(v / 100.0);
    // });

    // QtSlider *watermarkRotationSlider = new QtSlider(this);
    // watermarkRotationSlider->setRange(-90, 90);
    // watermarkRotationSlider->setValue(-22);
    // watermarkRotationSlider->setFixedWidth(120);
    // connect(watermarkRotationSlider, &QtSlider::valueChanged, this, [=](int v)
    // {
    // 	_watermark->setRotation(v);
    // });

    // QtScrollPageArea *watermarkArea = new QtScrollPageArea(this);
    // QHBoxLayout *watermarkLayout = new QHBoxLayout(watermarkArea);
    // watermarkLayout->addWidget(new QtText("ElaWatermark", 15, this));
    // watermarkLayout->addSpacing(20);
    // watermarkLayout->addWidget(watermarkShowBtn);
    // watermarkLayout->addWidget(watermarkHideBtn);
    // watermarkLayout->addSpacing(10);
    // watermarkLayout->addWidget(watermarkTextEdit);
    // watermarkLayout->addSpacing(10);
    // watermarkLayout->addWidget(new QtText("透明度", 13, this));
    // watermarkLayout->addWidget(watermarkOpacitySlider);
    // watermarkLayout->addSpacing(10);
    // watermarkLayout->addWidget(new QtText("角度", 13, this));
    // watermarkLayout->addWidget(watermarkRotationSlider);
    // watermarkLayout->addStretch();

    // ========== ElaSnackbar 示例 ==========
    // QtPushButton *snackSuccessBtn = new QtPushButton("成功", this);
    // snackSuccessBtn->setFixedSize(70, 32);
    // connect(snackSuccessBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaSnackbar::success("文件保存成功!", "", 3000, window());
    // });

    // QtPushButton *snackUndoBtn = new QtPushButton("撤销操作", this);
    // snackUndoBtn->setFixedSize(90, 32);
    // connect(snackUndoBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaSnackbar *snackbar = ElaSnackbar::info("已删除 3 个项目", "撤销", 5000, window());
    // 	connect(snackbar, &ElaSnackbar::actionClicked, this, [=]()
    // 	{
    // 		ElaToast::success("已撤销删除操作", 2000, window());
    // 	});
    // });

    // QtPushButton *snackWarnBtn = new QtPushButton("警告", this);
    // snackWarnBtn->setFixedSize(70, 32);
    // connect(snackWarnBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaSnackbar::warning("磁盘空间不足", "查看详情", 4000, window());
    // });

    // QtPushButton *snackErrorBtn = new QtPushButton("错误", this);
    // snackErrorBtn->setFixedSize(70, 32);
    // connect(snackErrorBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	ElaSnackbar::error("网络连接失败", "重试", 4000, window());
    // });

    // QtScrollPageArea *snackbarArea = new QtScrollPageArea(this);
    // QHBoxLayout *snackbarLayout = new QHBoxLayout(snackbarArea);
    // snackbarLayout->addWidget(new QtText("ElaSnackbar", 15, this));
    // snackbarLayout->addWidget(snackSuccessBtn);
    // snackbarLayout->addWidget(snackUndoBtn);
    // snackbarLayout->addWidget(snackWarnBtn);
    // snackbarLayout->addWidget(snackErrorBtn);
    // snackbarLayout->addStretch();

    // ========== ElaSplitter 示例 ==========
    // _splitter = new ElaSplitter(Qt::Horizontal, this);
    // _splitter->setFixedHeight(200);

    // QWidget *splitterLeft = new QWidget(this);
    // splitterLeft->setMinimumWidth(120);
    // QVBoxLayout *leftLayout = new QVBoxLayout(splitterLeft);
    // leftLayout->setContentsMargins(10, 10, 10, 10);
    // QtText *leftTitle = new QtText("左侧面板", this);
    // leftTitle->setTextPixelSize(14);
    // leftLayout->addWidget(leftTitle);
    // for (int i = 1; i <= 5; ++i)
    // {
    // 	leftLayout->addWidget(new QtText(QString("  列表项 #%1").arg(i), 13, this));
    // }
    // leftLayout->addStretch();

    // QWidget *splitterRight = new QWidget(this);
    // splitterRight->setMinimumWidth(120);
    // QVBoxLayout *rightLayout = new QVBoxLayout(splitterRight);
    // rightLayout->setContentsMargins(10, 10, 10, 10);
    // QtText *rightTitle = new QtText("右侧面板", this);
    // rightTitle->setTextPixelSize(14);
    // rightLayout->addWidget(rightTitle);
    // rightLayout->addWidget(new QtText("拖拽中间手柄调整面板大小", 13, this));
    // rightLayout->addStretch();

    // _splitter->addWidget(splitterLeft);
    // _splitter->addWidget(splitterRight);
    // _splitter->setSizes({300, 300});

    // ElaSplitter *vSplitter = new ElaSplitter(Qt::Vertical, this);
    // vSplitter->setFixedHeight(200);

    // QWidget *splitterTop = new QWidget(this);
    // splitterTop->setMinimumHeight(40);
    // QVBoxLayout *topLayout = new QVBoxLayout(splitterTop);
    // topLayout->setContentsMargins(10, 10, 10, 10);
    // topLayout->addWidget(new QtText("上方面板", 14, this));
    // topLayout->addStretch();

    // QWidget *splitterBottom = new QWidget(this);
    // splitterBottom->setMinimumHeight(40);
    // QVBoxLayout *bottomLayout = new QVBoxLayout(splitterBottom);
    // bottomLayout->setContentsMargins(10, 10, 10, 10);
    // bottomLayout->addWidget(new QtText("下方面板", 14, this));
    // bottomLayout->addStretch();

    // vSplitter->addWidget(splitterTop);
    // vSplitter->addWidget(splitterBottom);
    // vSplitter->setSizes({100, 100});

    // QtScrollPageArea *splitterArea = new QtScrollPageArea(this);
    // splitterArea->setFixedHeight(460);
    // QVBoxLayout *splitterMainLayout = new QVBoxLayout(splitterArea);
    // splitterMainLayout->addWidget(new QtText("ElaSplitter", 15, this));
    // splitterMainLayout->addSpacing(5);
    // splitterMainLayout->addWidget(new QtText("水平分割", 13, this));
    // splitterMainLayout->addWidget(_splitter);
    // splitterMainLayout->addSpacing(5);
    // splitterMainLayout->addWidget(new QtText("垂直分割", 13, this));
    // splitterMainLayout->addWidget(vSplitter);

    // ========== ElaInfoBar 示例 ==========
    // _infoBarInfo = new ElaInfoBar(ElaInfoBarType::Informational, this);
    // _infoBarInfo->setTitle("提示");
    // _infoBarInfo->setMessage("这是一条信息提示，用于展示一般性的通知内容。");

    // _infoBarSuccess = new ElaInfoBar(ElaInfoBarType::Success, this);
    // _infoBarSuccess->setTitle("成功");
    // _infoBarSuccess->setMessage("文件已成功保存到指定路径。");
    // _infoBarSuccess->addAction("查看", [=]() { qDebug() << "查看按钮被点击"; });

    // _infoBarWarning = new ElaInfoBar(ElaInfoBarType::Warning, this);
    // _infoBarWarning->setTitle("警告");
    // _infoBarWarning->setMessage("你的许可证将在 7 天后过期。");
    // _infoBarWarning->addAction("续费", [=]() { qDebug() << "续费按钮被点击"; });
    // _infoBarWarning->addAction("稍后提醒", [=]() { qDebug() << "稍后提醒按钮被点击"; });

    // _infoBarError = new ElaInfoBar(ElaInfoBarType::Error, this);
    // _infoBarError->setTitle("错误");
    // _infoBarError->setMessage("无法连接到服务器，请检查网络设置。");
    // _infoBarError->setIsClosable(false);
    // _infoBarError->addAction("重试", [=]() { qDebug() << "重试按钮被点击"; });

    // ========== 中心布局 ==========
    QVBoxLayout* c = new QVBoxLayout(centralWidget);
    c->setContentsMargins(0, 0, 0, 0);
    c->addWidget(badgeArea);
    c->addWidget(badgeControlArea);
    c->addWidget(severityArea);
    c->addWidget(passwordArea);
    c->addWidget(numberArea);
    c->addWidget(tagArea);
    c->addWidget(tagCloseArea);
    c->addWidget(tagCheckArea);
    c->addWidget(splitArea);
    c->addWidget(personArea);
    c->addWidget(ratingArea);
    c->addWidget(flyoutArea);
    c->addWidget(teachingTipArea);
    c->addWidget(tipPosArea);
    c->addWidget(divider1);
    c->addWidget(skelArea);
    c->addWidget(divider2);
    c->addLayout(stepsCtrl);
    c->addWidget(_steps);
    c->addWidget(cmdArea);
    c->addWidget(pgArea);
    c->addWidget(pageStack);
    c->addWidget(expanderText);
    c->addWidget(_expander);
    c->addWidget(expanderUpText);
    c->addWidget(_expanderUp);
    // c->addWidget(captchaArea);
    // c->addLayout(statLayout);
    // c->addWidget(dropArea);
    // c->addWidget(selectorArea);
    // c->addWidget(toastArea);
    // c->addWidget(wizardArea);
    // c->addWidget(notifArea);
    // c->addSpacing(5);
    // c->addWidget(new QtText("ElaMarkdownViewer", 15, this));
    // c->addWidget(_markdownViewer);
    // c->addSpacing(5);
    c->addWidget(new QtText("QtTimeline", 15, this));
    c->addWidget(_timeline);
    c->addSpacing(10);
    // c->addWidget(virtualListArea);
    // c->addWidget(splashArea);
    // c->addWidget(sheetArea);
    // c->addWidget(spotlightArea);
    // c->addWidget(countdownArea);
    // c->addWidget(popconfirmArea);
    // c->addWidget(watermarkArea);
    // c->addWidget(snackbarArea);
    // c->addWidget(splitterArea);

    // ========== ElaQRCode ==========
    // _qrCode = new ElaQRCode("https://github.com/RainbowCandyX/ElaWidgetTools", this);
    // _qrCode->setFixedSize(180, 180);

    // QtLineEdit *qrInput = new QtLineEdit(this);
    // qrInput->setText("https://github.com/RainbowCandyX/ElaWidgetTools");
    // qrInput->setFixedHeight(35);
    // qrInput->setMinimumWidth(300);
    // connect(qrInput, &QtLineEdit::textChanged, this, [=](const QString &text)
    // {
    // 	_qrCode->setText(text);
    // });

    // ElaComboBox *qrEcLevel = new ElaComboBox(this);
    // qrEcLevel->addItems({"低 (7%)", "中 (15%)", "较高 (25%)", "高 (30%)"});
    // qrEcLevel->setCurrentIndex(1);
    // connect(qrEcLevel, &ElaComboBox::currentIndexChanged, this, [=](int index)
    // {
    // 	_qrCode->setErrorCorrectionLevel(static_cast<ElaQRCode::ErrorCorrectionLevel>(index));
    // });

    // QtScrollPageArea *qrArea = new QtScrollPageArea(this);
    // qrArea->setFixedHeight(240);
    // QVBoxLayout *qrMainLayout = new QVBoxLayout(qrArea);
    // QHBoxLayout *qrHeader = new QHBoxLayout();
    // qrHeader->addWidget(new QtText("ElaQRCode", 15, this));
    // qrHeader->addWidget(qrInput);
    // qrHeader->addSpacing(10);
    // qrHeader->addWidget(new QtText("纠错:", 13, this));
    // qrHeader->addWidget(qrEcLevel);
    // qrHeader->addStretch();
    // qrMainLayout->addLayout(qrHeader);
    // QHBoxLayout *qrBody = new QHBoxLayout();
    // qrBody->addWidget(_qrCode);
    // qrBody->addStretch();
    // qrMainLayout->addLayout(qrBody);
    // c->addWidget(qrArea);

    // _emojiPicker = new ElaEmojiPicker(this);

    // QtLineEdit *emojiInput = new QtLineEdit(this);
    // emojiInput->setPlaceholderText(QString::fromUtf8("点击表情按钮插入..."));
    // emojiInput->setFixedHeight(35);
    // emojiInput->setMinimumWidth(300);

    // QtPushButton *emojiBtn = new QtPushButton(QString::fromUtf8("😀"), this);
    // emojiBtn->setFixedSize(40, 35);
    // connect(emojiBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_emojiPicker->popup(emojiBtn);
    // });
    // connect(_emojiPicker, &ElaEmojiPicker::emojiSelected, this, [=](const QString &emoji)
    // {
    // 	emojiInput->setText(emojiInput->text() + emoji);
    // });

    // QtScrollPageArea *emojiArea = new QtScrollPageArea(this);
    // QHBoxLayout *emojiLayout = new QHBoxLayout(emojiArea);
    // emojiLayout->addWidget(new QtText("ElaEmojiPicker", 15, this));
    // emojiLayout->addSpacing(10);
    // emojiLayout->addWidget(emojiInput);
    // emojiLayout->addWidget(emojiBtn);
    // emojiLayout->addStretch();
    // c->addWidget(emojiArea);

    // _floatButton = new ElaFloatButton(QtIconType::Plus, this);
    // QtMenu *floatMenu = new QtMenu(this);
    // floatMenu->addQtIconAction(QtIconType::Pen, "新建笔记");
    // floatMenu->addQtIconAction(QtIconType::Upload, "上传文件");
    // floatMenu->addQtIconAction(QtIconType::Share, "分享");
    // _floatButton->setMenu(floatMenu);

    // QtScrollPageArea *floatBtnArea = new QtScrollPageArea(this);
    // QHBoxLayout *floatBtnLayout = new QHBoxLayout(floatBtnArea);
    // floatBtnLayout->addWidget(new QtText("ElaFloatButton", 15, this));
    // floatBtnLayout->addSpacing(10);
    // floatBtnLayout->addWidget(new QtText("位置:", 13, this));
    // ElaComboBox *floatPosCombo = new ElaComboBox(this);
    // floatPosCombo->addItems({"右下", "左下", "右上", "左上"});
    // connect(floatPosCombo, &ElaComboBox::currentIndexChanged, this, [=](int index)
    // {
    // 	_floatButton->setPosition(static_cast<ElaFloatButton::Position>(index));
    // });
    // floatBtnLayout->addWidget(floatPosCombo);
    // floatBtnLayout->addSpacing(10);
    // floatBtnLayout->addWidget(new QtText("边距:", 13, this));
    // QtSlider *floatMarginSlider = new QtSlider(this);
    // floatMarginSlider->setRange(10, 100);
    // floatMarginSlider->setValue(30);
    // floatMarginSlider->setFixedWidth(150);
    // connect(floatMarginSlider, &QtSlider::valueChanged, this, [=](int value)
    // {
    // 	_floatButton->setMargin(value);
    // });
    // floatBtnLayout->addWidget(floatMarginSlider);
    // floatBtnLayout->addStretch();
    // c->addWidget(floatBtnArea);

    // c->addSpacing(5);
    // c->addWidget(new QtText("ElaInfoBar", 15, this));
    // c->addWidget(_infoBarInfo);
    // c->addWidget(_infoBarSuccess);
    // c->addWidget(_infoBarWarning);
    // c->addWidget(_infoBarError);

    // ========== ElaAutoComplete 示例 ==========
    // _autoComplete = new ElaAutoComplete(this);
    // _autoComplete->setPlaceholderText("输入城市名称...");
    // _autoComplete->setCompletions({
    // 	"北京", "北京大兴机场", "北京西站", "上海", "上海虹桥", "上海浦东",
    // 	"广州", "广州南站", "深圳", "深圳北站", "杭州", "杭州西湖",
    // 	"成都", "成都天府", "武汉", "南京", "南京南站", "西安",
    // 	"重庆", "天津", "苏州", "长沙",
    // 	"Singapore", "Hong Kong", "Taiwan", "Tokyo", "Seoul",
    // 	"Bangkok", "New York", "London", "Paris", "Sydney",
    // 	"Los Angeles", "San Francisco", "Chicago", "Toronto", "Vancouver",
    // 	"Berlin", "Amsterdam", "Dubai", "Mumbai", "Melbourne",
    // 	"Oslo", "Stockholm", "Helsinki", "Lisbon", "Barcelona"
    // });

    // QtText *autoCompleteResult = new QtText("", this);
    // autoCompleteResult->setTextPixelSize(13);
    // connect(_autoComplete, &ElaAutoComplete::completionSelected, this, [=](const QString &text)
    // {
    // 	autoCompleteResult->setText(QString("已选择: %1").arg(text));
    // });

    // ElaComboBox *matchModeCombo = new ElaComboBox(this);
    // matchModeCombo->addItems({"包含匹配", "前缀匹配", "后缀匹配", "正则表达式"});
    // connect(matchModeCombo, &ElaComboBox::currentIndexChanged, this, [=](int index)
    // {
    // 	_autoComplete->setMatchMode(static_cast<ElaAutoComplete::MatchMode>(index));
    // });

    // QtScrollPageArea *autoCompleteArea = new QtScrollPageArea(this);
    // QHBoxLayout *autoCompleteLayout = new QHBoxLayout(autoCompleteArea);
    // autoCompleteLayout->addWidget(new QtText("ElaAutoComplete", 15, this));
    // autoCompleteLayout->addSpacing(10);
    // autoCompleteLayout->addWidget(_autoComplete);
    // autoCompleteLayout->addSpacing(10);
    // autoCompleteLayout->addWidget(matchModeCombo);
    // autoCompleteLayout->addSpacing(10);
    // autoCompleteLayout->addWidget(autoCompleteResult);
    // autoCompleteLayout->addStretch();
    // c->addWidget(autoCompleteArea);

    // ========== ElaTreeSelect 示例 ==========
    // _treeSelect = new ElaTreeSelect(this);
    // _treeSelect->setPlaceholderText("请选择分类...");
    // _treeSelect->setMinimumWidth(280);

    // QStandardItemModel *treeModel = new QStandardItemModel(this);
    // QStandardItem *china = new QStandardItem("中国");
    // QStandardItem *beijing = new QStandardItem("北京");
    // beijing->appendRow(new QStandardItem("朝阳区"));
    // beijing->appendRow(new QStandardItem("海淀区"));
    // beijing->appendRow(new QStandardItem("西城区"));
    // china->appendRow(beijing);
    // QStandardItem *shanghai = new QStandardItem("上海");
    // shanghai->appendRow(new QStandardItem("浦东新区"));
    // shanghai->appendRow(new QStandardItem("黄浦区"));
    // china->appendRow(shanghai);
    // QStandardItem *guangdong = new QStandardItem("广东");
    // guangdong->appendRow(new QStandardItem("深圳"));
    // guangdong->appendRow(new QStandardItem("广州"));
    // china->appendRow(guangdong);
    // treeModel->appendRow(china);

    // QStandardItem *usa = new QStandardItem("美国");
    // QStandardItem *california = new QStandardItem("加利福尼亚");
    // california->appendRow(new QStandardItem("洛杉矶"));
    // california->appendRow(new QStandardItem("旧金山"));
    // usa->appendRow(california);
    // QStandardItem *newYork = new QStandardItem("纽约州");
    // newYork->appendRow(new QStandardItem("纽约市"));
    // usa->appendRow(newYork);
    // treeModel->appendRow(usa);

    // _treeSelect->setModel(treeModel);

    // QtText *treeSelectResult = new QtText("", this);
    // treeSelectResult->setTextPixelSize(13);
    // connect(_treeSelect, &ElaTreeSelect::currentTextChanged, this, [=](const QString &text)
    // {
    // 	treeSelectResult->setText(QString("已选择: %1").arg(text));
    // });

    // QtScrollPageArea *treeSelectArea = new QtScrollPageArea(this);
    // QHBoxLayout *treeSelectLayout = new QHBoxLayout(treeSelectArea);
    // treeSelectLayout->addWidget(new QtText("ElaTreeSelect", 15, this));
    // treeSelectLayout->addSpacing(10);
    // treeSelectLayout->addWidget(_treeSelect);
    // treeSelectLayout->addSpacing(10);
    // treeSelectLayout->addWidget(treeSelectResult);
    // treeSelectLayout->addStretch();
    // c->addWidget(treeSelectArea);

    // // ========== ElaUploadArea 示例 ==========
    // _uploadArea = new ElaUploadArea(this);
    // _uploadArea->setFixedHeight(200);
    // _uploadArea->setAcceptedSuffixes({"png", "jpg", "jpeg", "gif", "bmp", "pdf", "txt"});
    // _uploadArea->setMaxFileCount(5);
    // _uploadArea->setTitle("拖拽文件到此处");
    // _uploadArea->setSubTitle("支持 png, jpg, gif, bmp, pdf, txt (最多5个)");
    // _uploadArea->setDialogTitle("选择要上传的文件");

    // QtText *uploadInfo = new QtText("已选择 0 个文件", this);
    // uploadInfo->setTextPixelSize(13);
    // connect(_uploadArea, &ElaUploadArea::filesSelected, this, [=](const QStringList &files)
    // {
    // 	uploadInfo->setText(QString("已选择 %1 个文件").arg(files.size()));
    // });
    // connect(_uploadArea, &ElaUploadArea::fileRejected, this, [=](const QString &path, const
    // QString &reason)
    // {
    // 	Q_UNUSED(path)
    // 	ElaToast::warning(reason, 2000, window());
    // });

    // QtPushButton *clearUploadBtn = new QtPushButton("清空文件", this);
    // clearUploadBtn->setFixedSize(100, 35);
    // connect(clearUploadBtn, &QtPushButton::clicked, this, [=]()
    // {
    // 	_uploadArea->clearFiles();
    // 	uploadInfo->setText("已选择 0 个文件");
    // });

    // QtScrollPageArea *uploadAreaContainer = new QtScrollPageArea(this);
    // uploadAreaContainer->setFixedHeight(280);
    // QVBoxLayout *uploadMainLayout = new QVBoxLayout(uploadAreaContainer);
    // QHBoxLayout *uploadHeader = new QHBoxLayout();
    // uploadHeader->addWidget(new QtText("ElaUploadArea", 15, this));
    // uploadHeader->addSpacing(10);
    // uploadHeader->addWidget(uploadInfo);
    // uploadHeader->addSpacing(10);
    // uploadHeader->addWidget(clearUploadBtn);
    // uploadHeader->addStretch();
    // uploadMainLayout->addLayout(uploadHeader);
    // uploadMainLayout->addWidget(_uploadArea);
    // c->addWidget(uploadAreaContainer);

    // _copyButton = new ElaCopyButton(this);
    // _copyButton->setCopyText("https://github.com/RainbowCandyX/ElaWidgetTools");

    // QtLineEdit *copyInput = new QtLineEdit(this);
    // copyInput->setText("https://github.com/RainbowCandyX/ElaWidgetTools");
    // copyInput->setFixedHeight(35);
    // copyInput->setMinimumWidth(350);
    // connect(copyInput, &QtLineEdit::textChanged, this, [=](const QString &text)
    // {
    // 	_copyButton->setCopyText(text);
    // });

    // QtText *copyStatus = new QtText("", this);
    // copyStatus->setTextPixelSize(13);
    // connect(_copyButton, &ElaCopyButton::copyCompleted, this, [=](const QString &text)
    // {
    // 	Q_UNUSED(text)
    // 	copyStatus->setText("已复制!");
    // 	QTimer::singleShot(2000, this, [=]()
    // 	{
    // 		copyStatus->setText("");
    // 	});
    // });

    // QtScrollPageArea *copyArea = new QtScrollPageArea(this);
    // QHBoxLayout *copyLayout = new QHBoxLayout(copyArea);
    // copyLayout->addWidget(new QtText("ElaCopyButton", 15, this));
    // copyLayout->addSpacing(10);
    // copyLayout->addWidget(copyInput);
    // copyLayout->addWidget(_copyButton);
    // copyLayout->addSpacing(5);
    // copyLayout->addWidget(copyStatus);
    // copyLayout->addStretch();
    // c->addWidget(copyArea);

    // c->addSpacing(5);
    // c->addLayout(transferHeader);
    // c->addWidget(_transfer);
    // c->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}
T_NewComponents::~T_NewComponents()
{
}
