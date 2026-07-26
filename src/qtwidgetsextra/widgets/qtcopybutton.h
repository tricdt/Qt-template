#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtCopyButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtCopyButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString copyText READ copyText WRITE setCopyText NOTIFY copyTextChanged)
    Q_PROPERTY(int successDuration READ successDuration WRITE setSuccessDuration NOTIFY
            successDurationChanged)
    Q_PROPERTY(QString successText READ successText WRITE setSuccessText NOTIFY successTextChanged)
public:
    explicit QtCopyButton(QWidget* parent = nullptr);
    ~QtCopyButton();

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& copyText() const;
    void setCopyText(const QString& value);

    int successDuration() const;
    void setSuccessDuration(int value);

    const QString& successText() const;
    void setSuccessText(const QString& value);

Q_SIGNALS:
    void borderRadiusChanged();
    void copyTextChanged();
    void successDurationChanged();
    void successTextChanged();

private:
    QScopedPointer<class QtCopyButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCopyButton)
};
