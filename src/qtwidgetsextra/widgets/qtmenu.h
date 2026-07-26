#pragma once

#include <QMenu>
#include <QObject>
#include "qtdef.h"
#include "qtwidgetsextra.h"

class QtMenuPrivate;
class QTWIDGETSEXTRA_EXPORT QtMenu : public QMenu
{
    Q_OBJECT
    Q_DISABLE_COPY(QtMenu)

public:
    explicit QtMenu(QWidget* parent = nullptr);
    explicit QtMenu(const QString& title, QWidget* parent = nullptr);
    ~QtMenu();

    void setMenuItemHeight(int menuItemHeight);
    int getMenuItemHeight() const;

    QAction* addMenu(QMenu* menu);
    QtMenu* addMenu(const QString& title);
    QtMenu* addMenu(const QIcon& icon, const QString& title);
    QtMenu* addMenu(QtIconType::IconName icon, const QString& title);
    QAction* addQtIconAction(QtIconType::IconName icon, const QString& text);
    QAction* addQtIconAction(
        QtIconType::IconName icon, const QString& text, const QKeySequence& shortcut);

    bool isHasChildMenu() const;
    bool isHasIcon() const;
Q_SIGNALS:
    void menuShow();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtMenuPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMenu);
};
