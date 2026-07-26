#pragma once
#include "qttheme.h"
#include "qtwidgetsextra.h"
#include "widgets/qtmenu.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtSplitButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtSplitButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QtIconType::IconName qtIcon READ qtIcon WRITE setQtIcon NOTIFY qtIconChanged)

public:
    explicit QtSplitButton(QWidget* parent = nullptr);
    ~QtSplitButton();

    void setMenu(QtMenu* menu);
    QtMenu* menu() const;

    int borderRadius() const;
    void setBorderRadius(int value);
    QString text() const;
    void setText(QString value);
    QtIconType::IconName qtIcon() const;
    void setQtIcon(QtIconType::IconName value);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;
Q_SIGNALS:
    void clicked();
    void borderRadiusChanged();
    void textChanged();
    void qtIconChanged();

private:
    QScopedPointer<class QtSplitButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSplitButton)
};
