#include "qtactioncommander.h"
#include "qtdef.h"
QtActionCommand::QtActionCommand(QObject* parent)
    : QObject(parent)
{
}

QtActionCommand::~QtActionCommand()
{
}

class QtActionCommanderPrivate
{
    Q_DECLARE_PUBLIC(QtActionCommander)
public:
    int maxRouteCount;
    struct QtActionCommandDomainData
    {
        int currentIndex{ -1 };
        QtActionCommanderType::CommanderState undoState{ QtActionCommanderType::UndoInvalid };
        QtActionCommanderType::CommanderState redoState{ QtActionCommanderType::RedoInvalid };
        QVector<QtActionCommand*> commandList;
    };
    QtActionCommander* q_ptr;
    QMap<QString, QtActionCommandDomainData> commandDomainMap;
    QtActionCommanderPrivate(QtActionCommander* q);
    ~QtActionCommanderPrivate();
};

QtActionCommanderPrivate::QtActionCommanderPrivate(QtActionCommander* q)
    : q_ptr(q)
{
}
QtActionCommanderPrivate::~QtActionCommanderPrivate()
{
}

Q_SINGLETON_CREATE_CPP(QtActionCommander)
QtActionCommander::QtActionCommander(QObject* parent)
    : QObject{ parent }
    , d_ptr(new QtActionCommanderPrivate(this))
{
    Q_D(QtActionCommander);
    d->maxRouteCount = 500;
}

QtActionCommander::~QtActionCommander()
{
}

void QtActionCommander::recordCommand(
    const QString& domainName, QtActionCommand* command, bool isRedo)
{
    Q_D(QtActionCommander);
    if(!command)
    {
        return;
    }
    command->setParent(this);
    auto& commandData = d->commandDomainMap[domainName];
    auto& commandList = commandData.commandList;
    if(commandData.currentIndex <= 0 || commandList.count() == 0)
    {
        commandData.undoState = QtActionCommanderType::UndoValid;
        commandData.redoState = QtActionCommanderType::RedoInvalid;
        Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::UndoValid);
        Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::RedoInvalid);
    }
    else
    {
        if(commandList.count() >= d->maxRouteCount)
        {
            commandList.first()->deleteLater();
            commandList.removeFirst();
            commandData.currentIndex -= 1;
        }
    }
    if(commandData.currentIndex != commandList.count() - 1)
    {
        for(int i = commandData.currentIndex + 1;
            i < commandList.count() - commandData.currentIndex - 1; i++)
        {
            commandList.at(i)->deleteLater();
        }
        commandList.remove(
            commandData.currentIndex + 1, commandList.count() - commandData.currentIndex - 1);
        if(commandData.currentIndex > 0)
        {
            commandData.redoState = QtActionCommanderType::RedoInvalid;
            Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::RedoInvalid);
        }
    }
    commandList.append(command);
    commandData.currentIndex = commandList.count() - 1;
    if(isRedo)
    {
        command->redo();
    }
}
void QtActionCommander::clearCommand(const QString& domainName)
{
    Q_D(QtActionCommander);
    if(!d->commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->commandDomainMap[domainName];
    auto& commandList = commandData.commandList;
    commandData.currentIndex = -1;
    for(const auto command : commandList)
    {
        command->deleteLater();
    }
    commandList.clear();
    commandData.undoState = QtActionCommanderType::UndoInvalid;
    commandData.redoState = QtActionCommanderType::RedoInvalid;
    Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::UndoInvalid);
    Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::RedoInvalid);
}
void QtActionCommander::undoCommand(const QString& domainName)
{
    Q_D(QtActionCommander);
    if(!d->commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->commandDomainMap[domainName];
    if(commandData.undoState == QtActionCommanderType::UndoInvalid)
    {
        return;
    }
    auto& commandList = commandData.commandList;
    if(commandList.isEmpty())
    {
        return;
    }
    if(commandData.currentIndex == 0)
    {
        commandData.undoState = QtActionCommanderType::UndoInvalid;
        Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::UndoInvalid);
    }
    if(commandData.currentIndex == commandList.size() - 1)
    {
        commandData.redoState = QtActionCommanderType::RedoValid;
        Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::RedoValid);
    }
    auto command = commandList[commandData.currentIndex];
    command->undo();
    commandData.currentIndex -= 1;
}
void QtActionCommander::redoCommand(const QString& domainName)
{
    Q_D(QtActionCommander);
    if(!d->commandDomainMap.contains(domainName))
    {
        return;
    }
    auto& commandData = d->commandDomainMap[domainName];
    if(commandData.redoState == QtActionCommanderType::RedoInvalid)
    {
        return;
    }
    auto& commandList = commandData.commandList;
    if(commandList.isEmpty())
    {
        return;
    }
    if(commandData.currentIndex <= 0)
    {
        commandData.undoState = QtActionCommanderType::UndoValid;
        Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::UndoValid);
    }
    if(commandData.currentIndex == commandList.size() - 2)
    {
        commandData.redoState = QtActionCommanderType::RedoInvalid;
        Q_EMIT commanderStateChanged(domainName, QtActionCommanderType::RedoInvalid);
    }
    if(commandData.currentIndex < commandList.size() - 1)
    {
        commandData.currentIndex += 1;
    }
    auto command = commandList[commandData.currentIndex];
    command->redo();
}

QtActionCommanderType::CommanderState QtActionCommander::commanderUndoState(
    const QString& domainName) const
{
    Q_D(const QtActionCommander);
    if(!d->commandDomainMap.contains(domainName))
    {
        return QtActionCommanderType::UndoInvalid;
    }
    return d->commandDomainMap[domainName].undoState;
}
QtActionCommanderType::CommanderState QtActionCommander::commanderRedoState(
    const QString& domainName) const
{
    Q_D(const QtActionCommander);
    if(!d->commandDomainMap.contains(domainName))
    {
        return QtActionCommanderType::RedoInvalid;
    }
    return d->commandDomainMap[domainName].redoState;
}

int QtActionCommander::maxRouteCount() const
{
    Q_D(const QtActionCommander);
    return d->maxRouteCount;
}
void QtActionCommander::setMaxRouteCount(int value)
{
    Q_D(QtActionCommander);
    d->maxRouteCount = value;
}