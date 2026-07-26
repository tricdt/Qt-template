#ifndef QTTOOLBUTTON_H
#define QTTOOLBUTTON_H
#include <QtWidgets>
#include "qtwidgetsextra.h"
#include "qtdef.h"
class QtMenu;
class QtToolButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged)
public:
    explicit QtToolButton(QWidget* parent = nullptr);
    ~QtToolButton();

    int borderRadius() const;
    void setBorderRadius(int newBorderRadius);

    bool isSelected() const;
    void setIsSelected(bool newIsSelected);

    void setIsTransparent(bool isTransparent);
    bool isTransparent() const;

    void setMenu(QtMenu* menu);
    void setQtIcon(QtIconType::IconName icon);
    void setQtIcon(QtIconType::IconName icon, int rotate);

Q_SIGNALS:
    void borderRadiusChanged();
    void isSelectedChanged();

private:
    QScopedPointer<class QtToolButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtToolButton)
};

#endif // QTTOOLBUTTON_H
