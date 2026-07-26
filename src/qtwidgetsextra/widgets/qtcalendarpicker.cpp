#include "qtcalendarpicker.h"
#include "itemviews/views/qtbaselistview.h"
#include "qtcalendarpickercontainer.h"
#include "qtdef.h"
#include "qttheme.h"
#include "widgets/qtcalendar.h"
#include "widgets/qttoolbutton.h"
#include <qboxlayout.h>
#include <qdatetime.h>
#include <qobject.h>
#include <qpainter.h>
#include <qvariant.h>
#include <qvariantanimation.h>

class QtCalendarPickerPrivate
{
    Q_DECLARE_PUBLIC(QtCalendarPicker)
public:
    QtCalendarPicker* q_ptr;
    int borderRadius;

    QtThemeType::ThemeMode themeMode;
    QtCalendar* calendar{ nullptr };
    QtCalendarPickerContainer* calendarPickerContainer{ nullptr };
   
    QtCalendarPickerPrivate(QtCalendarPicker* q);
    ~QtCalendarPickerPrivate();
    void onCalendarPickerClicked();
    void onCalendarSelectedDateChanged();

};

QtCalendarPickerPrivate::QtCalendarPickerPrivate(QtCalendarPicker* q)
    : q_ptr(q)
{
}
QtCalendarPickerPrivate::~QtCalendarPickerPrivate()
{
}

void QtCalendarPickerPrivate::onCalendarPickerClicked()
{
    Q_Q(QtCalendarPicker);
    QPoint endPoint(q->mapToGlobal(
        QPoint((q->width() - calendarPickerContainer->width()) / 2, q->height() + 5)));
    QVariantAnimation* showAnimation = new QVariantAnimation(q);
    QObject::connect(showAnimation, &QVariantAnimation::valueChanged, q,
        [this](const QVariant& value) { calendarPickerContainer->move(value.toPoint()); });
    showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    showAnimation->setDuration(250);
    showAnimation->setStartValue(QPoint(endPoint.x(), endPoint.y() - 10));
    showAnimation->setEndValue(endPoint);
    calendarPickerContainer->show();
    showAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtCalendarPickerPrivate::onCalendarSelectedDateChanged()
{
    Q_Q(QtCalendarPicker);
}


QtCalendarPicker::QtCalendarPicker(QWidget* parent)
    : QPushButton{ parent }
    , d_ptr(new QtCalendarPickerPrivate(this))
{
    Q_D(QtCalendarPicker);
    d->borderRadius = 3;
    setFixedSize(120, 30);
    setObjectName("QtCalendarPicker");
    setMouseTracking(true);
    d->calendarPickerContainer = new QtCalendarPickerContainer(this);
    d->calendarPickerContainer->resize(317, 352);
    d->calendar = new QtCalendar(d->calendarPickerContainer);
    QVBoxLayout* containerLayout = new QVBoxLayout(d->calendarPickerContainer);
    containerLayout->setContentsMargins(6, 6, 6, 6);
    containerLayout->addWidget(d->calendar);
    d->calendarPickerContainer->hide();
    connect(this, &QPushButton::clicked, this, [this]() { d_ptr->onCalendarPickerClicked(); });

    connect(d->calendar, &QtCalendar::selectedDateChanged, this,
        [this]() { d_ptr->onCalendarSelectedDateChanged(); });
    setSelectedDate(QDate::currentDate());

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtCalendarPicker::~QtCalendarPicker()
{
}

int QtCalendarPicker::borderRadius() const
{
    Q_D(const QtCalendarPicker);
    return d->borderRadius;
}
void QtCalendarPicker::setBorderRadius(int value)
{
    Q_D(QtCalendarPicker);
    d->borderRadius = value;
}
QDate QtCalendarPicker::selectedDate() const
{
    Q_D(const QtCalendarPicker);
    return d->calendar->selectedDate();
}
void QtCalendarPicker::setSelectedDate(QDate value)
{
    Q_D(QtCalendarPicker);
    d->calendar->setSelectedDate(value);
    Q_EMIT selectedDateChanged(value);
}

void QtCalendarPicker::paintEvent(QPaintEvent* event)
{
    Q_D(QtCalendarPicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRect baseRect = rect();
    baseRect.adjust(1, 1, -1, -1);
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                  : QtThemeColor(d->themeMode, BasicBase));
    painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);

    QDate selectedDate = this->selectedDate();
    QString date = QString("%1/%2/%3")
                       .arg(selectedDate.year())
                       .arg(selectedDate.month())
                       .arg(selectedDate.day());
    painter.setPen(QtThemeColor(d->themeMode, BasicText));
    QRect textRect = baseRect;
    textRect.adjust(10, 0, 0, 0);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, date);

    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(17);
    painter.setFont(iconFont);
    painter.drawText(QRect(baseRect.right() - 25, 0, 15, height()),
        Qt::AlignVCenter | Qt::AlignRight, QChar(static_cast<ushort>(QtIconType::CalendarRange)));
    painter.restore();
}