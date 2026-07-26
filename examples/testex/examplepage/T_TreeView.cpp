#include "T_TreeView.h"
#include "modelview/T_TreeViewModel.h"
#include "widgets/qtpushbutton.h"
#include "widgets/qtscrollbar.h"
#include "widgets/qtslider.h"
#include "widgets/qttext.h"
#include <QtWidgets>
T_TreeView::T_TreeView(QWidget* parent)
    : T_BasePage{ parent }
{
    setWindowTitle("QtTreeView");

    createCustomWidget("树型视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    T_TreeViewModel* treeModel = new T_TreeViewModel(this);
    QHBoxLayout* treeLayout = new QHBoxLayout();
    treeLayout->setContentsMargins(0, 0, 10, 0);
    QWidget* treeSettingWidget = new QWidget(this);
    QVBoxLayout* treeSettingWidgetLayout = new QVBoxLayout(treeSettingWidget);
    treeSettingWidgetLayout->setContentsMargins(0, 0, 0, 0);
    treeSettingWidgetLayout->setSpacing(15);

    QtText* dataText =
        new QtText(QString("dataText：%1").arg(treeModel->itemCount()), this);
    dataText->setTextPixelSize(15);

    QtText* itemHeightText = new QtText("ItemHeight", this);
    itemHeightText->setTextPixelSize(15);
    QtSlider* itemHeightSlider = new QtSlider(this);
    itemHeightSlider->setRange(200, 600);
    itemHeightSlider->setValue(350);
    QtText* itemHeightValueText = new QtText("35", this);
    itemHeightValueText->setTextPixelSize(15);
    connect(itemHeightSlider, &QtSlider::valueChanged, this, [this, itemHeightValueText](int value) {
        itemHeightValueText->setText(QString::number(value / 10));
        _treeView->setItemHeight(value / 10);
    });
    QHBoxLayout* itemHeightLayout = new QHBoxLayout();
    itemHeightLayout->setContentsMargins(0, 0, 0, 0);
    itemHeightLayout->addWidget(itemHeightText);
    itemHeightLayout->addWidget(itemHeightSlider);
    itemHeightLayout->addWidget(itemHeightValueText);

    QtText* headerMarginText = new QtText("HeaderMargin", this);
    headerMarginText->setTextPixelSize(15);
    QtSlider* headerMarginSlider = new QtSlider(this);
    headerMarginSlider->setRange(0, 200);
    headerMarginSlider->setValue(50);
    QtText* headerMarginValueText = new QtText("5", this);
    headerMarginValueText->setTextPixelSize(15);
    connect(headerMarginSlider, &QtSlider::valueChanged, this, [this, headerMarginValueText](int value) {
        headerMarginValueText->setText(QString::number(value / 10));
        _treeView->setHeaderMargin(value / 10);
    });
    QHBoxLayout* headerMarginLayout = new QHBoxLayout();
    headerMarginLayout->setContentsMargins(0, 0, 0, 0);
    headerMarginLayout->addWidget(headerMarginText);
    headerMarginLayout->addWidget(headerMarginSlider);
    headerMarginLayout->addWidget(headerMarginValueText);

    QtText* indentationText = new QtText("Indentation", this);
    indentationText->setTextPixelSize(15);
    QtSlider* indentationSlider = new QtSlider(this);
    indentationSlider->setRange(200, 1000);
    indentationSlider->setValue(200);
    QtText* indentationValueText = new QtText("20", this);
    indentationValueText->setTextPixelSize(15);
    connect(indentationSlider, &QtSlider::valueChanged, this, [this, indentationValueText](int value) {
        indentationValueText->setText(QString::number(value / 10));
        _treeView->setIndentation(value / 10);
    });
    QHBoxLayout* indentationLayout = new QHBoxLayout();
    indentationLayout->setContentsMargins(0, 0, 0, 0);
    indentationLayout->addWidget(indentationText);
    indentationLayout->addWidget(indentationSlider);
    indentationLayout->addWidget(indentationValueText);

    QHBoxLayout* expandCollapseLayout = new QHBoxLayout();
    expandCollapseLayout->setContentsMargins(0, 0, 0, 0);
    QtPushButton* expandButton = new QtPushButton("Expand", this);
    expandButton->setFixedWidth(80);
    connect(expandButton, &QtPushButton::clicked, this, [this]() { _treeView->expandAll(); });

    QtPushButton* collapseButton = new QtPushButton("Collapse", this);
    collapseButton->setFixedWidth(80);
    connect(collapseButton, &QtPushButton::clicked, this, [this]() { _treeView->collapseAll(); });
    expandCollapseLayout->addWidget(expandButton);
    expandCollapseLayout->addWidget(collapseButton);
    expandCollapseLayout->addStretch();

    treeSettingWidgetLayout->addWidget(dataText);
    treeSettingWidgetLayout->addLayout(itemHeightLayout);
    treeSettingWidgetLayout->addLayout(headerMarginLayout);
    treeSettingWidgetLayout->addLayout(indentationLayout);
    treeSettingWidgetLayout->addLayout(expandCollapseLayout);
    treeSettingWidgetLayout->addStretch();

    QtText* treeText = new QtText("QtTreeView", this);
    treeText->setTextPixelSize(18);
    _treeView = new QtTreeView(this);
    QtScrollBar* treeViewFloatScrollBar =
        new QtScrollBar(_treeView->verticalScrollBar(), _treeView);
    treeViewFloatScrollBar->setIsAnimation(true);
    QFont headerFont = _treeView->header()->font();
    headerFont.setPixelSize(16);
    _treeView->header()->setFont(headerFont);
    _treeView->setFixedHeight(450);
    _treeView->setModel(treeModel);
    QVBoxLayout* treeViewLayout = new QVBoxLayout();
    treeViewLayout->setContentsMargins(0, 0, 0, 0);
    treeViewLayout->addWidget(_treeView);
    treeViewLayout->addStretch();
    treeLayout->addWidget(treeSettingWidget);
    treeLayout->addLayout(treeViewLayout);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("QtView");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(treeText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(treeLayout);
    addCentralWidget(centralWidget, true, false, 0);
}
