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
#include "widgets/qttoolbutton.h"
#include <qabstractitemmodel.h>
#include <qdatetime.h>
#include <qobject.h>
#include <qscrollbar.h>
#include <qvariant.h>
#include <qvariantanimation.h>

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
    if(!isSwitchAnimationFinished)
    {
        return;
    }
    Q_Q(QtCalendar);
    QtCalendarType displayMode = calendarModel->displayMode();
    if(displayMode == QtCalendarType::DayMode)
    {
        oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
            q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
        calendarModel->setDisplayMode(QtCalendarType::MonthMode);
        calendarTitleView->setVisible(false);
        QApplication::processEvents();
        calendarView->setSpacing(15);
        scrollToDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
        newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
            q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
        doSwitchAnimation(false);
    }
    else if(displayMode == QtCalendarType::MonthMode)
    {
        oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
            q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
        calendarModel->setDisplayMode(QtCalendarType::YearMode);
        scrollToDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
        newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
            q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
        doSwitchAnimation(false);
    }
}
void QtCalendarPrivate::onCalendarViewClicked(const QModelIndex& index)
{
    Q_Q(QtCalendar);
    if(!isSwitchAnimationFinished)
    {
        return;
    }
    switch(calendarModel->displayMode())
    {
        case YearMode: {
            oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
                q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
            lastSelectedYear = calendarModel->minimumDate().year() + index.row();
            calendarModel->setDisplayMode(QtCalendarType::MonthMode);
            scrollToDate(QDate(lastSelectedYear, lastSelectedMonth, 15));
            newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
                q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
            doSwitchAnimation(true);
            break;
        }
        case MonthMode: {
            oldCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
                q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
            lastSelectedMonth = index.row() % 12 + 1;
            calendarModel->setDisplayMode(QtCalendarType::DayMode);
            calendarTitleView->setVisible(true);
            calendarView->setSpacing(0);
            scrollToDate(QDate(lastSelectedYear, lastSelectedMonth, 15));
            calendarView->selectionModel()->setCurrentIndex(
                calendarModel->indexFromDate(selectedDate), QItemSelectionModel::Select);
            newCalendarViewPix = q->grab(QRect(borderWidth, borderWidth + 45,
                q->width() - 2 * borderWidth, q->height() - 2 * borderWidth - 45));
            calendarTitleView->setVisible(false);
            doSwitchAnimation(true);
            break;
        }
        case DayMode: {
            QDate date = calendarModel->dateFromIndex(index);
            if(date.isValid())
            {
                calendarView->clearSelection();
                q->setSelectedDate(date);
            }
            Q_EMIT q->clicked(selectedDate);
            break;
        }
    }
}
void QtCalendarPrivate::onUpButtonClicked()
{
    Q_Q(QtCalendar);
    QScrollBar* vscrollBar = calendarView->verticalScrollBar();
    QPropertyAnimation* scrollAnimation = new QPropertyAnimation(vscrollBar, "value");
    scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
    scrollAnimation->setDuration(300);
    scrollAnimation->setStartValue(vscrollBar->value());
    scrollAnimation->setEndValue(vscrollBar->value() - 150);
    scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtCalendarPrivate::onDownButtonClicked()
{
    Q_Q(QtCalendar);
    QScrollBar* vscrollBar = calendarView->verticalScrollBar();
    QPropertyAnimation* scrollAnimation = new QPropertyAnimation(vscrollBar, "value");
    scrollAnimation->setEasingCurve(QEasingCurve::OutSine);
    scrollAnimation->setDuration(300);
    scrollAnimation->setStartValue(vscrollBar->value());
    scrollAnimation->setEndValue(vscrollBar->value() + 150);
    scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
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
    if(!isSwitchAnimationFinished)
    {
        return;
    }
    isDrawNewPix = false;
    isSwitchAnimationFinished = false;
    calendarDelegate->setIsTransparent(true);
    QVariantAnimation* oldPixZoomAnimation = new QVariantAnimation(q);
    QObject::connect(
        oldPixZoomAnimation, &QVariantAnimation::valueChanged, q, [this](const QVariant& value) {
            zoomRatio = value.toReal();
            q_ptr->update();
        });
    QObject::connect(oldPixZoomAnimation, &QVariantAnimation::finished, q, [this, q, isZoomIn]() {
        isDrawNewPix = true;
        QVariantAnimation* newPixZoomAnimation = new QVariantAnimation(q);
        QObject::connect(newPixZoomAnimation, &QVariantAnimation::valueChanged, q,
            [this](const QVariant& value) {
                zoomRatio = value.toReal();
                q_ptr->update();
            });
        QObject::connect(newPixZoomAnimation, &QVariantAnimation::finished, q, [this]() {
            if(calendarModel->displayMode() == QtCalendarType::DayMode)
            {
                calendarTitleView->setVisible(true);
            }
            isSwitchAnimationFinished = true;
            calendarDelegate->setIsTransparent(false);
        });
        if(isZoomIn)
        {
            newPixZoomAnimation->setStartValue(0.85);
            newPixZoomAnimation->setEndValue(1.0);
        }
        else
        {
            newPixZoomAnimation->setStartValue(1.5);
            newPixZoomAnimation->setEndValue(1.0);
        }
        newPixZoomAnimation->setDuration(300);
        newPixZoomAnimation->setEasingCurve(QEasingCurve::OutCubic);
        newPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    if(isZoomIn)
    {
        oldPixZoomAnimation->setStartValue(1.0);
        oldPixZoomAnimation->setEndValue(1.15);
    }
    else
    {
        oldPixZoomAnimation->setStartValue(1.0);
        oldPixZoomAnimation->setEndValue(0.85);
    }
    oldPixZoomAnimation->setDuration(150);
    oldPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QVariantAnimation* oldPixOpacityAnimation = new QVariantAnimation(q);
    QObject::connect(oldPixZoomAnimation, &QVariantAnimation::valueChanged, q,
        [this](const QVariant& value) { pixOpacity = value.toReal(); });
    QObject::connect(oldPixZoomAnimation, &QVariantAnimation::finished, q, [this, q]() {
        QVariantAnimation* newPixOpacityAnimation = new QVariantAnimation(q);
        newPixOpacityAnimation->setStartValue(0.0);
        newPixOpacityAnimation->setEndValue(1.0);
        newPixOpacityAnimation->setDuration(300);
        newPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
    oldPixOpacityAnimation->setStartValue(1.0);
    oldPixOpacityAnimation->setEndValue(0.0);
    oldPixOpacityAnimation->setDuration(150);
    oldPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtCalendarPrivate::updateSwitchButtonText()
{
    Q_Q(QtCalendar);
    QModelIndex modelIndex = calendarView->indexAt(
        QPoint(calendarView->rect().center().x() - 20, calendarView->rect().center().y()));
    if(!modelIndex.isValid())
    {
        return;
    }
    QtCalendarData data = calendarModel->data(modelIndex, Qt::UserRole).value<QtCalendarData>();
    switch(calendarModel->displayMode())
    {
        case YearMode: {
            modeSwitchButton->setText(QString("%1-%2").arg(data.year - 5).arg(data.year + 10));
            break;
        }
        case MonthMode: {
            modeSwitchButton->setText(QString("%1").arg(data.year));
            break;
        }
        case DayMode: {
            modeSwitchButton->setText(QString("%1 / %2").arg(data.year).arg(data.month));
            break;
        }
    }
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
    connect(d->calendarView, &QtBaseListView::clicked, this,
        [this](const QModelIndex& index) { d_ptr->onCalendarViewClicked(index); });

    d->modeSwitchButton = new QtToolButton(this);
    d->modeSwitchButton->setText("1924年1月");
    QFont switchButtonFont = d->modeSwitchButton->font();
    switchButtonFont.setWeight(QFont::Bold);
    d->modeSwitchButton->setFont(switchButtonFont);
    d->modeSwitchButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    d->modeSwitchButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(d->modeSwitchButton, &QtToolButton::clicked, this,
        [this]() { d_ptr->onSwitchButtonClicked(); });
    connect(d->calendarView->verticalScrollBar(), &QScrollBar::valueChanged, this,
        [this]() { d_ptr->updateSwitchButtonText(); });

    d->upButton = new QtToolButton(this);
    d->upButton->setFixedSize(36, 36);
    d->upButton->setQtIcon(QtIconType::CaretUp);
    connect(d->upButton, &QtToolButton::clicked, this, [this]() { d_ptr->onUpButtonClicked(); });

    d->downButton = new QtToolButton(this);
    d->downButton->setFixedSize(36, 36);
    d->downButton->setQtIcon(QtIconType::CaretDown);
    connect(
        d->downButton, &QtToolButton::clicked, this, [this]() { d_ptr->onDownButtonClicked(); });

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
    if (!selectedDate.isValid() || selectedDate.daysTo(d->calendarModel->maximumDate()) < 0
    || selectedDate.daysTo(d->calendarModel->minimumDate()) > 0)
    {
        return;
    }
    d->selectedDate = selectedDate;
    d->calendarView->selectionModel()->setCurrentIndex(d->calendarModel->indexFromDate(selectedDate),
    QItemSelectionModel::Select);
    Q_EMIT selectedDateChanged();
}
QDate QtCalendar::minimumDate() const
{
    Q_D(const QtCalendar);
    return d->calendarModel->minimumDate();
}
void QtCalendar::setMinimumDate(QDate minimudate)
{
    Q_D(QtCalendar);
    if (!minimudate.isValid() || minimudate.daysTo(d->calendarModel->maximumDate()) < 0)
    {
        return;
    }
    d->calendarModel->setMinimumDate(minimudate);
    Q_EMIT minimumDateChanged();
}
QDate QtCalendar::maximumDate() const
{
    Q_D(const QtCalendar);
    return d->calendarModel->maximumDate();
}
void QtCalendar::setMaximumDate(QDate maximumDate)
{
    Q_D(QtCalendar);
    if (!maximumDate.isValid() || maximumDate.daysTo(d->calendarModel->minimumDate()) > 0)
    {
        return;
    }
    d->calendarModel->setMaximumDate(maximumDate);
    Q_EMIT maximumDateChanged();
}
