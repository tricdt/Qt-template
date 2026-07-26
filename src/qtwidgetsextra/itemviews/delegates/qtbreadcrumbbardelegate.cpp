#include "qtbreadcrumbbardelegate.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qabstractitemmodel.h>
class QtBreadcrumbBarDelegatePrivate
{
    Q_DECLARE_PUBLIC(QtBreadcrumbBarDelegate)
public:
    QtBreadcrumbBarDelegate* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QModelIndex pressIndex;
    QtBreadcrumbBarDelegatePrivate(QtBreadcrumbBarDelegate* q);
    ~QtBreadcrumbBarDelegatePrivate();
};

QtBreadcrumbBarDelegatePrivate::QtBreadcrumbBarDelegatePrivate(QtBreadcrumbBarDelegate* q)
    : q_ptr(q)
{
}
QtBreadcrumbBarDelegatePrivate::~QtBreadcrumbBarDelegatePrivate()
{
}

QtBreadcrumbBarDelegate::QtBreadcrumbBarDelegate(QObject* parent)
    : QStyledItemDelegate{ parent }
    , d_ptr(new QtBreadcrumbBarDelegatePrivate(this))
{
    Q_D(QtBreadcrumbBarDelegate);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtBreadcrumbBarDelegate::~QtBreadcrumbBarDelegate()
{
}

void QtBreadcrumbBarDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtBreadcrumbBarDelegate);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(QtThemeColor(d->themeMode, BasicText));
    QRect itemRect = option.rect;
    QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
    QString breadcrumbUserData = index.data(Qt::UserRole).toString();
    if(breadcrumbUserData != "LastBreadcrumb")
    {
        if(d->pressIndex == index)
        {
            painter->setPen(QtThemeColor(d->themeMode, BasicTextPress));
        }
        else
        {
            if(!(option.state & QStyle::State_MouseOver))
            {
                painter->setPen(QtThemeColor(d->themeMode, BasicTextNoFocus));
            }
        }
    }
    if(breadcrumbDisplayData != ">")
    {
        painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, breadcrumbDisplayData);
    }
    else
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(painter->font().pixelSize() * 0.785);
        painter->setFont(iconFont);
        itemRect.setX(itemRect.x() - itemRect.width() * 0.36);
        painter->drawText(
            itemRect, Qt::AlignCenter, QChar(static_cast<ushort>(QtIconType::AngleRight)));
    }
    painter->restore();
}

QModelIndex QtBreadcrumbBarDelegate::pressIndex() const
{
    Q_D(const QtBreadcrumbBarDelegate);
    return d->pressIndex;
}
void QtBreadcrumbBarDelegate::setPressIndex(QModelIndex value)
{
    Q_D(QtBreadcrumbBarDelegate);
    d->pressIndex = value;
}
