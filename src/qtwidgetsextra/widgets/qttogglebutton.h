#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtToggleButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtToggleButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged NOTIFY textChanged)
public:
    explicit QtToggleButton(QWidget* parent = nullptr);
    explicit QtToggleButton(QString text, QWidget* parent = nullptr);
    ~QtToggleButton();

    int borderRadius() const;
    void setBorderRadius(int radius);

    QString text() const;
    void setText(const QString& text);

    void setIsToggled(bool isToggled);
    bool isToggled() const;

Q_SIGNALS:
    void borderRadiusChanged(int radius);
    void textChanged(QString text);
    void toggled(bool checked);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtToggleButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtToggleButton)
};
