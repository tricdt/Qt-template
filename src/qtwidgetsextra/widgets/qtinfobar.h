#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include "qtdef.h"

class QtInfoBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtInfoBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QtInfoBarType::InfoBarSeverity severity READ severity WRITE setSeverity NOTIFY
            severityChanged)
    Q_PROPERTY(bool isClosable READ isClosable WRITE setIsClosable NOTIFY isClosableChanged)
    Q_PROPERTY(QtIconType::IconName infoBarIcon READ infoBarIcon WRITE setInfoBarIcon NOTIFY
            infoBarIconChanged)
public:
    explicit QtInfoBar(QWidget* parent = nullptr);
    ~QtInfoBar();

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& title() const;
    void setTitle(const QString& value);

    const QString& message() const;
    void setMessage(const QString& value);

    QtInfoBarType::InfoBarSeverity severity() const;
    void setSeverity(QtInfoBarType::InfoBarSeverity value);

    bool isClosable() const;
    void setIsClosable(bool value);

    QtIconType::IconName infoBarIcon() const;
    void setInfoBarIcon(QtIconType::IconName value);

Q_SIGNALS:
    void borderRadiusChanged();
    void titleChanged();
    void messageChanged();
    void severityChanged();
    void isClosableChanged();
    void infoBarIconChanged();

private:
    QScopedPointer<class QtInfoBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtInfoBar)
};
