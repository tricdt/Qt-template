#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtTreeSelectPrivate;
class QTWIDGETSEXTRA_EXPORT QtTreeSelect : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems NOTIFY
            maxVisibleItemsChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText NOTIFY
            placeholderTextChanged)
    Q_PROPERTY(bool isSearchVisible READ isSearchVisible WRITE setIsSearchVisible NOTIFY
            isSearchVisibleChanged)
    Q_PROPERTY(bool isEditable READ isEditable WRITE setIsEditable NOTIFY isEditableChanged)
public:
    explicit QtTreeSelect(QWidget* parent = nullptr);
    ~QtTreeSelect();

    int borderRadius() const;
    void setBorderRadius(int value);

    int itemHeight() const;
    void setItemHeight(int value);

    int maxVisibleItems() const;
    void setMaxVisibleItems(int value);

    const QString& placeholderText() const;
    void setPlaceholderText(const QString& value);

    bool isSearchVisible() const;
    void setIsSearchVisible(bool value);

    bool isEditable() const;
    void setIsEditable(bool value);

Q_SIGNALS:
    void borderRadiusChanged();
    void itemHeightChanged();
    void maxVisibleItemsChanged();
    void placeholderTextChanged();
    void isSearchVisibleChanged();
    void isEditableChanged();

private:
    QScopedPointer<class QtTreeSelectPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTreeSelect)
};
