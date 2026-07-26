#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtProgressRingPrivate;
class QTWIDGETSEXTRA_EXPORT QtProgressRing : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isBusying READ isBusying WRITE setIsBusying NOTIFY isBusyingChanged);
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged);
    Q_PROPERTY(
        bool isDisplayValue READ isDisplayValue WRITE setDisplayValue NOTIFY isDisplayValueChanged);
    Q_PROPERTY(QtProgressRingType::ValueDisplayMode valueDisplayMode READ valueDisplayMode WRITE
            setValueDisplayMode NOTIFY valueDisplayModeChanged);
    Q_PROPERTY(int busyingWidth READ busyingWidth WRITE setBusyingWidth NOTIFY busyingWidthChanged);
    Q_PROPERTY(int busyingDurationTime READ busyingDurationTime WRITE setBusyingDurationTime NOTIFY
            busyingDurationTimeChanged);
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged);
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged);
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged);
    Q_PROPERTY(int valuePixelSize READ valuePixelSize WRITE setValuePixelSize NOTIFY
            valuePixelSizeChanged);

public:
    explicit QtProgressRing(QWidget* parent = nullptr);
    ~QtProgressRing();

    bool isBusying() const;
    void setIsBusying(bool value);

    bool isTransparent() const;
    void setIsTransparent(bool value);

    bool isDisplayValue() const;
    void setDisplayValue(bool value);

    int busyingWidth() const;
    void setBusyingWidth(int value);

    QtProgressRingType::ValueDisplayMode valueDisplayMode() const;
    void setValueDisplayMode(QtProgressRingType::ValueDisplayMode value);

    int busyingDurationTime() const;
    void setBusyingDurationTime(int value);

    int minimum() const;
    void setMinimum(int value);

    int maximum() const;
    void setMaximum(int value);

    int value() const;
    void setValue(int value);

    int valuePixelSize() const;
    void setValuePixelSize(int value);

    void setRange(int min, int max);

Q_SIGNALS:
    void isBusyingChanged(bool value);
    void isTransparentChanged(bool value);
    void isDisplayValueChanged(bool value);
    void valueDisplayModeChanged(QtProgressRingType::ValueDisplayMode value);
    void busyingWidthChanged(int value);
    void busyingDurationTimeChanged(int value);
    void minimumChanged(int value);
    void maximumChanged(int value);
    void valueChanged(int value);
    void valuePixelSizeChanged(int value);
    void rangeChanged(int min, int max);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtProgressRingPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtProgressRing)
};
