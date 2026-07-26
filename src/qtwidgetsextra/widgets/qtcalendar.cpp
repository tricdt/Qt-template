#include "qtcalendar.h"
#include "itemviews/delegates/qtcalendardelegate.h"
#include "itemviews/delegates/qtcalendartitledelegate.h"
#include "itemviews/models/qtcalendarmodel.h"
#include "itemviews/models/qtcalendartitlemodel.h"
#include "itemviews/views/qtbaselistview.h"
#include "qtdef.h"
#include "qttheme.h"
#include "qttoolbutton.h"
#include "widgets/qtscrollbar.h"
#include <qdatetime.h>

class QtCalendarPrivate
{
    Q_DECLARE_PUBLIC(QtCalendar)
public:
    int borderRadius;
    QDate selectedDate;
    qreal zoomRatio;
    qreal pixOpacity;
    QtCalendar* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QPixmap oldCalendarViewPix;
    QPixmap newCalendarViewPix;
    int lastSelectedYear{ 0 };
    int lastSelectedMonth{ 1 };
    qreal borderWidth{ 1.5 };
    QtBaseListView* calendarView{ nullptr };
    QtCalendarModel* calendarModel{ nullptr };
    QtCalendarDelegate* calendarDelegate{ nullptr };
    QtBaseListView* calendarTitleView{ nullptr };
    QtToolButton* modeSwitchButton{ nullptr };
    QtToolButton* upButton{ nullptr };
    QtToolButton* downButton{ nullptr };
    bool isSwitchAnimationFinished{ true };
    bool isDrawNewPix{ false };
    QtCalendarPrivate(QtCalendar* q);
    ~QtCalendarPrivate();
    void onSwitchButtonClicked();
    void onCalendarViewClicked(const QModelIndex& index);
    void onUpButtonClicked();
    void onDownButtonClicked();
    void scrollToDate(QDate date);
    void doSwitchAnimation(bool isZoomIn);
    void updateSwitchButtonText();
};

QtCalendarPrivate::QtCalendarPrivate(QtCalendar* q)
    : q_ptr(q)
{
}
QtCalendarPrivate::~QtCalendarPrivate()
{
}

void QtCalendarPrivate::onSwitchButtonClicked()
{
    Q_Q(QtCalendar);
}
void QtCalendarPrivate::onCalendarViewClicked(const QModelIndex& index)
{
    Q_Q(QtCalendar);
}
void QtCalendarPrivate::onUpButtonClicked()
{
    Q_Q(QtCalendar);
}
void QtCalendarPrivate::onDownButtonClicked()
{
    Q_Q(QtCalendar);
}
void QtCalendarPrivate::scrollToDate(QDate date)
{
    Q_Q(QtCalendar);
    int index = calendarModel->indexFromDate(date).row();
    switch(calendarModel->displayMode())
    {
        case YearMode:
        case MonthMode: {
            calendarView->scrollTo(
                calendarModel->index(index - 4), QAbstractItemView::PositionAtTop);
            break;
        }
        case DayMode: {
            calendarView->scrollTo(
                calendarModel->index(index - 14), QAbstractItemView::PositionAtTop);
            break;
        }
    }
}
void QtCalendarPrivate::doSwitchAnimation(bool isZoomIn)
{
    Q_Q(QtCalendar);
}
void QtCalendarPrivate::updateSwitchButtonText()
{
    Q_Q(QtCalendar);
}

