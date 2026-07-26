#include "qtfooterdelegate.h"
#include "itemviews/models/qtfootermodel.h"
#include "itemviews/views/qtbaselistview.h"
#include "qttheme.h"
#include "utils/qtnavigationnode.h"
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>
#include <qstyleditemdelegate.h>
#include <qtypes.h>
#include <qvariantanimation.h>

class QtFooterDelegatePrivate
{
    Q_DECLARE_PUBLIC(QtFooterDelegate)
public:
    QtBaseListView* qtListView;
    QModelIndex pressIndex;
    QtFooterDelegate* q_ptr;
    int leftPadding{ 11 };
    int iconAreaWidth{ 40 };
    int textRightSpacing{ 3 };
    int indicatorIconAreaWidth{ 24 };
    qreal lastSelectMarkTop{ 10 };
    qreal lastSelectMarkBottom{ 10 };
    qreal selectMarkTop{ 10 };
    qreal selectMarkBottom{ 10 };
    bool isSelectMarkDisplay{ true };
    QtNavigationNode* lastSelectedNode{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QVariantAnimation* lastSelectMarkTopAnimation{ nullptr };
    QVariantAnimation* lastSelectMarkBottomAnimation{ nullptr };
    QVariantAnimation* selectMarkTopAnimation{ nullptr };
    QVariantAnimation* selectMarkBottomAnimation{ nullptr };

    QtFooterDelegatePrivate(QtFooterDelegate* q);
    ~QtFooterDelegatePrivate();
    bool compareItemY(QtNavigationNode* node1, QtNavigationNode* node2);
};

QtFooterDelegatePrivate::QtFooterDelegatePrivate(QtFooterDelegate* q)
    : q_ptr(q)
{
}
QtFooterDelegatePrivate::~QtFooterDelegatePrivate()
{
}

bool QtFooterDelegatePrivate::compareItemY(QtNavigationNode* node1, QtNavigationNode* node2)
{
    if(!node1)
    {
        return true;
    }
    if(!node2)
    {
        return false;
    }
    if(node1->modelIndex().row() < node2->modelIndex().row())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QtFooterDelegate::QtFooterDelegate(QObject* parent)
    : QStyledItemDelegate{ parent }
    , d_ptr(new QtFooterDelegatePrivate(this))
{
    Q_D(QtFooterDelegate);
    d->qtListView = nullptr;
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
    d->selectMarkTop = 10.0;
    d->lastSelectMarkBottom = 10.0;
    d->selectMarkTop = 10.0;
    d->selectMarkBottom = 10.0;
    // setProperty("lastSelectMarkTop", 10.0);
    // setProperty("lastSelectMarkBottom", 10.0);
    // setProperty("selectMarkTop", 10.0);
    // setProperty("selectMarkBottom", 10.0);
    // Mark向上
    d->lastSelectMarkTopAnimation = new QVariantAnimation(this);
    connect(d->lastSelectMarkTopAnimation, &QVariantAnimation::valueChanged, this,
        [this](const QVariant& value) {
            d_ptr->lastSelectMarkTop = value.toReal();
            d_ptr->qtListView->viewport()->update();
        });
    d->lastSelectMarkTopAnimation->setDuration(300);
    d->lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    d->selectMarkBottomAnimation = new QVariantAnimation(this);
    connect(d->selectMarkBottomAnimation, &QVariantAnimation::valueChanged, this,
        [this](const QVariant& value) {
            d_ptr->selectMarkBottom = value.toReal();
            d_ptr->qtListView->viewport()->update();
        });
    d->selectMarkBottomAnimation->setDuration(300);
    d->selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->lastSelectMarkTopAnimation, &QVariantAnimation::finished, this, [this]() {
        d_ptr->isSelectMarkDisplay = true;
        d_ptr->lastSelectedNode = nullptr;
        d_ptr->selectMarkBottomAnimation->setStartValue(0);
        d_ptr->selectMarkBottomAnimation->setEndValue(10);
        d_ptr->selectMarkBottomAnimation->start();
    });

    // Mark向下
    d->lastSelectMarkBottomAnimation = new QVariantAnimation(this);
    connect(d->lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) {
            d_ptr->lastSelectMarkBottom = value.toReal();
            d_ptr->qtListView->viewport()->update();
        });
    d->lastSelectMarkBottomAnimation->setDuration(300);
    d->lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    d->selectMarkTopAnimation = new QVariantAnimation(this);
    connect(d->selectMarkTopAnimation, &QVariantAnimation::valueChanged, this,
        [this](const QVariant& value) {
            d_ptr->selectMarkTop = value.toReal();
            d_ptr->qtListView->viewport()->update();
        });
    d->selectMarkTopAnimation->setDuration(300);
    d->selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [this]() {
        d_ptr->isSelectMarkDisplay = true;
        d_ptr->lastSelectedNode = nullptr;
        d_ptr->selectMarkTopAnimation->setStartValue(0);
        d_ptr->selectMarkTopAnimation->setEndValue(10);
        d_ptr->selectMarkTopAnimation->start();
    });
}

QtFooterDelegate::~QtFooterDelegate()
{
}

