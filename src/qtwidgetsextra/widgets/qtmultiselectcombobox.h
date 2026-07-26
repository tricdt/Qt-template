#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qcombobox.h>
class QtMultiSelectComboBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtMultiSelectComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtMultiSelectComboBox(QWidget* parent = nullptr);
    ~QtMultiSelectComboBox();

    int borderRadius() const;
    void setBorderRadius(int radius);

    void setCurrentSelection(QString selection);
    void setCurrentSelection(QStringList selection);
    void setCurrentSelection(int index);
    void setCurrentSelection(QList<int> selectionIndex);
    QStringList currentSelection() const;
    QList<int> currentSelectionIndex() const;

Q_SIGNALS:
    void borderRadiusChanged(int radius);
    void itemSelectionChanged(QVector<bool> itemSelection);
    void currentTextListChanged(QStringList selectedTextList);

protected:
    virtual void paintEvent(QPaintEvent* e) override;
    virtual void showPopup() override;
    virtual void hidePopup() override;

private:
    QScopedPointer<class QtMultiSelectComboBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMultiSelectComboBox)
};
