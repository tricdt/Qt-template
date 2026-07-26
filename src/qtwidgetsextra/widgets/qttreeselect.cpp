#include "qttreeselect.h"

class QtTreeSelectPrivate
{
    Q_DECLARE_PUBLIC(QtTreeSelect)
public:
int borderRadius;
    int itemHeight;
    int maxVisibleItems;
    QString placeholderText;
    bool isSearchVisible;
    bool isEditable;
    QtTreeSelect* q_ptr;
    QtTreeSelectPrivate(QtTreeSelect* q);
    ~QtTreeSelectPrivate();
};

QtTreeSelectPrivate::QtTreeSelectPrivate(QtTreeSelect* q) : q_ptr(q)
{
}
QtTreeSelectPrivate::~QtTreeSelectPrivate()
{
}

QtTreeSelect::QtTreeSelect(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtTreeSelectPrivate(this))
{
}

QtTreeSelect::~QtTreeSelect()
{
}

int QtTreeSelect::borderRadius() const
{
    Q_D(const QtTreeSelect);
    return d->borderRadius;
}

void QtTreeSelect::setBorderRadius(int value)
{
    Q_D(QtTreeSelect);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtTreeSelect::itemHeight() const
{
    Q_D(const QtTreeSelect);
    return d->itemHeight;
}

void QtTreeSelect::setItemHeight(int value)
{
    Q_D(QtTreeSelect);
    if (d->itemHeight != value) {
        d->itemHeight = value;
        Q_EMIT itemHeightChanged();
    }
}

int QtTreeSelect::maxVisibleItems() const
{
    Q_D(const QtTreeSelect);
    return d->maxVisibleItems;
}

void QtTreeSelect::setMaxVisibleItems(int value)
{
    Q_D(QtTreeSelect);
    if (d->maxVisibleItems != value) {
        d->maxVisibleItems = value;
        Q_EMIT maxVisibleItemsChanged();
    }
}

const QString& QtTreeSelect::placeholderText() const
{
    Q_D(const QtTreeSelect);
    return d->placeholderText;
}

void QtTreeSelect::setPlaceholderText(const QString &value)
{
    Q_D(QtTreeSelect);
    if (d->placeholderText != value) {
        d->placeholderText = value;
        Q_EMIT placeholderTextChanged();
    }
}

bool QtTreeSelect::isSearchVisible() const
{
    Q_D(const QtTreeSelect);
    return d->isSearchVisible;
}

void QtTreeSelect::setIsSearchVisible(bool value)
{
    Q_D(QtTreeSelect);
    if (d->isSearchVisible != value) {
        d->isSearchVisible = value;
        Q_EMIT isSearchVisibleChanged();
    }
}

bool QtTreeSelect::isEditable() const
{
    Q_D(const QtTreeSelect);
    return d->isEditable;
}

void QtTreeSelect::setIsEditable(bool value)
{
    Q_D(QtTreeSelect);
    if (d->isEditable != value) {
        d->isEditable = value;
        Q_EMIT isEditableChanged();
    }
}