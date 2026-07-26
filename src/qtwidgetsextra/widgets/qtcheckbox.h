#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qcheckbox.h>

class QtCheckBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtCheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtCheckBox(QWidget* parent = nullptr);
    explicit QtCheckBox(const QString& text, QWidget* parent = nullptr);
    ~QtCheckBox();

    int borderRadius() const;
    void setBorderRadius(int value);

Q_SIGNALS:
    void borderRadiusChanged(int value);

private:
    QScopedPointer<class QtCheckBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCheckBox)
};
