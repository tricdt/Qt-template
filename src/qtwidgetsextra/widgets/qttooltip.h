#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtToolTipPrivate;
class QTWIDGETSEXTRA_EXPORT QtToolTip : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int displayMsec READ displayMsec WRITE setDisplayMsec NOTIFY displayMsecChanged)
    Q_PROPERTY(
        int showDelayMsec READ showDelayMsec WRITE setShowDelayMsec NOTIFY showDelayMsecChanged)
    Q_PROPERTY(
        int hideDelayMsec READ hideDelayMsec WRITE setHideDelayMsec NOTIFY hideDelayMsecChanged)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
    Q_PROPERTY(
        QWidget* customWidget READ customWidget WRITE setCustomWidget NOTIFY customWidgetChanged)
public:
    explicit QtToolTip(QWidget* parent = nullptr);
    ~QtToolTip();

    void updatePos();

    int borderRadius() const;
    void setBorderRadius(int value);

    int displayMsec() const;
    void setDisplayMsec(int value);

    int showDelayMsec() const;
    void setShowDelayMsec(int value);

    int hideDelayMsec() const;
    void setHideDelayMsec(int value);

    QString toolTip() const;
    void setToolTip(const QString& value);

    QWidget* customWidget() const;
    void setCustomWidget(QWidget* value);

Q_SIGNALS:
    void borderRadiusChanged();
    void displayMsecChanged();
    void showDelayMsecChanged();
    void hideDelayMsecChanged();
    void toolTipChanged();
    void customWidgetChanged();

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    QScopedPointer<class QtToolTipPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtToolTip)
};
