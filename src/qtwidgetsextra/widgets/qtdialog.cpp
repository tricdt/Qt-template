#include "qtdialog.h"
#include <qnamespace.h>

class QtDialogPrivate
{
    Q_DECLARE_PUBLIC(QtDialog)
public:
    QtDialog* q_ptr;
    QtDialogPrivate(QtDialog* q);
    ~QtDialogPrivate();
};

QtDialogPrivate::QtDialogPrivate(QtDialog* q)
    : q_ptr(q)
{
}
QtDialogPrivate::~QtDialogPrivate()
{
}

QtDialog::QtDialog(QWidget* parent)
    : QDialog{ parent }
    , d_ptr(new QtDialogPrivate(this))
{
    Q_D(QtDialog);
    d->q_ptr = this;
    resize(500, 500); // 默认宽高
    setWindowTitle("QtDialog");
    setObjectName("QtDialog");
#ifndef Q_OS_WIN
    setAttribute(Qt::WA_Hover);
#endif

    setAttribute(Qt::WA_DeleteOnClose);
}

QtDialog::~QtDialog()
{
}