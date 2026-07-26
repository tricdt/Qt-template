#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlineedit.h>
#include <qtmetamacros.h>

class QtPasswordBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtPasswordBox : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(bool isPasswordVisible READ isPasswordVisible WRITE setIsPasswordVisible NOTIFY
            isPasswordVisibleChanged)
public:
    explicit QtPasswordBox(QWidget* parent = nullptr);
    ~QtPasswordBox();

    int borderRadius() const;
    void setBorderRadius(int value);
    bool isPasswordVisible() const;
    void setIsPasswordVisible(bool value);
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);

Q_SIGNALS:
    Q_SIGNAL void focusIn(QString text);
    Q_SIGNAL void focusOut(QString text);
    Q_SIGNAL void wmFocusOut(QString text);
    void borderRadiusChanged();
    void isPasswordVisibleChanged();

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    QScopedPointer<class QtPasswordBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPasswordBox)
};
