#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlineedit.h>
#include <qtmetamacros.h>

class QtLineEditPrivate;
class QTWIDGETSEXTRA_EXPORT QtLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged);
    Q_PROPERTY(bool isClearButtonEnable READ isClearButtonEnable WRITE setIsClearButtonEnable NOTIFY
            isClearButtonEnableChanged);

public:
    explicit QtLineEdit(QWidget* parent = nullptr);
    ~QtLineEdit();

    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);

    int borderRadius() const;
    void setBorderRadius(int value);

    bool isClearButtonEnable() const;
    void setIsClearButtonEnable(int value);
Q_SIGNALS:
    void borderRadiusChanged();
    void isClearButtonEnableChanged();
    void focusIn(QString text);
    void focusOut(QString text);
    void wmFocusOut(QString text);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    QScopedPointer<class QtLineEditPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLineEdit)
};
