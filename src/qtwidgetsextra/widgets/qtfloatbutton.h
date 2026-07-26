#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtFloatButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtFloatButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int buttonSize READ buttonSize WRITE setButtonSize NOTIFY buttonSizeChanged)
    Q_PROPERTY(int margin READ margin WRITE setMargin NOTIFY marginChanged)
public:
    explicit QtFloatButton(QWidget* parent = nullptr);
    ~QtFloatButton();

    int buttonSize() const;
    void setButtonSize(int value);

    int margin() const;
    void setMargin(int value);

Q_SIGNALS:
    void buttonSizeChanged();
    void marginChanged();

private:
    QScopedPointer<class QtFloatButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtFloatButton)
};
