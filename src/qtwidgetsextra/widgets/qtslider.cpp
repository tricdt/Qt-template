#include "qtslider.h"
#include "styles/qtsliderstyle.h"

class QtSliderPrivate
{
    Q_DECLARE_PUBLIC(QtSlider)
public:
    QtSlider* q_ptr;
    QtSliderPrivate(QtSlider* q);
    ~QtSliderPrivate();
};

QtSliderPrivate::QtSliderPrivate(QtSlider* q)
    : q_ptr(q)
{
}
QtSliderPrivate::~QtSliderPrivate()
{
}

QtSlider::QtSlider(QWidget* parent)
    : QSlider{ parent }
    , d_ptr(new QtSliderPrivate(this))
{
    setOrientation(Qt::Horizontal);
    setStyle(new QtSliderStyle(style()));
}

QtSlider::QtSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
}

QtSlider::~QtSlider()
{
    delete this->style();
}