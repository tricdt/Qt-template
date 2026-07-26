#pragma once

#include <QWidget>
#include <qtmetamacros.h>
#include <qwidget.h>
#include "qtwidgetsextra.h"
class QtScrollPagePrivate;
class QTWIDGETSEXTRA_EXPORT QtScrollPage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QWidget* customWidget READ customWidget WRITE setCustomWidget NOTIFY customWidgetChanged)
public:
    explicit QtScrollPage(QWidget* parent = nullptr);
    ~QtScrollPage();

    QWidget* customWidget() const;
    void setCustomWidget(QWidget* value);

Q_SIGNALS:
    void customWidgetChanged(QWidget* widget);

private:
    QScopedPointer<class QtScrollPagePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollPage)
};
