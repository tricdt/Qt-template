#pragma once

#include "qtwidgetsextra.h"
#include "widgets/qtactioncommander.h"
#include "widgets/qtwindow.h"
#include <QWidget>

class QtWindowStackChangeCommandPrivate;
class QTWIDGETSEXTRA_EXPORT QtWindowStackChangeCommand : public QtActionCommand
{
    Q_OBJECT
public:
    explicit QtWindowStackChangeCommand(QObject* parent = nullptr);
    ~QtWindowStackChangeCommand();

    int undoStackIndex() const;
    void setUndoStackIndex(int value);

    int redoStackIndex() const;
    void setRedoStackIndex(int value);

    QtWindowPrivate* windowPrivate() const;
    void setWindowPrivate(QtWindowPrivate* value);

    void undo() override;
    void redo() override;

private:
    QScopedPointer<class QtWindowStackChangeCommandPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWindowStackChangeCommand)
};
