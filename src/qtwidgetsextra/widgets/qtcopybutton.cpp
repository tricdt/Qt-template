#include "qtcopybutton.h"

class QtCopyButtonPrivate
{
    Q_DECLARE_PUBLIC(QtCopyButton)
public:
int borderRadius;
    QString copyText;
    int successDuration;
    QString successText;
    QtCopyButton* q_ptr;
    QtCopyButtonPrivate(QtCopyButton* q);
    ~QtCopyButtonPrivate();
};

QtCopyButtonPrivate::QtCopyButtonPrivate(QtCopyButton* q) : q_ptr(q)
{
}
QtCopyButtonPrivate::~QtCopyButtonPrivate()
{
}

QtCopyButton::QtCopyButton(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCopyButtonPrivate(this))
{
}

QtCopyButton::~QtCopyButton()
{
}

int QtCopyButton::borderRadius() const
{
    Q_D(const QtCopyButton);
    return d->borderRadius;
}

void QtCopyButton::setBorderRadius(int value)
{
    Q_D(QtCopyButton);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

const QString& QtCopyButton::copyText() const
{
    Q_D(const QtCopyButton);
    return d->copyText;
}

void QtCopyButton::setCopyText(const QString &value)
{
    Q_D(QtCopyButton);
    if (d->copyText != value) {
        d->copyText = value;
        Q_EMIT copyTextChanged();
    }
}

int QtCopyButton::successDuration() const
{
    Q_D(const QtCopyButton);
    return d->successDuration;
}

void QtCopyButton::setSuccessDuration(int value)
{
    Q_D(QtCopyButton);
    if (d->successDuration != value) {
        d->successDuration = value;
        Q_EMIT successDurationChanged();
    }
}

const QString& QtCopyButton::successText() const
{
    Q_D(const QtCopyButton);
    return d->successText;
}

void QtCopyButton::setSuccessText(const QString &value)
{
    Q_D(QtCopyButton);
    if (d->successText != value) {
        d->successText = value;
        Q_EMIT successTextChanged();
    }
}