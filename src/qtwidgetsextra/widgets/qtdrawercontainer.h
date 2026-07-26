#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtDrawerContainerPrivate;
class QTWIDGETSEXTRA_EXPORT QtDrawerContainer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    explicit QtDrawerContainer(QWidget* parent = nullptr);
    ~QtDrawerContainer();

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);

    void doDrawerAnimation(bool isExpand);

    int borderRadius() const;
    void setBorderRadius(int value);

    QPixmap containerPix() const;
    void setContainerPix(QPixmap value);

    qreal opacity() const;
    void setOpacity(qreal value);

Q_SIGNALS:
    void opacityChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtDrawerContainerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDrawerContainer)
};
