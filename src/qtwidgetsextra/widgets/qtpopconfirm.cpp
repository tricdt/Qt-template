#include "qtpopconfirm.h"

class QtPopConfirmPrivate
{
    Q_DECLARE_PUBLIC(QtPopConfirm)
public:
int borderRadius;
    QString title;
    QString content;
    QString confirmButtonText;
    QString cancelButtonText;
    QtIconType::IconName icon;
    bool isLightDismiss;
    QtPopConfirm* q_ptr;
    QtPopConfirmPrivate(QtPopConfirm* q);
    ~QtPopConfirmPrivate();
};

QtPopConfirmPrivate::QtPopConfirmPrivate(QtPopConfirm* q) : q_ptr(q)
{
}
QtPopConfirmPrivate::~QtPopConfirmPrivate()
{
}

QtPopConfirm::QtPopConfirm(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtPopConfirmPrivate(this))
{
}

QtPopConfirm::~QtPopConfirm()
{
}

int QtPopConfirm::borderRadius() const
{
    Q_D(const QtPopConfirm);
    return d->borderRadius;
}

void QtPopConfirm::setBorderRadius(int value)
{
    Q_D(QtPopConfirm);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

const QString& QtPopConfirm::title() const
{
    Q_D(const QtPopConfirm);
    return d->title;
}

void QtPopConfirm::setTitle(const QString &value)
{
    Q_D(QtPopConfirm);
    if (d->title != value) {
        d->title = value;
        Q_EMIT titleChanged();
    }
}

const QString& QtPopConfirm::content() const
{
    Q_D(const QtPopConfirm);
    return d->content;
}

void QtPopConfirm::setContent(const QString &value)
{
    Q_D(QtPopConfirm);
    if (d->content != value) {
        d->content = value;
        Q_EMIT contentChanged();
    }
}

const QString& QtPopConfirm::confirmButtonText() const
{
    Q_D(const QtPopConfirm);
    return d->confirmButtonText;
}

void QtPopConfirm::setConfirmButtonText(const QString &value)
{
    Q_D(QtPopConfirm);
    if (d->confirmButtonText != value) {
        d->confirmButtonText = value;
        Q_EMIT confirmButtonTextChanged();
    }
}

const QString& QtPopConfirm::cancelButtonText() const
{
    Q_D(const QtPopConfirm);
    return d->cancelButtonText;
}

void QtPopConfirm::setCancelButtonText(const QString &value)
{
    Q_D(QtPopConfirm);
    if (d->cancelButtonText != value) {
        d->cancelButtonText = value;
        Q_EMIT cancelButtonTextChanged();
    }
}

QtIconType::IconName QtPopConfirm::icon() const
{
    Q_D(const QtPopConfirm);
    return d->icon;
}

void QtPopConfirm::setIcon(QtIconType::IconName value)
{
    Q_D(QtPopConfirm);
    if (d->icon != value) {
        d->icon = value;
        Q_EMIT iconChanged();
    }
}

bool QtPopConfirm::isLightDismiss() const
{
    Q_D(const QtPopConfirm);
    return d->isLightDismiss;
}

void QtPopConfirm::setIsLightDismiss(bool value)
{
    Q_D(QtPopConfirm);
    if (d->isLightDismiss != value) {
        d->isLightDismiss = value;
        Q_EMIT isLightDismissChanged();
    }
}