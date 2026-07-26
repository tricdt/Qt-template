#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
class QtNotificationCenterPrivate;
class QTWIDGETSEXTRA_EXPORT QtNotificationCenter : public QWidget
{
    Q_OBJECT
Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int panelWidth READ panelWidth WRITE setPanelWidth NOTIFY panelWidthChanged)
public:
    explicit QtNotificationCenter(QWidget* parent = nullptr);
    ~QtNotificationCenter();

    int borderRadius() const;
    void setBorderRadius(int value);

    int panelWidth() const;
    void setPanelWidth(int value);

Q_SIGNALS:
    void borderRadiusChanged();
    void panelWidthChanged();

private:
    QScopedPointer<class QtNotificationCenterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNotificationCenter)
};
