#pragma once

#include <QObject>
#include <QtWidgets/qwidget.h>
#include "testlib.h"
class TestWidgetPrivate;
class QTTESTLIB_EXPORT TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget* parent = nullptr);
    ~TestWidget();

private:
    QScopedPointer<class TestWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(TestWidget)
};
