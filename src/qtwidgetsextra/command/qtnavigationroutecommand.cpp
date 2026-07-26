#include "qtnavigationroutecommand.h"

class QtNavigationRouteCommandPrivate
{
    Q_DECLARE_PUBLIC(QtNavigationRouteCommand)
public:
    QtNavigationBar* navigationBar;
    QString undoPageKey;
    QString redoPageKey;
    QtNavigationRouteCommand* q_ptr;
    QtNavigationRouteCommandPrivate(QtNavigationRouteCommand* q);
    ~QtNavigationRouteCommandPrivate();
};

QtNavigationRouteCommandPrivate::QtNavigationRouteCommandPrivate(QtNavigationRouteCommand* q)
    : q_ptr(q)
{
}
QtNavigationRouteCommandPrivate::~QtNavigationRouteCommandPrivate()
{
}

QtNavigationRouteCommand::QtNavigationRouteCommand(QObject* parent)
    : QtActionCommand{ parent }
    , d_ptr(new QtNavigationRouteCommandPrivate(this))
{
}

QtNavigationRouteCommand::~QtNavigationRouteCommand()
{
}

void QtNavigationRouteCommand::undo()
{
    Q_D(QtNavigationRouteCommand);
    d->navigationBar->navigation(d->undoPageKey, false, false);
}
void QtNavigationRouteCommand::redo()
{
    Q_D(QtNavigationRouteCommand);
    d->navigationBar->navigation(d->redoPageKey, false, false);
}

QtNavigationBar* QtNavigationRouteCommand::navigationBar() const
{
    Q_D(const QtNavigationRouteCommand);
    return d->navigationBar;
}
void QtNavigationRouteCommand::setNavigationBar(QtNavigationBar* value)
{
    Q_D(QtNavigationRouteCommand);
    d->navigationBar = value;
}

const QString& QtNavigationRouteCommand::undoPageKey() const
{
    Q_D(const QtNavigationRouteCommand);
    return d->undoPageKey;
}
void QtNavigationRouteCommand::setUndoPageKey(const QString& value)
{
    Q_D(QtNavigationRouteCommand);
    d->undoPageKey = value;
}

const QString& QtNavigationRouteCommand::redoPageKey() const
{
    Q_D(const QtNavigationRouteCommand);
    return d->redoPageKey;
}
void QtNavigationRouteCommand::setRedoPageKey(const QString& value)
{
    Q_D(QtNavigationRouteCommand);
    d->redoPageKey = value;
}