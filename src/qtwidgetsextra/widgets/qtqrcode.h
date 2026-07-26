#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtQRCodePrivate;
class QTWIDGETSEXTRA_EXPORT QtQRCode : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int quietZone READ quietZone WRITE setQuietZone NOTIFY quietZoneChanged)
    Q_PROPERTY(QColor foregroundColor READ foregroundColor WRITE setForegroundColor NOTIFY
            foregroundColorChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
            backgroundColorChanged)
public:
    explicit QtQRCode(QWidget* parent = nullptr);
    ~QtQRCode();

    int borderRadius() const;
    void setBorderRadius(int value);

    int quietZone() const;
    void setQuietZone(int value);

    QColor foregroundColor() const;
    void setForegroundColor(QColor value);

    QColor backgroundColor() const;
    void setBackgroundColor(QColor value);

Q_SIGNALS:
    void borderRadiusChanged();
    void quietZoneChanged();
    void foregroundColorChanged();
    void backgroundColorChanged();

private:
    QScopedPointer<class QtQRCodePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtQRCode)
};
