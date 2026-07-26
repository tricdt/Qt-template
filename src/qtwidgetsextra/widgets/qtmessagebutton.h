#pragma once

#include "qtdef.h"

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpushbutton.h>
class QtMessageButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtMessageButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString barTitle READ barTitle WRITE setBarTitle NOTIFY barTitleChanged)
    Q_PROPERTY(QString barText READ barText WRITE setBarText NOTIFY barTextChanged)
    Q_PROPERTY(int displayMsec READ displayMsec WRITE setDisplayMsec NOTIFY displayMsecChanged)
    Q_PROPERTY(QWidget* messageTargetWidget READ messageTargetWidget WRITE setMessageTargetWidget
            NOTIFY messageTargetWidgetChanged)
    Q_PROPERTY(QtMessageBarType::MessageMode messageMode READ messageMode WRITE setMessageMode
            NOTIFY messageModeChanged)
    Q_PROPERTY(QtMessageBarType::PositionPolicy positionPolicy READ positionPolicy WRITE
            setPositionPolicy NOTIFY positionPolicyChanged)
public:
    explicit QtMessageButton(QWidget* parent = nullptr);
    explicit QtMessageButton(QString text, QWidget* parent = nullptr);
    ~QtMessageButton();

    int borderRadius() const;
    void setBorderRadius(int value);

    QString barTitle() const;
    void setBarTitle(const QString& value);

    QString barText() const;
    void setBarText(const QString& value);

    int displayMsec() const;
    void setDisplayMsec(int value);

    QWidget* messageTargetWidget() const;
    void setMessageTargetWidget(QWidget* value);

    QtMessageBarType::MessageMode messageMode() const;
    void setMessageMode(QtMessageBarType::MessageMode value);

    QtMessageBarType::PositionPolicy positionPolicy() const;
    void setPositionPolicy(QtMessageBarType::PositionPolicy value);

Q_SIGNALS:
    void borderRadiusChanged(int value);
    void barTitleChanged(const QString& value);
    void barTextChanged(const QString& value);
    void displayMsecChanged(int value);
    void messageTargetWidgetChanged(QWidget* value);
    void messageModeChanged(QtMessageBarType::MessageMode value);
    void positionPolicyChanged(QtMessageBarType::PositionPolicy value);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtMessageButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMessageButton)
};
