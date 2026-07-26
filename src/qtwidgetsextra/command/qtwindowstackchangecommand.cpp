#include "qtwindowstackchangecommand.h"
#include "widgets/qtwindow.h"
#include "widgets/qtwindow_p.h"
class QtWindowStackChangeCommandPrivate
{
    Q_DECLARE_PUBLIC(QtWindowStackChangeCommand)
public:
    QtWindowStackChangeCommand* q_ptr;
    int undoStackIndex;
    int redoStackIndex;
    QtWindowPrivate* windowPrivate;
    QtWindowStackChangeCommandPrivate(QtWindowStackChangeCommand* q);
    ~QtWindowStackChangeCommandPrivate();
};

QtWindowStackChangeCommandPrivate::QtWindowStackChangeCommandPrivate(QtWindowStackChangeCommand* q)
    : q_ptr(q)
{
}
QtWindowStackChangeCommandPrivate::~QtWindowStackChangeCommandPrivate()
{
}

QtWindowStackChangeCommand::QtWindowStackChangeCommand(QObject* parent)
    : QtActionCommand{ parent }
    , d_ptr(new QtWindowStackChangeCommandPrivate(this))
{
}

QtWindowStackChangeCommand::~QtWindowStackChangeCommand()
{
}

int QtWindowStackChangeCommand::undoStackIndex() const
{
    Q_D(const QtWindowStackChangeCommand);
    return d->undoStackIndex;
}
void QtWindowStackChangeCommand::setUndoStackIndex(int value){
    Q_D(QtWindowStackChangeCommand);
    d->undoStackIndex = value;
}

int QtWindowStackChangeCommand::redoStackIndex() const
{
    Q_D(const QtWindowStackChangeCommand);
    return d->redoStackIndex;
}
void QtWindowStackChangeCommand::setRedoStackIndex(int value){
    Q_D(QtWindowStackChangeCommand);
    d->redoStackIndex = value;
}

QtWindowPrivate* QtWindowStackChangeCommand::windowPrivate() const
{
    Q_D(const QtWindowStackChangeCommand);
    return d->windowPrivate;
}
void QtWindowStackChangeCommand::setWindowPrivate(QtWindowPrivate* value){
    Q_D(QtWindowStackChangeCommand);
    d->windowPrivate = value;
}

void QtWindowStackChangeCommand::undo()
{
    Q_D(QtWindowStackChangeCommand);
    d->windowPrivate->centerStackedWidget->doWindowStackSwitch(
        d->windowPrivate->stackSwitchMode, d->undoStackIndex, false);
    Q_EMIT d->windowPrivate->q_ptr->currentStackIndexChanged();
}

void QtWindowStackChangeCommand::redo()
{
    Q_D(QtWindowStackChangeCommand);
    d->windowPrivate->centerStackedWidget->doWindowStackSwitch(
        d->windowPrivate->stackSwitchMode, d->redoStackIndex, false);
    Q_EMIT d->windowPrivate->q_ptr->currentStackIndexChanged();
}