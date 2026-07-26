#include "qtcalendarpickercontainer.h"
#include "qtdef.h"
#include "qttheme.h"

class QtCalendarPickerContainerPrivate
{
    Q_DECLARE_PUBLIC(QtCalendarPickerContainer)
public:
    QtCalendarPickerContainer* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtCalendarPickerContainerPrivate(QtCalendarPickerContainer* q);
    ~QtCalendarPickerContainerPrivate();
};

QtCalendarPickerContainerPrivate::QtCalendarPickerContainerPrivate(QtCalendarPickerContainer* q)
    : q_ptr(q)
{
}
QtCalendarPickerContainerPrivate::~QtCalendarPickerContainerPrivate()
{
}

QtCalendarPickerContainer::QtCalendarPickerContainer(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCalendarPickerContainerPrivate(this))
{
    Q_D(QtCalendarPickerContainer);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(0, 0, 0, 0);
    setObjectName("QtCalendarPickerContainer");
    setStyleSheet("#QtCalendarPickerContainer{background-color:transparent}");
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtCalendarPickerContainer::~QtCalendarPickerContainer()
{
}

void QtCalendarPickerContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    qtTheme->drawEffectShadow(&painter, rect(), 6, 5);
    painter.restore();
}
