#pragma once

#include "qcombobox.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtComboBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtComboBox(QWidget* parent = nullptr);
    ~QtComboBox();

    int borderRadius() const;
    void setBorderRadius(int value);
    void setEditable(bool editable);
Q_SIGNALS:
    void borderRadiusChanged(int value);

protected:
    virtual void showPopup() override;
    virtual void hidePopup() override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtComboBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtComboBox)
};
