#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qplaintextedit.h>
#include <qtmetamacros.h>

class QtPlainTextEditPrivate;
class QTWIDGETSEXTRA_EXPORT QtPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit QtPlainTextEdit(QWidget* parent = nullptr);
    explicit QtPlainTextEdit(const QString& text, QWidget* parent = nullptr);
    ~QtPlainTextEdit();

protected:
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    QScopedPointer<class QtPlainTextEditPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPlainTextEdit)
};
