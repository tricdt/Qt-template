#pragma once

#include "qtwidgetsextra.h"
#include "widgets/qtactioncommander.h"
#include "widgets/qtnavigationbar.h"
#include <QWidget>

class QtNavigationRouteCommandPrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationRouteCommand : public QtActionCommand
{
    Q_OBJECT

public:
    explicit QtNavigationRouteCommand(QObject* parent = nullptr);
    ~QtNavigationRouteCommand();

    void undo() override;
    void redo() override;

    QtNavigationBar* navigationBar() const;
    void setNavigationBar(QtNavigationBar* value);

    const QString& undoPageKey() const;
    void setUndoPageKey(const QString& value);

    const QString& redoPageKey() const;
    void setRedoPageKey(const QString& value);

private:
    QScopedPointer<class QtNavigationRouteCommandPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationRouteCommand)
};
