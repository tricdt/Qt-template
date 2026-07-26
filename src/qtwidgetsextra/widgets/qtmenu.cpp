#include "qtmenu.h"
#include <QAction>
#include <QPainter>
#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>
#include <QVariantAnimation>
#include "qtdef.h"
#include <QStyle>
#include "styles/qtmenustyle.h"

class QtMenuPrivate
{
public:
    QtMenu* q_ptr;
    int animationImagePosY;
    QPixmap animationPix;
    bool isCloseAnimation{ false };
    QtMenuStyle* menuStyle{ nullptr };
    QtMenuPrivate(QtMenu* q);
};

QtMenuPrivate::QtMenuPrivate(QtMenu* q)
    : q_ptr(q)
{
}

QtMenu::QtMenu(QWidget* parent)
    : QMenu(parent)
    , d_ptr(new QtMenuPrivate(this))
{
    Q_D(QtMenu);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("QtMenu");
    d->menuStyle = new QtMenuStyle(style());
    setStyle(d->menuStyle);
    d->animationImagePosY = 0;
}

QtMenu::QtMenu(const QString& title, QWidget* parent)
    : QtMenu(parent)
{
    setTitle(title);
}

QtMenu::~QtMenu()
{
    Q_D(QtMenu);
    delete d->menuStyle;
}

void QtMenu::setMenuItemHeight(int menuItemHeight)
{
    Q_D(QtMenu);
    d->menuStyle->setMenuItemHeight(menuItemHeight);
}
int QtMenu::getMenuItemHeight() const
{
    Q_D(const QtMenu);
    return d->menuStyle->menuItemHeight();
}

QAction* QtMenu::addMenu(QMenu* menu)
{
    return QMenu::addMenu(menu);
}
QtMenu* QtMenu::addMenu(const QString& title)
{
    QtMenu* menu = new QtMenu(title, this);
    QMenu::addAction(menu->menuAction());
    return menu;
}
QtMenu* QtMenu::addMenu(const QIcon& icon, const QString& title)
{
    QtMenu* menu = new QtMenu(title, this);
    menu->setIcon(icon);
    QMenu::addAction(menu->menuAction());
    return menu;
}

QtMenu* QtMenu::addMenu(QtIconType::IconName icon, const QString& title)
{
    QtMenu* menu = new QtMenu(title, this);
    QMenu::addAction(menu->menuAction());
    QChar ch = QChar::fromUcs2(static_cast<char16_t>(icon));
    menu->menuAction()->setProperty("QtIconType", ch);
    return menu;
}

QAction* QtMenu::addQtIconAction(QtIconType::IconName icon, const QString& text)
{
    QAction* action = new QAction(text, this);
    QChar ch = QChar::fromUcs2(static_cast<char16_t>(icon));
    action->setProperty("QtIconType", ch);
    QMenu::addAction(action);
    return action;
}
QAction* QtMenu::addQtIconAction(
    QtIconType::IconName icon, const QString& text, const QKeySequence& shortcut)
{
    QAction* action = new QAction(text, this);
    action->setShortcut(shortcut);
    action->setProperty("QtIconType", QChar::fromUcs2(static_cast<char16_t>(icon)));
    QMenu::addAction(action);
    return action;
}

bool QtMenu::isHasChildMenu() const
{
    QList<QAction*> actionList = this->actions();
    for(auto action : actionList)
    {
        if(action->isSeparator())
        {
            continue;
        }
        if(action->menu())
        {
            return true;
        }
    }
    return false;
}
bool QtMenu::isHasIcon() const
{
    QList<QAction*> actionList = this->actions();
    for(auto action : actionList)
    {
        if(action->isSeparator())
        {
            continue;
        }
        QMenu* menu = action->menu();
        if(menu && (!menu->icon().isNull() || !menu->property("QtIconType").toString().isEmpty()))
        {
            return true;
        }
        if(!action->icon().isNull() || !action->property("QtIconType").toString().isEmpty())
        {
            return true;
        }
    }
    return false;
}

void QtMenu::showEvent(QShowEvent* event)
{
    Q_EMIT menuShow();
    Q_D(QtMenu);
    move(this->pos().x() - 6, this->pos().y());
    updateGeometry();
    if(!d->animationPix.isNull())
    {
        d->animationPix = QPixmap();
    }
    d->animationPix = this->grab(this->rect());
    QVariantAnimation* posAnimation = new QVariantAnimation(this);
    connect(posAnimation, &QVariantAnimation::finished, this, [this]() {
        this->d_ptr->animationPix = QPixmap();
        update();
    });
    connect(posAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        this->d_ptr->animationImagePosY = value.toInt();
        update();
    });
    posAnimation->setEasingCurve(QEasingCurve::OutCubic);
    posAnimation->setDuration(400);
    int targetPosY = height();
    if(targetPosY > 160)
    {
        if(targetPosY < 320)
        {
            targetPosY = 160;
        }
        else
        {
            targetPosY /= 2;
        }
    }
    if(pos().y() + d->menuStyle->menuItemHeight() + 9 >= QCursor::pos().y())
    {
        posAnimation->setStartValue(-targetPosY);
    }
    else
    {
        posAnimation->setStartValue(targetPosY);
    }
    posAnimation->setEndValue(0);
    posAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    QMenu::showEvent(event);
}
void QtMenu::paintEvent(QPaintEvent* event)
{
    Q_D(QtMenu);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    if(!d->animationPix.isNull())
    {
        painter.drawPixmap(QRect(0, d->animationImagePosY, width(), height()), d->animationPix);
    }
    else
    {
        QMenu::paintEvent(event);
    }
}

