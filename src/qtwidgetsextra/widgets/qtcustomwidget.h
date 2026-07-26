#pragma once

#include "qtappbar.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qdialog.h>

class QtCustomWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtCustomWidget : public QDialog
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit QtCustomWidget(QWidget* parent = nullptr);
    ~QtCustomWidget();

    void setCentralWidget(QWidget* widget);

protected:
    QVBoxLayout* mainLayout{ nullptr };
    QtAppBar* appBar{ nullptr };
    QWidget* centralWidget{ nullptr };
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtCustomWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCustomWidget)
};
