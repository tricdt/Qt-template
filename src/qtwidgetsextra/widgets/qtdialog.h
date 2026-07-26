#pragma once

#include <QWidget>
#include <qdialog.h>
#include "qtwidgetsextra.h"
class QtDialogPrivate;
class QTWIDGETSEXTRA_EXPORT QtDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QtDialog(QWidget* parent = nullptr);
    ~QtDialog();

private:
    QScopedPointer<class QtDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDialog)
};
