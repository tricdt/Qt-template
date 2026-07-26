#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
#include "qtdef.h"
class QtPopConfirmPrivate;
class QTWIDGETSEXTRA_EXPORT QtPopConfirm : public QWidget
{
    Q_OBJECT
Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QString confirmButtonText READ confirmButtonText WRITE setConfirmButtonText NOTIFY confirmButtonTextChanged)
    Q_PROPERTY(QString cancelButtonText READ cancelButtonText WRITE setCancelButtonText NOTIFY cancelButtonTextChanged)
    Q_PROPERTY(QtIconType::IconName icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(bool isLightDismiss READ isLightDismiss WRITE setIsLightDismiss NOTIFY isLightDismissChanged)
public:
    explicit QtPopConfirm(QWidget* parent = nullptr);
    ~QtPopConfirm();

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& title() const;
    void setTitle(const QString &value);

    const QString& content() const;
    void setContent(const QString &value);

    const QString& confirmButtonText() const;
    void setConfirmButtonText(const QString &value);

    const QString& cancelButtonText() const;
    void setCancelButtonText(const QString &value);

    QtIconType::IconName icon() const;
    void setIcon(QtIconType::IconName value);

    bool isLightDismiss() const;
    void setIsLightDismiss(bool value);

Q_SIGNALS:
    void borderRadiusChanged();
    void titleChanged();
    void contentChanged();
    void confirmButtonTextChanged();
    void cancelButtonTextChanged();
    void iconChanged();
    void isLightDismissChanged();

private:
    QScopedPointer<class QtPopConfirmPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPopConfirm)
};
