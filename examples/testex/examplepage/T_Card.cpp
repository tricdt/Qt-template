#include "T_Card.h"
#include <qboxlayout.h>

T_Card::T_Card(QWidget* parent)
    : T_BasePage{ parent }
{
    setWindowTitle("QtCard");

    createCustomWidget("一些常用的卡片组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    _lcdNumber = new QtLCDNumber(this);
    _lcdNumber->setIsUseAutoClock(true);
    _lcdNumber->setIsTransparent(false);
    // _lcdNumber->setAutoClockFormat("hh:mm:ss");
    _lcdNumber->setFixedHeight(100);

    _promotionCard = new QtPromotionCard(this);
    _promotionCard->setFixedSize(600, 300);
    _promotionCard->setCardPixmap(QPixmap(":/include/miku.png"));
    _promotionCard->setCardTitle("MiKu");
    _promotionCard->setPromotionTitle("SONG~");
    _promotionCard->setTitle("STYX HELIX");
    _promotionCard->setSubTitle("Never close your eyes, Searching for a true fate");

    _promotionView = new QtPromotionView(this);

    QtPromotionCard* exampleCard1 = new QtPromotionCard(this);
    exampleCard1->setCardPixmap(QPixmap(":/include/miku.png"));
    exampleCard1->setCardTitle("MiKu");
    exampleCard1->setPromotionTitle("SONG~");
    exampleCard1->setTitle("STYX HELIX");
    exampleCard1->setSubTitle("Never close your eyes, Searching for a true fate");

    QtPromotionCard* exampleCard2 = new QtPromotionCard(this);
    exampleCard2->setCardPixmap(QPixmap(":/include/beach.png"));
    exampleCard2->setCardTitle("Beach");
    exampleCard2->setPromotionTitle("SONG~");
    exampleCard2->setTitle("STYX HELIX");
    exampleCard2->setSubTitle("Never close your eyes, Searching for a true fate");

    QtPromotionCard* exampleCard3 = new QtPromotionCard(this);
    exampleCard3->setCardPixmap(QPixmap(":/include/dream.png"));
    exampleCard3->setCardTitle("Dream");
    exampleCard3->setPromotionTitle("SONG~");
    exampleCard3->setTitle("STYX HELIX");
    exampleCard3->setSubTitle("Never close your eyes, Searching for a true fate");

    QtPromotionCard* exampleCard4 = new QtPromotionCard(this);
    exampleCard4->setCardPixmap(QPixmap(":/include/classroom.png"));
    exampleCard4->setCardTitle("Classroom");
    exampleCard4->setPromotionTitle("SONG~");
    exampleCard4->setTitle("STYX HELIX");
    exampleCard4->setSubTitle("Never close your eyes, Searching for a true fate");

    _promotionView->appendPromotionCard(exampleCard1);
    _promotionView->appendPromotionCard(exampleCard2);
    _promotionView->appendPromotionCard(exampleCard3);
    _promotionView->appendPromotionCard(exampleCard4);
    _promotionView->setIsAutoScroll(true);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("QtCard");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(_lcdNumber);
    centerLayout->addSpacing(20);
    centerLayout->addWidget(_promotionCard);
    centerLayout->addSpacing(20);
    centerLayout->addWidget(_promotionView);
    centerLayout->addSpacing(100);
    centerLayout->addStretch();
    addCentralWidget(centralWidget, true, true, 0);
}

T_Card::~T_Card()
{
}
