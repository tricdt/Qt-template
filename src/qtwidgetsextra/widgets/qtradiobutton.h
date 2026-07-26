#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qradiobutton.h>

class QtRadioButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtRadioButton : public QRadioButton
{
    Q_OBJECT

public:
    explicit QtRadioButton(QWidget* parent = nullptr);
    explicit QtRadioButton(const QString& text, QWidget* parent = nullptr);
    ~QtRadioButton();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtRadioButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRadioButton)
};
