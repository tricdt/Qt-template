#include "qttransfer.h"

class QtTransferPrivate
{
    Q_DECLARE_PUBLIC(QtTransfer)
public:
int borderRadius;
    int itemHeight;
    QString sourceTitle;
    QString targetTitle;
    bool isSearchVisible;
    QtTransfer* q_ptr;
    QtTransferPrivate(QtTransfer* q);
    ~QtTransferPrivate();
};

QtTransferPrivate::QtTransferPrivate(QtTransfer* q) : q_ptr(q)
{
}
QtTransferPrivate::~QtTransferPrivate()
{
}

QtTransfer::QtTransfer(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtTransferPrivate(this))
{
}

QtTransfer::~QtTransfer()
{
}

int QtTransfer::borderRadius() const
{
    Q_D(const QtTransfer);
    return d->borderRadius;
}

void QtTransfer::setBorderRadius(int value)
{
    Q_D(QtTransfer);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtTransfer::itemHeight() const
{
    Q_D(const QtTransfer);
    return d->itemHeight;
}

void QtTransfer::setItemHeight(int value)
{
    Q_D(QtTransfer);
    if (d->itemHeight != value) {
        d->itemHeight = value;
        Q_EMIT itemHeightChanged();
    }
}

const QString& QtTransfer::sourceTitle() const
{
    Q_D(const QtTransfer);
    return d->sourceTitle;
}

void QtTransfer::setSourceTitle(const QString &value)
{
    Q_D(QtTransfer);
    if (d->sourceTitle != value) {
        d->sourceTitle = value;
        Q_EMIT sourceTitleChanged();
    }
}

const QString& QtTransfer::targetTitle() const
{
    Q_D(const QtTransfer);
    return d->targetTitle;
}

void QtTransfer::setTargetTitle(const QString &value)
{
    Q_D(QtTransfer);
    if (d->targetTitle != value) {
        d->targetTitle = value;
        Q_EMIT targetTitleChanged();
    }
}

bool QtTransfer::isSearchVisible() const
{
    Q_D(const QtTransfer);
    return d->isSearchVisible;
}

void QtTransfer::setIsSearchVisible(bool value)
{
    Q_D(QtTransfer);
    if (d->isSearchVisible != value) {
        d->isSearchVisible = value;
        Q_EMIT isSearchVisibleChanged();
    }
}