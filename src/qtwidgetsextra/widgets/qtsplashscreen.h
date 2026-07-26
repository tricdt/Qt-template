#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
class QtSplashScreenPrivate;
class QTWIDGETSEXTRA_EXPORT QtSplashScreen : public QWidget
{
    Q_OBJECT
Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool isShowProgressBar READ isShowProgressBar WRITE setIsShowProgressBar NOTIFY isShowProgressBarChanged)
    Q_PROPERTY(bool isShowProgressRing READ isShowProgressRing WRITE setIsShowProgressRing NOTIFY isShowProgressRingChanged)
    Q_PROPERTY(bool isClosable READ isClosable WRITE setIsClosable NOTIFY isClosableChanged)
public:
    explicit QtSplashScreen(QWidget* parent = nullptr);
    ~QtSplashScreen();

    int borderRadius() const;
    void setBorderRadius(int value);

    int minimum() const;
    void setMinimum(int value);

    int maximum() const;
    void setMaximum(int value);

    int value() const;
    void setValue(int value);

    bool isShowProgressBar() const;
    void setIsShowProgressBar(bool value);

    bool isShowProgressRing() const;
    void setIsShowProgressRing(bool value);

    bool isClosable() const;
    void setIsClosable(bool value);

Q_SIGNALS:
    void borderRadiusChanged();
    void minimumChanged();
    void maximumChanged();
    void valueChanged();
    void isShowProgressBarChanged();
    void isShowProgressRingChanged();
    void isClosableChanged();

private:
    QScopedPointer<class QtSplashScreenPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSplashScreen)
};