void QtFooterDelegate::navigationNodeStateChange(QVariantMap data)
{
    Q_D(QtFooterDelegate);
    if(data.contains("SelectMarkChanged"))
    {
        d->lastSelectedNode = data.value("LastSelectedNode").value<QtNavigationNode*>();
        QtNavigationNode* selectedNode = data.value("SelectedNode").value<QtNavigationNode*>();
        bool direction = d->compareItemY(selectedNode, d->lastSelectedNode);
        d->lastSelectMarkTop = 10;
        d->lastSelectMarkBottom = 10;
        d->selectMarkTop = 10;
        d->selectMarkBottom = 10;
        if(direction)
        {
            d->lastSelectMarkTopAnimation->setStartValue(10);
            d->lastSelectMarkTopAnimation->setEndValue(0);
            d->lastSelectMarkTopAnimation->start();
            d->lastSelectMarkBottomAnimation->stop();
            d->selectMarkTopAnimation->stop();
            d->isSelectMarkDisplay = false;
        }
        else
        {
            d->lastSelectMarkBottomAnimation->setStartValue(10);
            d->lastSelectMarkBottomAnimation->setEndValue(0);
            d->lastSelectMarkBottomAnimation->start();
            d->lastSelectMarkTopAnimation->stop();
            d->selectMarkBottomAnimation->stop();
            d->isSelectMarkDisplay = false;
        }
    }
}

QtBaseListView* QtFooterDelegate::qtListView() const
{
    Q_D(const QtFooterDelegate);
    return d->qtListView;
}
void QtFooterDelegate::setQtListView(QtBaseListView* value)
{
    Q_D(QtFooterDelegate);
    d->qtListView = value;
}

QModelIndex QtFooterDelegate::pressIndex() const
{
    Q_D(const QtFooterDelegate);
    return d->pressIndex;
}
void QtFooterDelegate::setPressIndex(QModelIndex value)
{
    Q_D(QtFooterDelegate);
    d->pressIndex = value;
}

void QtFooterDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtFooterDelegate);
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    QtFooterModel* model =
        dynamic_cast<QtFooterModel*>(const_cast<QAbstractItemModel*>(index.model()));
    QtNavigationNode* node = index.data(Qt::UserRole).value<QtNavigationNode*>();
    if(option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, viewOption, index);
    QRect itemRect = option.rect;
    painter->save();
    painter->setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect.setTop(itemRect.top() + 2);
    itemRect.setBottom(itemRect.bottom() - 2);
    QPainterPath path;
    path.addRoundedRect(itemRect, 8, 8);
    if(option.state & QStyle::State_Selected)
    {
        if(index == d->pressIndex)
        {
            painter->fillPath(path, QtThemeColor(d->themeMode, BasicHoverAlpha));
        }
        else
        {
            if(option.state & QStyle::State_MouseOver)
            {
                painter->fillPath(path, QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
            }
            else
            {
                painter->fillPath(path, QtThemeColor(d->themeMode, BasicSelectedAlpha));
            }
        }
    }
    else
    {
        if(index == d->pressIndex)
        {
            painter->fillPath(path, QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
        }
        else
        {
            if(option.state & QStyle::State_MouseOver)
            {
                painter->fillPath(path, QtThemeColor(d->themeMode, BasicHoverAlpha));
            }
        }
    }
    painter->restore();

    painter->save();
    painter->setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    itemRect = option.rect;

    if(index.row() == 0)
    {
        painter->setPen(QtThemeColor(d->themeMode, BasicBaseLine));
        painter->drawLine(option.rect.x(), itemRect.y() + 1, option.rect.x() + option.rect.width(),
            itemRect.y() + 1);
    }
    painter->setPen(index == d->pressIndex ? QtThemeColor(d->themeMode, BasicTextPress)
                                           : QtThemeColor(d->themeMode, BasicText));
    if(node->awesome() != QtIconType::None)
    {
        painter->save();
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(QRect(itemRect.x(), itemRect.y(), d->iconAreaWidth, itemRect.height()),
            Qt::AlignCenter, QChar(static_cast<ushort>(node->awesome())));
        painter->restore();
    }

    int keyPoints = node->keyPoints();
    if(keyPoints)
    {
        // KeyPoints
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 10, 10);
        painter->setBrush(QtThemeColor(d->themeMode, StatusDanger));
        painter->drawEllipse(QPoint(255, itemRect.y() + itemRect.height() / 2), 9, 9);
        painter->setPen(QPen(Qt::white, 2));
        QFont font = painter->font();
        font.setBold(true);
        if(keyPoints > 99)
        {
            keyPoints = 99;
        }
        if(keyPoints > 9)
        {
            font.setPixelSize(11);
        }
        else
        {
            font.setPixelSize(12);
        }
        painter->setFont(font);
        painter->drawText(keyPoints > 9 ? 248 : 251, itemRect.y() + itemRect.height() / 2 + 4,
            QString::number(keyPoints));
        painter->restore();
    }

    painter->setPen(index == d->pressIndex ? QtThemeColor(d->themeMode, BasicTextPress)
                                           : QtThemeColor(d->themeMode, BasicText));
    QRect textRect;
    if(node->awesome() != QtIconType::None)
    {
        textRect = QRect(itemRect.x() + d->iconAreaWidth, itemRect.y(),
            itemRect.width() - d->textRightSpacing - d->indicatorIconAreaWidth - d->iconAreaWidth,
            itemRect.height());
    }
    else
    {
        textRect = QRect(itemRect.x() + d->leftPadding, itemRect.y(),
            itemRect.width() - d->textRightSpacing - d->indicatorIconAreaWidth - d->leftPadding,
            itemRect.height());
    }
    QString text =
        painter->fontMetrics().elidedText(node->nodeTitle(), Qt::ElideRight, textRect.width());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    if(d->isSelectMarkDisplay && (node == model->selectedNode()))
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
        painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + d->selectMarkTop, 3,
                                     itemRect.height() - d->selectMarkTop - d->selectMarkBottom),
            3, 3);
    }
    if(node == d->lastSelectedNode)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
        painter->drawRoundedRect(
            QRectF(itemRect.x() + 3, itemRect.y() + d->lastSelectMarkTop, 3,
                itemRect.height() - d->lastSelectMarkTop - d->lastSelectMarkBottom),
            3, 3);
    }
    painter->restore();
}
QSize QtFooterDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_D(const QtFooterDelegate);
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}