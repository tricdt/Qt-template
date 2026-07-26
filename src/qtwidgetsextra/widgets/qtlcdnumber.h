#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlcdnumber.h>
#include <qtmetamacros.h>

class QtLCDNumberPrivate;
class QTWIDGETSEXTRA_EXPORT QtLCDNumber : public QLCDNumber
{
    Q_OBJECT

    Q_PROPERTY(bool isUseAutoClock READ isUseAutoClock WRITE setIsUseAutoClock NOTIFY
            isUseAutoClockChanged)
    Q_PROPERTY(QString autoClockFormat READ autoClockFormat WRITE setAutoClockFormat NOTIFY
            autoClockFormatChanged)
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged)

public:
    explicit QtLCDNumber(QWidget* parent = nullptr);
    explicit QtLCDNumber(uint numDigits, QWidget* parent = nullptr);
    ~QtLCDNumber();

    bool isUseAutoClock() const;
    void setIsUseAutoClock(bool value);

    QString autoClockFormat() const;
    void setAutoClockFormat(QString value);

    bool isTransparent() const;
    void setIsTransparent(bool value);

Q_SIGNALS:
    void isUseAutoClockChanged();
    void autoClockFormatChanged();
    void isTransparentChanged();

    
protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtLCDNumberPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtLCDNumber)
};
