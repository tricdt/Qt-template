#include "qtinfobar.h"

class QtInfoBarPrivate
{
    Q_DECLARE_PUBLIC(QtInfoBar)
public:
int borderRadius;
    QString title;
    QString message;
    QtInfoBarType::InfoBarSeverity severity;
    bool isClosable;
    QtIconType::IconName infoBarIcon;
    QtInfoBar* q_ptr;
    QtInfoBarPrivate(QtInfoBar* q);
    ~QtInfoBarPrivate();
};

QtInfoBarPrivate::QtInfoBarPrivate(QtInfoBar* q) : q_ptr(q)
{
}
QtInfoBarPrivate::~QtInfoBarPrivate()
{
}

QtInfoBar::QtInfoBar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtInfoBarPrivate(this))
{
}

QtInfoBar::~QtInfoBar()
{
}

int QtInfoBar::borderRadius() const
{
    Q_D(const QtInfoBar);
    return d->borderRadius;
}

void QtInfoBar::setBorderRadius(int value)
{
    Q_D(QtInfoBar);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

const QString& QtInfoBar::title() const
{
    Q_D(const QtInfoBar);
    return d->title;
}

void QtInfoBar::setTitle(const QString &value)
{
    Q_D(QtInfoBar);
    if (d->title != value) {
        d->title = value;
        Q_EMIT titleChanged();
    }
}

const QString& QtInfoBar::message() const
{
    Q_D(const QtInfoBar);
    return d->message;
}

void QtInfoBar::setMessage(const QString &value)
{
    Q_D(QtInfoBar);
    if (d->message != value) {
        d->message = value;
        Q_EMIT messageChanged();
    }
}

QtInfoBarType::InfoBarSeverity QtInfoBar::severity() const
{
    Q_D(const QtInfoBar);
    return d->severity;
}

void QtInfoBar::setSeverity(QtInfoBarType::InfoBarSeverity value)
{
    Q_D(QtInfoBar);
    if (d->severity != value) {
        d->severity = value;
        Q_EMIT severityChanged();
    }
}

bool QtInfoBar::isClosable() const
{
    Q_D(const QtInfoBar);
    return d->isClosable;
}

void QtInfoBar::setIsClosable(bool value)
{
    Q_D(QtInfoBar);
    if (d->isClosable != value) {
        d->isClosable = value;
        Q_EMIT isClosableChanged();
    }
}

QtIconType::IconName QtInfoBar::infoBarIcon() const
{
    Q_D(const QtInfoBar);
    return d->infoBarIcon;
}

void QtInfoBar::setInfoBarIcon(QtIconType::IconName value)
{
    Q_D(QtInfoBar);
    if (d->infoBarIcon != value) {
        d->infoBarIcon = value;
        Q_EMIT infoBarIconChanged();
    }
}