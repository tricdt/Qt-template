#pragma once

#include "qtwidgetsextra.h"
#include "widgets/qtappbar.h"
#include <QWidget>
#include <qdialog.h>

class QtContentDialogPrivate;
class QTWIDGETSEXTRA_EXPORT QtContentDialog : public QDialog
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit QtContentDialog(QWidget* parent = nullptr);
    ~QtContentDialog();

    Q_SLOT virtual void onLeftButtonClicked();
    Q_SLOT virtual void onMiddleButtonClicked();
    Q_SLOT virtual void onRightButtonClicked();

    void setCentralWidget(QWidget* centralWidget);

    void setLeftButtonText(QString text);
    void setMiddleButtonText(QString text);
    void setRightButtonText(QString text);

    void setLeftButtonVisible(bool visible);
    void setMiddleButtonVisible(bool visible);
    void setRightButtonVisible(bool visible);

    void close();

Q_SIGNALS:
    Q_SIGNAL void leftButtonClicked();
    Q_SIGNAL void middleButtonClicked();
    Q_SIGNAL void rightButtonClicked();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    QScopedPointer<class QtContentDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtContentDialog)
};
