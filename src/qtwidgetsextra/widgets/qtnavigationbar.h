#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtNavigationBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged)
    Q_PROPERTY(bool isAllowPageOpenInNewWindow READ isAllowPageOpenInNewWindow WRITE
            setIsAllowPageOpenInNewWindow NOTIFY isAllowPageOpenInNewWindowChanged)
    Q_PROPERTY(bool navigationBarWidth READ navigationBarWidth WRITE
            setNavigationBarWidth NOTIFY navigationBarWidthChanged)
public:
    explicit QtNavigationBar(QWidget* parent = nullptr);
    ~QtNavigationBar();

    bool isTransparent() const;
    void setIsTransparent(bool value);

    bool isAllowPageOpenInNewWindow() const;
    void setIsAllowPageOpenInNewWindow(bool value);

    int navigationBarWidth() const;
    void setNavigationBarWidth(int value);

Q_SIGNALS:
    void isTransparentChanged();
    void isAllowPageOpenInNewWindowChanged();
    void navigationBarWidthChanged();
        

private:
    QScopedPointer<class QtNavigationBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationBar)
};
