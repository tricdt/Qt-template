#include "qtdrawerarea.h"
#include "qtdrawercontainer.h"
#include "qtdrawerheader.h"
#include "qttheme.h"
#include <qboxlayout.h>
class QtDrawerAreaPrivate
{
    Q_DECLARE_PUBLIC(QtDrawerArea)
public:
    QtDrawerArea* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtDrawerHeader* drawerHeader{ nullptr };
    QtDrawerContainer* drawerContainer{ nullptr };
    QtDrawerAreaPrivate(QtDrawerArea* q);
    ~QtDrawerAreaPrivate();
    void onDrawerHeaderClicked(bool isExpand);
};

QtDrawerAreaPrivate::QtDrawerAreaPrivate(QtDrawerArea* q)
    : q_ptr(q)
{
}
QtDrawerAreaPrivate::~QtDrawerAreaPrivate()
{
}
void QtDrawerAreaPrivate::onDrawerHeaderClicked(bool isExpand)
{
    Q_Q(QtDrawerArea);
    drawerContainer->doDrawerAnimation(isExpand);
    Q_EMIT q->expandStateChanged(isExpand);
}
QtDrawerArea::QtDrawerArea(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtDrawerAreaPrivate(this))
{
    Q_D(QtDrawerArea);
    setObjectName("QtDrawerArea");
    setStyleSheet("#QtDrawerArea{background-color:transparent;}");

    d->drawerHeader = new QtDrawerHeader(this);
    d->drawerContainer = new QtDrawerContainer(this);
    connect(d->drawerHeader, &QtDrawerHeader::drawerHeaderClicked, this,
        [this](bool isExpand) { d_ptr->onDrawerHeaderClicked(isExpand); });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(d->drawerHeader);
    mainLayout->addWidget(d->drawerContainer);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtDrawerArea::~QtDrawerArea()
{
}

void QtDrawerArea::setDrawerHeader(QWidget* widget)
{
    Q_D(QtDrawerArea);
    d->drawerHeader->setHeaderWidget(widget);
}

void QtDrawerArea::addDrawer(QWidget* widget)
{
    Q_D(QtDrawerArea);
    d->drawerContainer->addWidget(widget);
}
void QtDrawerArea::removeDrawer(QWidget* widget)
{
    Q_D(QtDrawerArea);
    d->drawerContainer->removeWidget(widget);
}

void QtDrawerArea::expand()
{
    Q_D(QtDrawerArea);
    if(isExpand())
        return;
    d->drawerHeader->setIsExpand(true);
    d->drawerHeader->doExpandOrCollapseAnimation();
    d->drawerContainer->doDrawerAnimation(true);
    Q_EMIT expandStateChanged(true);
}
void QtDrawerArea::collapse()
{
    Q_D(QtDrawerArea);
    if(!isExpand())
        return;
    d->drawerHeader->setIsExpand(false);
    d->drawerHeader->doExpandOrCollapseAnimation();
    d->drawerContainer->doDrawerAnimation(false);
    Q_EMIT expandStateChanged(false);
}

bool QtDrawerArea::isExpand() const
{
    Q_D(const QtDrawerArea);
    return d->drawerHeader->isExpand();
}

int QtDrawerArea::borderRadius() const
{
    Q_D(const QtDrawerArea);
    return d->drawerHeader->borderRadius();
}
void QtDrawerArea::setBorderRadius(int borderRadius)
{
    Q_D(QtDrawerArea);
    d->drawerHeader->setBorderRadius(borderRadius);
    d->drawerContainer->setBorderRadius(borderRadius);
    Q_EMIT borderRadiusChanged();
}

int QtDrawerArea::headerHeight() const
{
    Q_D(const QtDrawerArea);
    return d->drawerHeader->height();
}
void QtDrawerArea::setHeaderHeight(int value)
{
    Q_D(QtDrawerArea);
    d->drawerHeader->setFixedHeight(value);
}