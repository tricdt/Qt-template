#pragma once

#include "qtwidgetsextra.h"
#include "widgets/qtactioncommander.h"
#include "widgets/qtscrollpage.h"
#include <QWidget>
#include <qcontainerfwd.h>

class QtScrollPageRouteCommandPrivate;
class QTWIDGETSEXTRA_EXPORT QtScrollPageRouteCommand : public QtActionCommand
{
    Q_OBJECT

public:
    explicit QtScrollPageRouteCommand(QWidget* parent = nullptr);
    ~QtScrollPageRouteCommand();

    bool isBreadcrumbClicked() const;
    void setIsBreadcrumbClicked(bool);

    QtScrollPagePrivate* qtScrollPagePrivate() const;
    void setQtScrollPagePrivate(QtScrollPagePrivate*);

    const int& undoPageIndex() const;
    void setUndoPageIndex(int&);

    const int& redoPageIndex() const;
    void setRedoPageIndex(int&);

    const QStringList& undoBreadcrumbList() const;
    void setUndoBreadcrumbList(const QStringList&);

    const QStringList& redoBreadcrumbList() const;
    void setRedoBreadcrumbList(const QStringList&);

private:
    QScopedPointer<class QtScrollPageRouteCommandPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollPageRouteCommand)

    void undo() override;
    void redo() override;
};
