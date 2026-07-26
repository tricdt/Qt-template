#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtFlyoutPrivate;
class QTWIDGETSEXTRA_EXPORT QtFlyout : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(bool isLightDismiss READ isLightDismiss WRITE setIsLightDismiss NOTIFY
            isLightDismissChanged)
public:
    explicit QtFlyout(QWidget* parent = nullptr);
    ~QtFlyout();

    void setContentWidget(QWidget* widget);
    void showFlyout(QWidget* target);
    void closeFlyout();

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& title() const;
    void setTitle(const QString& value);

    const QString& content() const;
    void setContent(const QString& value);

    bool isLightDismiss() const;
    void setIsLightDismiss(bool value);

Q_SIGNALS:
    void closed();
    void borderRadiusChanged();
    void titleChanged();
    void contentChanged();
    void isLightDismissChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
private:
    QScopedPointer<class QtFlyoutPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFlyout)
};
