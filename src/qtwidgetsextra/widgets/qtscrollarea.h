#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qscrollarea.h>

class QtScrollAreaPrivate;
class QTWIDGETSEXTRA_EXPORT QtScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit QtScrollArea(QWidget* parent = nullptr);
    ~QtScrollArea();

    void setIsGrabGesture(bool isEnable, qreal mousePressEventDelay = 0.5);

    void setIsOverShoot(Qt::Orientation orientation, bool isEnable);
    bool isOverShoot(Qt::Orientation orientation) const;

    void setIsAnimation(Qt::Orientation orientation, bool isAnimation);
    bool isAnimation(Qt::Orientation orientation) const;

private:
    QScopedPointer<class QtScrollAreaPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollArea)
};
