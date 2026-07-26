#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qslider.h>

class QtSliderPrivate;
class QTWIDGETSEXTRA_EXPORT QtSlider : public QSlider
{
    Q_OBJECT

public:
    explicit QtSlider(QWidget* parent = nullptr);
    explicit QtSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~QtSlider();


private:
    QScopedPointer<class QtSliderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSlider)
};
