#include "qttoolbutton.h"
#include "qticon.h"
#include "styles/qttoolbuttonstyle.h"
#include "widgets/qtmenu.h"
#include <qnamespace.h>

class QtToolButtonPrivate
{
    Q_DECLARE_PUBLIC(QtToolButton)
public:
    QtToolButton* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtToolButtonStyle* toolButtonStyle;
    QtToolButtonPrivate(QtToolButton* q);
};

QtToolButtonPrivate::QtToolButtonPrivate(QtToolButton* q)
    : q_ptr(q)
{
}

QtToolButton::QtToolButton(QWidget* parent)
    : QToolButton(parent)
    , d_ptr(new QtToolButtonPrivate(this))
{
    Q_D(QtToolButton);
    setIconSize(QSize(22, 22));
    setPopupMode(QToolButton::InstantPopup);
    d->toolButtonStyle = new QtToolButtonStyle(style());
    setStyle(d->toolButtonStyle);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);
}

QtToolButton::~QtToolButton()
{
    Q_D(QtToolButton);
    delete d->toolButtonStyle;
}

int QtToolButton::borderRadius() const
{
    Q_D(const QtToolButton);
    return d->toolButtonStyle->borderRadius();
}

void QtToolButton::setBorderRadius(int newBorderRadius)
{
    Q_D(QtToolButton);
    if(d->toolButtonStyle->borderRadius() == newBorderRadius)
        return;
    d->toolButtonStyle->setBorderRadius(newBorderRadius);
    Q_EMIT borderRadiusChanged();
}

bool QtToolButton::isSelected() const
{
    Q_D(const QtToolButton);
    return d->toolButtonStyle->isSelected();
}

void QtToolButton::setIsSelected(bool newIsSelected)
{
    Q_D(QtToolButton);
    if(d->toolButtonStyle->isSelected() == newIsSelected)
        return;
    d->toolButtonStyle->setIsSelected(newIsSelected);
    Q_EMIT isSelectedChanged();
}

void QtToolButton::setIsTransparent(bool isTransparent)
{
    Q_D(QtToolButton);
    if(d->toolButtonStyle->isTransparent() == isTransparent)
        return;
    d->toolButtonStyle->setIsTransparent(isTransparent);
    update();
}
bool QtToolButton::isTransparent() const
{
    Q_D(const QtToolButton);
    return d->toolButtonStyle->isTransparent();
}

void QtToolButton::setMenu(QtMenu* menu)
{
    Q_D(QtToolButton);
    if(!menu || menu == this->menu())
    {
        return;
    }
    menu->setMenuItemHeight(27);
    QToolButton::setMenu(menu);
    menu->installEventFilter(this);
}
void QtToolButton::setQtIcon(QtIconType::IconName icon)
{
    Q_D(QtToolButton);
    setProperty("QtIconType", QChar(static_cast<ushort>(icon)));
    setIcon(QtIcon::getInstance()->getQtIcon(QtIconType::Broom, 1));
}
void QtToolButton::setQtIcon(QtIconType::IconName icon, int rotate)
{
    Q_D(QtToolButton);
    setQtIcon(icon);
    setProperty("QtIconRotate", rotate);
}

bool QtToolButton::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtToolButton);
    if(watched == menu())
    {
        switch(event->type())
        {
            case QEvent::Show: {
                QPropertyAnimation* rotateAnimation =
                    new QPropertyAnimation(d->toolButtonStyle, "expandIconRotate");
                connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
                    [this](const QVariant& value) { update(); });
                rotateAnimation->setDuration(300);
                rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
                rotateAnimation->setStartValue(d->toolButtonStyle->expandIconRotate());
                rotateAnimation->setEndValue(-180);
                rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                break;
            }
            case QEvent::Hide: {
                QPropertyAnimation* rotateAnimation =
                    new QPropertyAnimation(d->toolButtonStyle, "expandIconRotate");
                connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
                    [this](const QVariant& value) { update(); });
                rotateAnimation->setDuration(300);
                rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
                rotateAnimation->setStartValue(d->toolButtonStyle->expandIconRotate());
                rotateAnimation->setEndValue(0);
                rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1),
                    Qt::NoButton, Qt::NoButton, Qt::NoModifier);
                QApplication::sendEvent(parentWidget(), &focusEvent);
                break;
            }
            default: {
                break;
            }
        }
    }
    return QObject::eventFilter(watched, event);
}
