#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtToggleSwitchPrivate;
class QTWIDGETSEXTRA_EXPORT QtToggleSwitch : public QWidget
{
    Q_OBJECT

public:
    explicit QtToggleSwitch(QWidget* parent = nullptr);
    ~QtToggleSwitch();

    void setIsToggled(bool isToggled);
    bool getIsToggled() const;

Q_SIGNALS:
    void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtToggleSwitchPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtToggleSwitch)
};
