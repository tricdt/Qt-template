#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpushbutton.h>
#include <qtmetamacros.h>

class QtRollerPickerPrivate;
class QTWIDGETSEXTRA_EXPORT QtRollerPicker : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtRollerPicker(QWidget* parent = nullptr);
    ~QtRollerPicker();

    void addRoller(const QStringList& itemList, bool isEnableLoop = true);
    void removeRoller(int index);

    void setRollerItemList(int index, const QStringList& itemList);
    void setRollerWidth(int index, int width);

    void setCurrentData(int index, const QString& data);
    void setCurrentData(const QStringList& dataList);
    QString currentData(int index) const;
    QStringList currentData() const;

    void setCurrentIndex(int rollerIndex, int index);
    void setCurrentIndex(const QList<int>& indexList);
    int currentIndex(int rollerIndex) const;
    QList<int> currentIndex() const;

    int borderRadius() const;
    void setBorderRadius(int value);

Q_SIGNALS:
    void borderRadiusChanged();
    Q_SIGNAL void currentDataChanged(const QStringList& dataList);
    Q_SIGNAL void currentDataSelectionChanged(const QStringList& dataList);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtRollerPickerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRollerPicker)
};
