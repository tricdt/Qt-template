#include "qtscrollpageroutecommand.h"
#include "widgets/qtscrollpage_p.h"
class QtScrollPageRouteCommandPrivate
{
    Q_DECLARE_PUBLIC(QtScrollPageRouteCommand)
public:
    bool isBreadcrumbClicked;
    QtScrollPagePrivate* scrollPagePrivate;
    int undoPageIndex;
    int redoPageIndex;
    QStringList undoBreadcrumbList;
    QStringList redoBreadcrumbList;
    QtScrollPageRouteCommand* q_ptr;

    QtScrollPageRouteCommandPrivate(QtScrollPageRouteCommand* q);
    ~QtScrollPageRouteCommandPrivate();
};

QtScrollPageRouteCommandPrivate::QtScrollPageRouteCommandPrivate(QtScrollPageRouteCommand* q)
    : q_ptr(q)
{
}
QtScrollPageRouteCommandPrivate::~QtScrollPageRouteCommandPrivate()
{
}

QtScrollPageRouteCommand::QtScrollPageRouteCommand(QWidget* parent)
    : QtActionCommand{ parent }
    , d_ptr(new QtScrollPageRouteCommandPrivate(this))
{
}

QtScrollPageRouteCommand::~QtScrollPageRouteCommand()
{
}

bool QtScrollPageRouteCommand::isBreadcrumbClicked() const
{
    Q_D(const QtScrollPageRouteCommand);
    return d->isBreadcrumbClicked;
}
void QtScrollPageRouteCommand::setIsBreadcrumbClicked(bool value)
{
    Q_D(QtScrollPageRouteCommand);
    d->isBreadcrumbClicked = value;
}

QtScrollPagePrivate* QtScrollPageRouteCommand::qtScrollPagePrivate() const
{
    Q_D(const QtScrollPageRouteCommand);
    return d->scrollPagePrivate;
}
void QtScrollPageRouteCommand::setQtScrollPagePrivate(QtScrollPagePrivate* value)
{
    Q_D(QtScrollPageRouteCommand);
    d->scrollPagePrivate = value;
}

const int& QtScrollPageRouteCommand::undoPageIndex() const
{
    Q_D(const QtScrollPageRouteCommand);
    return d->undoPageIndex;
}
void QtScrollPageRouteCommand::setUndoPageIndex(int& value)
{
    Q_D(QtScrollPageRouteCommand);
    d->undoPageIndex = value;
}

const int& QtScrollPageRouteCommand::redoPageIndex() const
{
    Q_D(const QtScrollPageRouteCommand);
    return d->redoPageIndex;
}
void QtScrollPageRouteCommand::setRedoPageIndex(int& value)
{
    Q_D(QtScrollPageRouteCommand);
    d->redoPageIndex = value;
}

const QStringList& QtScrollPageRouteCommand::undoBreadcrumbList() const
{
    Q_D(const QtScrollPageRouteCommand);
    return d->undoBreadcrumbList;
}
void QtScrollPageRouteCommand::setUndoBreadcrumbList(const QStringList& value)
{
    Q_D(QtScrollPageRouteCommand);
    d->undoBreadcrumbList = value;
}

const QStringList& QtScrollPageRouteCommand::redoBreadcrumbList() const
{
    Q_D(const QtScrollPageRouteCommand);
    return d->redoBreadcrumbList;
}
void QtScrollPageRouteCommand::setRedoBreadcrumbList(const QStringList& value)
{
    Q_D(QtScrollPageRouteCommand);
    d->redoBreadcrumbList = value;
}
void QtScrollPageRouteCommand::undo()
{
    Q_D(QtScrollPageRouteCommand);
    if(d->isBreadcrumbClicked)
    {
        int widgetIndex =
            d->scrollPagePrivate->centralWidgetMap.value(d->undoBreadcrumbList.last());
        d->scrollPagePrivate->switchCentralStackIndex(
            widgetIndex, d->scrollPagePrivate->navigationTargetIndex);
        d->scrollPagePrivate->navigationTargetIndex = widgetIndex;
        d->scrollPagePrivate->breadcrumbBar->setBreadcrumbList(d->undoBreadcrumbList);
    }
    else
    {
        d->scrollPagePrivate->q_ptr->navigation(d->undoPageIndex, false);
    }
}

void QtScrollPageRouteCommand::redo()
{
    Q_D(QtScrollPageRouteCommand);
    if(d->isBreadcrumbClicked)
    {
        int widgetIndex =
            d->scrollPagePrivate->centralWidgetMap.value(d->redoBreadcrumbList.last());
        d->scrollPagePrivate->switchCentralStackIndex(
            widgetIndex, d->scrollPagePrivate->navigationTargetIndex);
        d->scrollPagePrivate->navigationTargetIndex = widgetIndex;
        d->scrollPagePrivate->breadcrumbBar->setBreadcrumbList(d->redoBreadcrumbList);
    }
    else
    {
        d->scrollPagePrivate->q_ptr->navigation(d->redoPageIndex, false);
    }
}