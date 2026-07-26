#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtAutoCompletePrivate;
class QTWIDGETSEXTRA_EXPORT QtAutoComplete : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems NOTIFY
            maxVisibleItemsChanged)
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity
            NOTIFY caseSensitivityChanged)
public:
    explicit QtAutoComplete(QWidget* parent = nullptr);
    ~QtAutoComplete();

    int borderRadius() const;
    void setBorderRadius(int value);

    int maxVisibleItems() const;
    void setMaxVisibleItems(int value);

    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity value);

Q_SIGNALS:
    void borderRadiusChanged();
    void maxVisibleItemsChanged();
    void caseSensitivityChanged();

private:
    QScopedPointer<class QtAutoCompletePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtAutoComplete)
};
