#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include "qtdef.h"
class QtDropDownButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtDropDownButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QtIconType::IconName qtIcon READ qtIcon WRITE setQtIcon NOTIFY qtIconChanged)
public:
    explicit QtDropDownButton(QWidget* parent = nullptr);
    ~QtDropDownButton();

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& text() const;
    void setText(const QString& value);

    QtIconType::IconName qtIcon() const;
    void setQtIcon(QtIconType::IconName value);

Q_SIGNALS:
    void borderRadiusChanged();
    void textChanged();
    void qtIconChanged();

private:
    QScopedPointer<class QtDropDownButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDropDownButton)
};
