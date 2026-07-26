#pragma once

#include "qtroller.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtRollerPickerContainerPrivate;
class QTWIDGETSEXTRA_EXPORT QtRollerPickerContainer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int animationPixOffsetY READ animationPixOffsetY WRITE setAnimationPixOffsetY NOTIFY
            animationPixOffsetYChanged)
public:
    explicit QtRollerPickerContainer(QWidget* parent = nullptr);
    ~QtRollerPickerContainer();

    void doPickerAnimation();

    int animationPixOffsetY() const;
    void setAnimationPixOffsetY(int value);

    QList<QtRoller*>& rollerList();

    int buttonAreaHeight() const;
    void setButtonAreaHeight(int value);

Q_SIGNALS:
    Q_SIGNAL void overButtonClicked();
    Q_SIGNAL void cancelButtonClicked();

    void animationPixOffsetYChanged();

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtRollerPickerContainerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRollerPickerContainer)
};
