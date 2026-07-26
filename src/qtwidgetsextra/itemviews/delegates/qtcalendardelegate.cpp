#include "qtcalendardelegate.h"
#include "qttheme.h"
#include <qdatetime.h>
class QtCalendarDelegatePrivate
{
    Q_DECLARE_PUBLIC(QtCalendarDelegate)
public:
    QtCalendarDelegate* q_ptr;
    QtCalendarModel* calendarModel{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QDate nowDate;
    int itemHeight;
    int itemWidth;
    bool isTransparent;
    QtCalendarDelegatePrivate(QtCalendarDelegate* q);
    ~QtCalendarDelegatePrivate();
    void onCalendarModelDisplayModeChanged();
    void drawYearOrMonth(
        QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void drawDays(
        QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

QtCalendarDelegatePrivate::QtCalendarDelegatePrivate(QtCalendarDelegate* q)
    : q_ptr(q)
{
}
QtCalendarDelegatePrivate::~QtCalendarDelegatePrivate()
{
}

void QtCalendarDelegatePrivate::drawYearOrMonth(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_Q(const QtCalendarDelegate);
    QRectF itemRect = option.rect;
    bool isNow = false;
    QtCalendarData data = index.data(Qt::UserRole).value<QtCalendarData>();
    qreal baseRadius = itemWidth * 0.5;
    QtCalendarType displayMode = calendarModel->displayMode();
    if((displayMode == QtCalendarType::YearMode && data.year == nowDate.year()) ||
        (displayMode == QtCalendarType::MonthMode && data.month == nowDate.month() &&
            data.year == nowDate.year()))
    {
        isNow = true;
        QColor drawColor = option.state.testFlag(QStyle::State_MouseOver)
                               ? QtThemeColor(themeMode, PrimaryHover)
                               : QtThemeColor(themeMode, PrimaryNormal);
        painter->setPen(Qt::NoPen);
        painter->setBrush(drawColor);
        painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
    }
    else
    {
        if(option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QtThemeColor(themeMode, BasicHover));
            painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
        }
    }

    painter->setPen(
        isNow ? QtThemeColor(themeMode, BasicTextInvert) : QtThemeColor(themeMode, BasicText));
    painter->drawText(itemRect, Qt::AlignCenter,
        displayMode == QtCalendarType::YearMode ? QString::number(data.year)
                                                : QString::number(data.month));
    QString desText = data.desText;
    if(!desText.isEmpty())
    {
        itemRect.adjust(0, 10, 0, 0);
        QFont font = painter->font();
        font.setPixelSize(9);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
    }
}
void QtCalendarDelegatePrivate::drawDays(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_Q(const QtCalendarDelegate);
    qreal penWidth = 1.5;
    qreal baseRadius = itemWidth * 0.5 - penWidth;
    QVariant variant = index.data(Qt::UserRole);
    if(variant.isValid())
    {
        QRectF itemRect = option.rect;
        bool isNow = false;
        QtCalendarData data = variant.value<QtCalendarData>();
        if(data.year == nowDate.year() && data.month == nowDate.month() &&
            data.day == nowDate.day())
        {
            isNow = true;
            QColor drawColor = option.state.testFlag(QStyle::State_MouseOver)
                                   ? QtThemeColor(themeMode, PrimaryHover)
                                   : QtThemeColor(themeMode, PrimaryNormal);
            if(option.state.testFlag(QStyle::State_Selected))
            {
                painter->setPen(QPen(drawColor, penWidth));
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
                painter->setBrush(drawColor);
                painter->setPen(Qt::NoPen);
                painter->drawEllipse(
                    itemRect.center(), baseRadius - 2 * penWidth, baseRadius - 2 * penWidth);
            }
            else
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(drawColor);
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
            }
        }
        else
        {
            if(option.state.testFlag(QStyle::State_Selected))
            {
                painter->setPen(QPen(option.state.testFlag(QStyle::State_MouseOver)
                                         ? QtThemeColor(themeMode, PrimaryHover)
                                         : QtThemeColor(themeMode, PrimaryNormal),
                    penWidth));
                painter->setBrush(Qt::NoBrush);
                painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
            }
            if(option.state.testFlag(QStyle::State_MouseOver))
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QtThemeColor(themeMode, BasicHover));
                if(option.state.testFlag(QStyle::State_Selected))
                {
                    painter->drawEllipse(
                        itemRect.center(), baseRadius - penWidth, baseRadius - penWidth);
                }
                else
                {
                    painter->drawEllipse(itemRect.center(), baseRadius, baseRadius);
                }
            }
        }

        painter->setPen(
            isNow ? QtThemeColor(themeMode, BasicTextInvert) : QtThemeColor(themeMode, BasicText));
        painter->drawText(itemRect, Qt::AlignCenter, QString::number(data.day));
        QString desText = data.desText;
        if(!desText.isEmpty())
        {
            itemRect.adjust(0, 2 * penWidth, 0, 0);
            QFont font = painter->font();
            font.setPixelSize(9);
            painter->setFont(font);
            painter->drawText(itemRect, Qt::AlignTop | Qt::AlignHCenter, desText);
        }
    }
}

void QtCalendarDelegatePrivate::onCalendarModelDisplayModeChanged()
{
    Q_Q(const QtCalendarDelegate);
    switch(calendarModel->displayMode())
    {
        case YearMode:
        case MonthMode: {
            itemWidth = 53;
            itemHeight = 60;
            break;
        }
        case DayMode: {
            itemWidth = 42;
            itemHeight = 42;
            break;
        }
    }
}

QtCalendarDelegate::QtCalendarDelegate(QtCalendarModel* calendarModel, QObject* parent)
    : QStyledItemDelegate{ parent }
    , d_ptr(new QtCalendarDelegatePrivate(this))
{
    Q_D(QtCalendarDelegate);
    d->isTransparent = false;
    d->calendarModel = calendarModel;
    d->itemWidth = 42;
    d->itemHeight = 42;
    d->nowDate = QDate::currentDate();
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
    connect(calendarModel, &QtCalendarModel::displayModeChanged, this,
        [this]() { d_ptr->onCalendarModelDisplayModeChanged(); });
}

QtCalendarDelegate::~QtCalendarDelegate()
{
}

bool QtCalendarDelegate::isTransparent() const
{
    Q_D(const QtCalendarDelegate);
    return d->isTransparent;
}
void QtCalendarDelegate::setIsTransparent(bool value)
{
    Q_D(QtCalendarDelegate);
    d->isTransparent = value;
}
void QtCalendarDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtCalendarDelegate);
    if(d->isTransparent)
    {
        return;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QtCalendarType displayModel = d->calendarModel->displayMode();
    switch(displayModel)
    {
        case YearMode:
        case MonthMode: {
            d->drawYearOrMonth(painter, option, index);
            break;
        }
        case DayMode: {
            d->drawDays(painter, option, index);
            break;
        }
    }
    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}
QSize QtCalendarDelegate::sizeHint(
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtCalendarDelegate);
    return QSize(d->itemWidth, d->itemHeight);
}