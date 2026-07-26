#include "qtcolordisplaydelegate.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qobject.h>
#include <qpainter.h>

class QtColorDisplayDelegatePrivate
{
    Q_DECLARE_PUBLIC(QtColorDisplayDelegate)
public:
    QtColorDisplayDelegate* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtColorDisplayDelegatePrivate(QtColorDisplayDelegate* q);
    ~QtColorDisplayDelegatePrivate();
};

QtColorDisplayDelegatePrivate::QtColorDisplayDelegatePrivate(QtColorDisplayDelegate* q)
    : q_ptr(q)
{
}
QtColorDisplayDelegatePrivate::~QtColorDisplayDelegatePrivate()
{
}

QtColorDisplayDelegate::QtColorDisplayDelegate(QObject* parent)
    : QStyledItemDelegate{ parent }
    , d_ptr(new QtColorDisplayDelegatePrivate(this))
{
    Q_D(QtColorDisplayDelegate);
    d->themeMode = qtTheme->getThemeMode();
}

QtColorDisplayDelegate::~QtColorDisplayDelegate()
{
}

void QtColorDisplayDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtColorDisplayDelegate);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QRectF itemRect = option.rect;
    QColor displayColor = index.data(Qt::UserRole).value<QColor>();
    if(displayColor.isValid())
    {
        painter->setPen(QtThemeColor(d->themeMode, PopupBorderHover));
        painter->setBrush(index.data(Qt::UserRole).value<QColor>());
    }
    else
    {
        QPen pen(QtThemeColor(d->themeMode, PopupBorderHover));
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
    }
    painter->drawEllipse(itemRect.center(), 10, 10);

    if(option.state.testFlag(QStyle::State_MouseOver) ||
        option.state.testFlag(QStyle::State_Selected))
    {
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(itemRect.center(), 13, 13);
    }

    painter->restore();
    QStyledItemDelegate::paint(painter, option, index);
}

QSize QtColorDisplayDelegate::sizeHint(
    const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(30, 30);
}
