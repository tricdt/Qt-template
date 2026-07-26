#include "T_BasePage.h"
#include "qttheme.h"
#include "widgets/qtmenu.h"
#include "widgets/qttext.h"
#include "widgets/qttoolbutton.h"
#include "widgets/qtwindow.h"

T_BasePage::T_BasePage(QWidget* parent)
    : QtScrollPage{ parent }
{
}

T_BasePage::~T_BasePage()
{
}

void T_BasePage::createCustomWidget(QString desText)
{
    QWidget* customWidget = new QWidget(this);
    QtText* subTitleText = new QtText(this);
    subTitleText->setText("https://github.com/Liniyous/salaelect");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(11);

    QtToolButton* documentationButton = new QtToolButton(this);
    documentationButton->setFixedHeight(35);
    documentationButton->setIsTransparent(false);
    documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    documentationButton->setText("Documentation");
    documentationButton->setQtIcon(QtIconType::FileDoc);
    QtMenu* documentationMenu = new QtMenu(this);
    documentationMenu->addQtIconAction(QtIconType::CardsBlank, "CardsBlank");
    documentationMenu->addQtIconAction(QtIconType::EarthAmericas, "EarthAmericas");
    documentationButton->setMenu(documentationMenu);

    QtToolButton* sourceButton = new QtToolButton(this);
    sourceButton->setFixedHeight(35);
    sourceButton->setIsTransparent(false);
    sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sourceButton->setText("Source");
    sourceButton->setQtIcon(QtIconType::NfcSymbol);
    QtMenu* sourceMenu = new QtMenu(this);
    sourceMenu->addQtIconAction(QtIconType::FireBurner, "FireBurner");
    sourceMenu->addQtIconAction(QtIconType::Galaxy, "Galaxy~~~~");
    sourceButton->setMenu(sourceMenu);

    QtToolButton* themeButton = new QtToolButton(this);
    themeButton->setFixedSize(35, 35);
    themeButton->setIsTransparent(false);
    themeButton->setQtIcon(QtIconType::MoonStars);
    connect(themeButton, &QtToolButton::clicked, this, [this]() {
        qtTheme->setThemeMode(qtTheme->getThemeMode() == QtThemeType::Light ? QtThemeType::Dark
                                                                           : QtThemeType::Light);
    });

    QtToolButton* backtrackButton = new QtToolButton(this);
    backtrackButton->setFixedSize(35, 35);
    backtrackButton->setIsTransparent(false);
    backtrackButton->setQtIcon(QtIconType::Timer);
    connect(backtrackButton, &QtToolButton::clicked, this, [this]() {
        QtWindow* window = dynamic_cast<QtWindow*>(this->window());
        if(window)
        {
            window->backtrackNavigationNode(property("QtPageKey").toString());
        }
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(backtrackButton);
    buttonLayout->addSpacing(15);

    QtText* descText = new QtText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}
