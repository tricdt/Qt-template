#include "qtscrollpage.h"

class QtScrollPagePrivate
{
    Q_DECLARE_PUBLIC(QtScrollPage)
public:
    QWidget* customWidget;
    QtScrollPage* q_ptr;
    QtScrollPagePrivate(QtScrollPage* q);
    ~QtScrollPagePrivate();
};

QtScrollPagePrivate::QtScrollPagePrivate(QtScrollPage* q)
    : q_ptr(q)
{
}
QtScrollPagePrivate::~QtScrollPagePrivate()
{
}

QtScrollPage::QtScrollPage(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtScrollPagePrivate(this))
{
}

QtScrollPage::~QtScrollPage()
{
}

QWidget* QtScrollPage::customWidget() const
{
    Q_D(const QtScrollPage);
    return d->customWidget;
}
void QtScrollPage::setCustomWidget(QWidget* value)
{
    Q_D(QtScrollPage);
    d->customWidget = value;
    Q_EMIT customWidgetChanged(d->customWidget);
}