QtCalendar::QtCalendar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCalendarPrivate(this))
{
    Q_D(QtCalendar);
    setFixedSize(305, 340);
    setObjectName("QtCalendar");
    d->q_ptr = this;
    d->borderRadius = 5;

    d->calendarTitleView = new QtBaseListView(this);
    d->calendarTitleView->setFlow(QListView::LeftToRight);
    d->calendarTitleView->setViewMode(QListView::IconMode);
    d->calendarTitleView->setResizeMode(QListView::Adjust);
    d->calendarTitleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->calendarTitleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->calendarTitleView->setModel(new QtCalendarTitleModel(this));
    d->calendarTitleView->setItemDelegate(new QtCalendarTitleDelegate(this));
    d->calendarTitleView->setFixedHeight(30);

    d->calendarView = new QtBaseListView(this);
    d->calendarView->setFlow(QListView::LeftToRight);
    d->calendarView->setViewMode(QListView::IconMode);
    d->calendarView->setResizeMode(QListView::Adjust);
    QtScrollBar* vScrollBar = dynamic_cast<QtScrollBar*>(d->calendarView->verticalScrollBar());
    vScrollBar->setSpeedLimit(6);
    d->calendarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->calendarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->calendarView->setSelectionMode(QAbstractItemView::NoSelection);
    d->calendarModel = new QtCalendarModel(this);
    d->calendarView->setModel(d->calendarModel);
    d->calendarDelegate = new QtCalendarDelegate(d->calendarModel, this);
    d->calendarView->setItemDelegate(d->calendarDelegate);
    // connect(d->_calendarView, &ElaBaseListView::clicked, d,
    // &ElaCalendarPrivate::onCalendarViewClicked);

    d->modeSwitchButton = new QtToolButton(this);
    d->modeSwitchButton->setText("1924年1月");
    QFont switchButtonFont = d->modeSwitchButton->font();
    switchButtonFont.setWeight(QFont::Bold);
    d->modeSwitchButton->setFont(switchButtonFont);
    d->modeSwitchButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    d->modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    // connect(d->modeSwitchButton, &QtToolButton::clicked, d,
    //     &ElaCalendarPrivate::onSwitchButtonClicked);
    // connect(d->calendarView->verticalScrollBar(), &QScrollBar::valueChanged, d,
    //     [=]() { d->updateSwitchButtonText(); });

    d->upButton = new QtToolButton(this);
    d->upButton->setFixedSize(36, 36);
    d->upButton->setQtIcon(QtIconType::CaretUp);
    // connect(d->upButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onUpButtonClicked);

    d->downButton = new QtToolButton(this);
    d->downButton->setFixedSize(36, 36);
    d->downButton->setQtIcon(QtIconType::CaretDown);
    // connect(d->downButton, &ElaToolButton::clicked, d, &ElaCalendarPrivate::onDownButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(5, 5, 10, 0);
    buttonLayout->addWidget(d->modeSwitchButton);
    buttonLayout->addWidget(d->upButton);
    buttonLayout->addWidget(d->downButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 0, 0, 0);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(d->calendarTitleView);
    mainLayout->addWidget(d->calendarView);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
    setVisible(true);
    QDate currentDate = QDate::currentDate();
    d->lastSelectedYear = currentDate.year();
    d->lastSelectedMonth = currentDate.month();
    d->scrollToDate(currentDate);
}

QtCalendar::~QtCalendar()
{
}

void QtCalendar::paintEvent(QPaintEvent* event)
{
    Q_D(QtCalendar);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QRect baseRect = rect();
    baseRect.adjust(d->borderWidth, d->borderWidth, -d->borderWidth, -d->borderWidth);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, DialogBase));
    painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);
    if(!d->isSwitchAnimationFinished)
    {
        painter.save();
        QRect pixRect =
            QRect(baseRect.x(), d->borderWidth + 45, baseRect.width(), baseRect.height() - 45);
        painter.setOpacity(d->pixOpacity);
        painter.translate(pixRect.center());
        painter.scale(d->zoomRatio, d->zoomRatio);
        painter.translate(-pixRect.center());
        painter.drawPixmap(
            pixRect, d->isDrawNewPix ? d->newCalendarViewPix : d->oldCalendarViewPix);
        painter.restore();
    }
    painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), d->borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);
    painter.drawLine(QPointF(baseRect.x(), 45), QPointF(baseRect.right(), 45));
    painter.restore();
}

int QtCalendar::borderRadius() const
{
    Q_D(const QtCalendar);
    return d->borderRadius;
}
void QtCalendar::setBorderRadius(int value)
{
    Q_D(QtCalendar);
    d->borderRadius = value;
}
QDate QtCalendar::selectedDate() const
{
    Q_D(const QtCalendar);
    return d->selectedDate;
}
void QtCalendar::setSelectedDate(QDate selectedDate)
{
    Q_D(QtCalendar);
    // if (!selectedDate.isValid() || selectedDate.daysTo(d->_calendarModel->getMaximumDate()) < 0
    // || selectedDate.daysTo(d->_calendarModel->getMinimumDate()) > 0)
    // {
    //     return;
    // }
    // d->selectedDate = selectedDate;
    // d->_calendarView->selectionModel()->setCurrentIndex(d->_calendarModel->getIndexFromDate(selectedDate),
    // QItemSelectionModel::Select);
    Q_EMIT selectedDateChanged();
}
QDate QtCalendar::minimumDate() const
{
    Q_D(const QtCalendar);
    return d->calendarModel->minimumDate();
}
void QtCalendar::setMinimumDate(QDate value)
{
    Q_D(QtCalendar);
    d->calendarModel->setMinimumDate(value);
}
QDate QtCalendar::maximumDate() const
{
    Q_D(const QtCalendar);
    return d->calendarModel->maximumDate();
}
void QtCalendar::setMaximumDate(QDate value)
{
    Q_D(QtCalendar);
    d->calendarModel->setMaximumDate(value);
}
