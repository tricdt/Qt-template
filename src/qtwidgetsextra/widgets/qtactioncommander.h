#pragma once

#include "qtdef.h"
#include "qtsingleton.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qobject.h>

class QTWIDGETSEXTRA_EXPORT QtActionCommand : public QObject
{
    Q_OBJECT

public:
    explicit QtActionCommand(QObject* parent = nullptr);
    ~QtActionCommand() override;

    virtual void undo() = 0;
    virtual void redo() = 0;
};

class QtActionCommanderPrivate;
class QTWIDGETSEXTRA_EXPORT QtActionCommander : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(QtActionCommander)
    Q_PROPERTY(
        int maxRouteCount READ maxRouteCount WRITE setMaxRouteCount NOTIFY maxRouteCountChanged)
public:
    void recordCommand(const QString& domainName, QtActionCommand* command, bool isRedo = true);
    void clearCommand(const QString& domainName);
    void undoCommand(const QString& domainName);
    void redoCommand(const QString& domainName);

    QtActionCommanderType::CommanderState commanderUndoState(const QString& domainName) const;
    QtActionCommanderType::CommanderState commanderRedoState(const QString& domainName) const;

    int maxRouteCount() const;
    void setMaxRouteCount(int value);

Q_SIGNALS:
    void maxRouteCountChanged();
    void commanderStateChanged(
        const QString& domainName, QtActionCommanderType::CommanderState state);

private:
    explicit QtActionCommander(QObject* parent = nullptr);
    ~QtActionCommander();

    QScopedPointer<class QtActionCommanderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtActionCommander)
};
