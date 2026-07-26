#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtSelectorBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtSelectorBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtSelectorBar(QWidget* parent = nullptr);
    ~QtSelectorBar();

    int currentIndex() const;
    void setCurrentIndex(int value);

    int borderRadius() const;
    void setBorderRadius(int value);

Q_SIGNALS:
    void currentIndexChanged();
    void borderRadiusChanged();

private:
    QScopedPointer<class QtSelectorBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSelectorBar)
};
