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
    QtCalendarDelegatePrivate(QtCalendarDelegate* q);
    ~QtCalendarDelegatePrivate();
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
}
void QtCalendarDelegatePrivate::drawDays(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_Q(const QtCalendarDelegate);
}

QtCalendarDelegate::QtCalendarDelegate(QObject* parent)
    : QStyledItemDelegate{ parent }
    , d_ptr(new QtCalendarDelegatePrivate(this))
{
}

QtCalendarDelegate::QtCalendarDelegate(QtCalendarModel* calendarModel, QObject* parent)
{
}

QtCalendarDelegate::~QtCalendarDelegate()
{
}

void QtCalendarDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtCalendarDelegate);
}
QSize QtCalendarDelegate::sizeHint(
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtCalendarDelegate);
    return QSize();
}