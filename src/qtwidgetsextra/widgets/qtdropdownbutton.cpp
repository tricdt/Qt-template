#include "qtdropdownbutton.h"

class QtDropDownButtonPrivate
{
    Q_DECLARE_PUBLIC(QtDropDownButton)
public:
    int borderRadius;
    QString text;
    QtIconType::IconName qtIcon;
    QtDropDownButton* q_ptr;
    QtDropDownButtonPrivate(QtDropDownButton* q);
    ~QtDropDownButtonPrivate();
};

QtDropDownButtonPrivate::QtDropDownButtonPrivate(QtDropDownButton* q)
    : q_ptr(q)
{
}
QtDropDownButtonPrivate::~QtDropDownButtonPrivate()
{
}

QtDropDownButton::QtDropDownButton(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtDropDownButtonPrivate(this))
{
}

QtDropDownButton::~QtDropDownButton()
{
}

int QtDropDownButton::borderRadius() const
{
    Q_D(const QtDropDownButton);
    return d->borderRadius;
}

void QtDropDownButton::setBorderRadius(int value)
{
    Q_D(QtDropDownButton);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

const QString& QtDropDownButton::text() const
{
    Q_D(const QtDropDownButton);
    return d->text;
}

void QtDropDownButton::setText(const QString &value)
{
    Q_D(QtDropDownButton);
    if (d->text != value) {
        d->text = value;
        Q_EMIT textChanged();
    }
}

QtIconType::IconName QtDropDownButton::qtIcon() const
{
    Q_D(const QtDropDownButton);
    return d->qtIcon;
}

void QtDropDownButton::setQtIcon(QtIconType::IconName value)
{
    Q_D(QtDropDownButton);
    if (d->qtIcon != value) {
        d->qtIcon = value;
        Q_EMIT qtIconChanged();
    }
}