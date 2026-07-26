#include "qtcalendartitledelegate.h"
#include "qtdef.h"
#include "qttheme.h"

class QtCalendarTitleDelegatePrivate
{
    Q_DECLARE_PUBLIC(QtCalendarTitleDelegate)
public:
    QtCalendarTitleDelegate* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtCalendarTitleDelegatePrivate(QtCalendarTitleDelegate* q);
    ~QtCalendarTitleDelegatePrivate();
};

QtCalendarTitleDelegatePrivate::QtCalendarTitleDelegatePrivate(QtCalendarTitleDelegate* q)
    : q_ptr(q)
{
}
QtCalendarTitleDelegatePrivate::~QtCalendarTitleDelegatePrivate()
{
}

QtCalendarTitleDelegate::QtCalendarTitleDelegate(QObject* parent)
    : QStyledItemDelegate{ parent }
    , d_ptr(new QtCalendarTitleDelegatePrivate(this))
{
    Q_D(QtCalendarTitleDelegate);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtCalendarTitleDelegate::~QtCalendarTitleDelegate()
{
}

void QtCalendarTitleDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtCalendarTitleDelegate);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRectF itemRect = option.rect;
    QString title = index.data(Qt::UserRole).toString();
    if(!title.isEmpty())
    {
        painter->setPen(QtThemeColor(d->themeMode, BasicText));
        QFont font = painter->font();
        font.setWeight(QFont::Bold);
        painter->setFont(font);
        painter->drawText(itemRect, Qt::AlignCenter, title);
    }
    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}

QSize QtCalendarTitleDelegate::sizeHint(
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(42, 30);
}
