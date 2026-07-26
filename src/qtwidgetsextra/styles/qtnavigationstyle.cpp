#include "qtnavigationstyle.h"
#include "itemviews/models/qtnavigationmodel.h"
#include "qttheme.h"
#include "utils/qtnavigationnode.h"
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>

class QtNavigationStylePrivate
{
    Q_DECLARE_PUBLIC(QtNavigationStyle)
public:
    qreal opacity;
    qreal rotate;
    int itemHeight;
    QtNavigationView* navigationView;
    qreal lastSelectMarkTop;
    qreal lastSelectMarkBottom;
    qreal selectMarkTop;
    qreal selectMarkBottom;
    QModelIndex pressIndex;
    QtNavigationStyle* q_ptr;
    QtThemeType::ThemeMode themeMode;
    bool isSelectMarkDisplay{ true };
    int leftPadding{ 11 };
    int iconAreaWidth{ 40 };
    int textRightSpacing{ 3 };
    int indicatorIconAreaWidth{ 34 };
    QtNavigationNode* opacityAnimationTargetNode{ nullptr };
    QtNavigationNode* expandAnimationTargetNode{ nullptr };
    QtNavigationNode* lastSelectedNode{ nullptr };
    QPropertyAnimation* lastSelectMarkTopAnimation{ nullptr };
    QPropertyAnimation* lastSelectMarkBottomAnimation{ nullptr };
    QPropertyAnimation* selectMarkTopAnimation{ nullptr };
    QPropertyAnimation* selectMarkBottomAnimation{ nullptr };
    QtNavigationStylePrivate(QtNavigationStyle* q);
    ~QtNavigationStylePrivate();
    bool compareItemY(QtNavigationNode* node1, QtNavigationNode* node2);
};

QtNavigationStylePrivate::QtNavigationStylePrivate(QtNavigationStyle* q)
    : q_ptr(q)
{
}
QtNavigationStylePrivate::~QtNavigationStylePrivate()
{
}

bool QtNavigationStylePrivate::compareItemY(QtNavigationNode* node1, QtNavigationNode* node2)
{
    if(!node1)
    {
        return false;
    }
    if(!node2)
    {
        return true;
    }
    if(node1->parentNode() == node2->parentNode())
    {
        if(node1->modelIndex().row() < node2->modelIndex().row())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        QtNavigationNode* node1OriginalNode = node1->originalNode();
        QtNavigationNode* node2OriginalNode = node2->originalNode();
        if(node1OriginalNode == node2OriginalNode)
        {
            int node1Depth = node1->depth();
            int node2Depth = node2->depth();
            if(node1Depth == node2Depth)
            {
                QtNavigationNode* node1ParentNode = node1->parentNode();
                QtNavigationNode* node2ParentNode = node2->parentNode();
                if(node1ParentNode->modelIndex().row() < node2ParentNode->modelIndex().row())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if(node1Depth < node2Depth)
                {
                    QtNavigationNode* node2ParentNode = node2->parentNode();
                    while(node2ParentNode->depth() != node1Depth)
                    {
                        node2ParentNode = node2ParentNode->parentNode();
                    }
                    if(node1 == node2ParentNode)
                    {
                        return true;
                    }
                    if(node1->modelIndex().row() < node2ParentNode->modelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    QtNavigationNode* node1ParentNode = node1->parentNode();
                    while(node1ParentNode->depth() != node2Depth)
                    {
                        node1ParentNode = node1ParentNode->parentNode();
                    }
                    if(node2 == node1ParentNode)
                    {
                        return false;
                    }
                    if(node1ParentNode->modelIndex().row() < node2->modelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            if(node1OriginalNode->modelIndex().row() < node2OriginalNode->modelIndex().row())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

QtNavigationStyle::QtNavigationStyle(QStyle* style)
    : QProxyStyle{ style }
    , d_ptr(new QtNavigationStylePrivate(this))
{
    Q_D(QtNavigationStyle);
    d->opacity = 1;
    d->itemHeight = 38;
    d->lastSelectMarkTop = 10.5;
    d->lastSelectMarkBottom = 10.5;
    d->selectMarkTop = 10.5;
    d->selectMarkBottom = 10.5;

    // Mark向上
    d->lastSelectMarkTopAnimation = new QPropertyAnimation(this, "lastSelectMarkTop");
    connect(d->lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant&) { d_ptr->navigationView->viewport()->update(); });
    d->lastSelectMarkTopAnimation->setDuration(300);
    d->lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    d->selectMarkBottomAnimation = new QPropertyAnimation(this, "selectMarkBottom");
    connect(d->selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant&) { d_ptr->navigationView->viewport()->update(); });
    d->selectMarkBottomAnimation->setDuration(300);
    d->selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->isSelectMarkDisplay = true;
        d->lastSelectedNode = nullptr;
        d->selectMarkBottomAnimation->setStartValue(0);
        d->selectMarkBottomAnimation->setEndValue(10.5);
        d->selectMarkBottomAnimation->start();
    });

    // Mark向下
    d->lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "lastSelectMarkBottom");
    connect(d->lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant&) { d_ptr->navigationView->viewport()->update(); });
    d->lastSelectMarkBottomAnimation->setDuration(300);
    d->lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    d->selectMarkTopAnimation = new QPropertyAnimation(this, "selectMarkTop");
    connect(d->selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant&) { d_ptr->navigationView->viewport()->update(); });
    d->selectMarkTopAnimation->setDuration(300);
    d->selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(d->lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->isSelectMarkDisplay = true;
        d->lastSelectedNode = nullptr;
        d->selectMarkTopAnimation->setStartValue(0.0);
        d->selectMarkTopAnimation->setEndValue(10.5);
        d->selectMarkTopAnimation->start();
    });
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtNavigationStyle::~QtNavigationStyle()
{
}

void QtNavigationStyle::navigationNodeStateChange(QVariantMap data)
{
    Q_D(QtNavigationStyle);
    if(data.contains("Expand"))
    {
        QtNavigationNode* lastExpandNode = d->expandAnimationTargetNode;
        d->opacityAnimationTargetNode = data.value("Expand").value<QtNavigationNode*>();
        d->expandAnimationTargetNode = d->opacityAnimationTargetNode;
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "opacity");
        connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this,
            [this]() { d_ptr->opacityAnimationTargetNode = nullptr; });
        connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this,
            [this](const QVariant& value) { d_ptr->navigationView->viewport()->update(); });
        nodeOpacityAnimation->setDuration(480);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setKeyValueAt(0.5, 0);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(1);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "rotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this,
            [this]() { d_ptr->expandAnimationTargetNode = nullptr; });
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
            [this](const QVariant& value) { d_ptr->navigationView->viewport()->update(); });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if(lastExpandNode == d->expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(d->rotate);
        }
        else
        {
            rotateAnimation->setStartValue(0);
        }
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if(data.contains("Collapse"))
    {
        QtNavigationNode* lastExpandNode = d->expandAnimationTargetNode;
        d->opacityAnimationTargetNode = data.value("Collapse").value<QtNavigationNode*>();
        d->expandAnimationTargetNode = d->opacityAnimationTargetNode;
        d->opacity = 0;

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "rotate");
        connect(rotateAnimation, &QPropertyAnimation::finished, this, [this]() {
            d_ptr->opacity = 1;
            d_ptr->expandAnimationTargetNode = nullptr;
        });
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
            [this](const QVariant& value) { d_ptr->navigationView->viewport()->update(); });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if(lastExpandNode == d->expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(d->rotate);
        }
        else
        {
            rotateAnimation->setStartValue(-180);
        }
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if(data.contains("SelectMarkChanged"))
    {
        d->lastSelectedNode = data.value("LastSelectedNode").value<QtNavigationNode*>();
        QtNavigationNode* selectedNode = data.value("SelectedNode").value<QtNavigationNode*>();
        bool direction = d->compareItemY(selectedNode, d->lastSelectedNode);
        d->lastSelectMarkTop = 10.5;
        d->lastSelectMarkBottom = 10.5;
        d->selectMarkTop = 10.5;
        d->selectMarkBottom = 10.5;
        if(direction)
        {
            d->lastSelectMarkTopAnimation->setStartValue(10.5);
            d->lastSelectMarkTopAnimation->setEndValue(0.0);
            d->lastSelectMarkTopAnimation->start();
            d->lastSelectMarkBottomAnimation->stop();
            d->selectMarkTopAnimation->stop();
            d->isSelectMarkDisplay = false;
        }
        else
        {
            d->lastSelectMarkBottomAnimation->setStartValue(10.5);
            d->lastSelectMarkBottomAnimation->setEndValue(0.0);
            d->lastSelectMarkBottomAnimation->start();
            d->lastSelectMarkTopAnimation->stop();
            d->selectMarkBottomAnimation->stop();
            d->isSelectMarkDisplay = false;
        }
    }
}

qreal QtNavigationStyle::opacity() const
{
    Q_D(const QtNavigationStyle);
    return d->opacity;
}
void QtNavigationStyle::setOpacity(qreal value)
{
    Q_D(QtNavigationStyle);
    d->opacity = value;
}

qreal QtNavigationStyle::rotate() const
{
    Q_D(const QtNavigationStyle);
    return d->rotate;
}
void QtNavigationStyle::setRotate(qreal value)
{
    Q_D(QtNavigationStyle);
    d->rotate = value;
}

int QtNavigationStyle::itemHeight() const
{
    Q_D(const QtNavigationStyle);
    return d->itemHeight;
}
void QtNavigationStyle::setItemHeight(int value)
{
    Q_D(QtNavigationStyle);
    d->itemHeight = value;
}

QtNavigationView* QtNavigationStyle::navigationView() const
{
    Q_D(const QtNavigationStyle);
    return d->navigationView;
}
void QtNavigationStyle::setNavigationView(QtNavigationView* value)
{
    Q_D(QtNavigationStyle);
    d->navigationView = value;
}

qreal QtNavigationStyle::lastSelectMarkTop() const
{
    Q_D(const QtNavigationStyle);
    return d->lastSelectMarkTop;
}
void QtNavigationStyle::setLastSelectMarkTop(qreal value)
{
    Q_D(QtNavigationStyle);
    d->lastSelectMarkTop = value;
}

qreal QtNavigationStyle::lastSelectMarkBottom() const
{
    Q_D(const QtNavigationStyle);
    return d->lastSelectMarkBottom;
}
void QtNavigationStyle::setLastSelectMarkBottom(qreal value)
{
    Q_D(QtNavigationStyle);
    d->lastSelectMarkBottom = value;
}

qreal QtNavigationStyle::selectMarkTop() const
{
    Q_D(const QtNavigationStyle);
    return d->selectMarkTop;
}
void QtNavigationStyle::setSelectMarkTop(qreal value)
{
    Q_D(QtNavigationStyle);
    d->selectMarkTop = value;
}

qreal QtNavigationStyle::selectMarkBottom() const
{
    Q_D(const QtNavigationStyle);
    return d->selectMarkBottom;
}
void QtNavigationStyle::setSelectMarkBottom(qreal value)
{
    Q_D(QtNavigationStyle);
    d->selectMarkBottom = value;
}

QModelIndex QtNavigationStyle::pressIndex() const
{
    Q_D(const QtNavigationStyle);
    return d->pressIndex;
}
void QtNavigationStyle::setPressIndex(QModelIndex value)
{
    Q_D(QtNavigationStyle);
    d->pressIndex = value;
}

void QtNavigationStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtNavigationStyle);
    switch(element)
    {
        case QStyle::PE_PanelItemViewItem: {
            // Item背景
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                QModelIndex index = vopt->index;
                QtNavigationNode* node = static_cast<QtNavigationNode*>(index.internalPointer());
                if(node->isCategoryNode())
                {
                    return;
                }
                painter->save();
                if(d->opacityAnimationTargetNode &&
                    node->parentNode() == d->opacityAnimationTargetNode)
                {
                    painter->setOpacity(d->opacity);
                }
                painter->setRenderHint(QPainter::Antialiasing);
                QRect itemRect = vopt->rect;
                itemRect.setTop(itemRect.top() + 2);
                itemRect.setBottom(itemRect.bottom() - 2);
                QPainterPath path;
                path.addRoundedRect(itemRect, 5, 5);
                if(vopt->state & QStyle::State_Selected)
                {
                    if(index == d->pressIndex)
                    {
                        painter->fillPath(path, QtThemeColor(d->themeMode, BasicHoverAlpha));
                    }
                    else
                    {
                        if(vopt->state & QStyle::State_MouseOver)
                        {
                            painter->fillPath(
                                path, QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
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
                        painter->fillPath(
                            path, QtThemeColor(d->themeMode, BasicSelectedHoverAlpha));
                    }
                    else
                    {
                        if(vopt->state & QStyle::State_MouseOver)
                        {
                            painter->fillPath(path, QtThemeColor(d->themeMode, BasicHoverAlpha));
                        }
                    }
                }
                painter->restore();
            }
            return;
        }
        case QStyle::PE_PanelItemViewRow: {
            return;
        }
        case QStyle::PE_IndicatorBranch: {
            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
void QtNavigationStyle::drawControl(ControlElement element, const QStyleOption* option,
    QPainter* painter, const QWidget* widget) const
{
    Q_D(const QtNavigationStyle);
    switch(element)
    {
        case QStyle::CE_ShapedFrame: {
            // viewport视口外的其他区域背景
            return;
        }
        case QStyle::CE_ItemViewItem: {
            if(const QStyleOptionViewItem* vopt =
                    qstyleoption_cast<const QStyleOptionViewItem*>(option))
            {
                this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);

                QRect itemRect = option->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform |
                                        QPainter::TextAntialiasing);
                QtNavigationNode* node =
                    static_cast<QtNavigationNode*>(vopt->index.internalPointer());
                QtNavigationModel* model = dynamic_cast<QtNavigationModel*>(
                    const_cast<QAbstractItemModel*>(vopt->index.model()));

                if(d->opacityAnimationTargetNode &&
                    node->parentNode() == d->opacityAnimationTargetNode)
                {
                    painter->setOpacity(d->opacity);
                }

                if(d->isSelectMarkDisplay &&
                    (node == model->selectedNode() || node == model->selectedExpandedNode()))
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                    painter->drawRoundedRect(
                        QRectF(itemRect.x() + 3, itemRect.y() + d->selectMarkTop, 3,
                            itemRect.height() - d->selectMarkTop - d->selectMarkBottom),
                        1.5, 1.5);
                }
                if(node == d->lastSelectedNode)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                    painter->drawRoundedRect(
                        QRectF(itemRect.x() + 3, itemRect.y() + d->lastSelectMarkTop, 3,
                            itemRect.height() - d->lastSelectMarkTop - d->lastSelectMarkBottom),
                        1.5, 1.5);
                }

                painter->setPen(vopt->index == d->pressIndex
                                    ? QtThemeColor(d->themeMode, BasicTextPress)
                                    : QtThemeColor(d->themeMode, BasicText));
                if(node->awesome() != QtIconType::None)
                {
                    painter->save();
                    QFont iconFont = QFont("ElaAwesome");
                    iconFont.setPixelSize(17);
                    painter->setFont(iconFont);
                    painter->drawText(
                        QRect(itemRect.x(), itemRect.y(), d->iconAreaWidth, itemRect.height()),
                        Qt::AlignCenter, QChar(static_cast<ushort>(node->awesome())));
                    painter->restore();
                }

                int viewWidth = widget->width();
                if(node->isCategoryNode())
                {
                    QFont categoryFont = painter->font();
                    categoryFont.setBold(true);
                    painter->setFont(categoryFont);
                    painter->setPen(QtThemeColor(d->themeMode, BasicTextCategory));
                }
                else
                {
                    painter->setPen((vopt->index == d->pressIndex)
                                        ? QtThemeColor(d->themeMode, BasicTextPress)
                                        : QtThemeColor(d->themeMode, BasicText));
                }
                QRect textRect;
                if(node->awesome() != QtIconType::None)
                {
                    textRect = QRect(itemRect.x() + d->iconAreaWidth, itemRect.y(),
                        itemRect.width() - d->textRightSpacing - d->indicatorIconAreaWidth -
                            d->iconAreaWidth,
                        itemRect.height());
                }
                else
                {
                    textRect = QRect(itemRect.x() + d->leftPadding, itemRect.y(),
                        itemRect.width() - d->textRightSpacing - d->indicatorIconAreaWidth -
                            d->leftPadding,
                        itemRect.height());
                }
                QString text = painter->fontMetrics().elidedText(
                    node->nodeTitle(), Qt::ElideRight, textRect.width());
                painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

                if(viewWidth > d->navigationView->navigationBar()->navigationBarWidth() -
                                   d->indicatorIconAreaWidth)
                {
                    if(node->isExpanderNode())
                    {
                        if(node->isHasChild())
                        {
                            QRectF expandIconRect(itemRect.right() - d->indicatorIconAreaWidth,
                                itemRect.y(), 17, itemRect.height());
                            painter->save();
                            QFont iconFont = QFont("ElaAwesome");
                            iconFont.setPixelSize(17);
                            painter->setFont(iconFont);
                            painter->translate(
                                expandIconRect.x() + (qreal) expandIconRect.width() / 2,
                                expandIconRect.y() + (qreal) expandIconRect.height() / 2);
                            if(node == d->expandAnimationTargetNode)
                            {
                                painter->rotate(d->rotate);
                            }
                            else
                            {
                                if(node->isExpanded())
                                {
                                    // 展开
                                    painter->rotate(-180);
                                }
                                else
                                {
                                    // 未展开
                                    painter->rotate(0);
                                }
                            }
                            painter->translate(
                                -expandIconRect.x() - (qreal) expandIconRect.width() / 2 + 1,
                                -expandIconRect.y() - (qreal) expandIconRect.height() / 2);
                            painter->drawText(expandIconRect, Qt::AlignVCenter,
                                QChar(static_cast<ushort>(QtIconType::AngleDown)));
                            painter->restore();
                        }
                        if(node->isChildHasKeyPoints())
                        {
                            painter->save();
                            painter->setPen(Qt::NoPen);
                            painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                            painter->drawEllipse(
                                QPoint(itemRect.right() - 17, itemRect.y() + 12), 3, 3);
                            painter->restore();
                        }
                    }
                    else
                    {
                        int keyPoints = node->keyPoints();
                        if(keyPoints)
                        {
                            // KeyPoints
                            painter->save();
                            painter->setPen(Qt::NoPen);
                            painter->setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
                            int keyPointRadius = 8;
                            painter->drawEllipse(
                                QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2),
                                keyPointRadius, keyPointRadius);
                            painter->setPen(QPen(QtThemeColor(d->themeMode, BasicTextInvert), 2));
                            QFont font = painter->font();
                            font.setBold(true);
                            if(keyPoints > 99)
                            {
                                keyPoints = 99;
                            }
                            if(keyPoints > 9)
                            {
                                font.setPixelSize(10);
                            }
                            else
                            {
                                font.setPixelSize(11);
                            }
                            painter->setFont(font);
                            painter->drawText(
                                QRect(QPoint(itemRect.right() - 26 - keyPointRadius,
                                          itemRect.y() + itemRect.height() / 2 - keyPointRadius),
                                    QSize(2 * keyPointRadius, 2 * keyPointRadius)),
                                Qt::AlignCenter, QString::number(keyPoints));
                            painter->restore();
                        }
                    }
                }
                painter->restore();
            }

            return;
        }
        default: {
            break;
        }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}
QSize QtNavigationStyle::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    Q_D(const QtNavigationStyle);
    switch(type)
    {
        case QStyle::CT_ItemViewItem: {
            QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            itemSize.setHeight(d->itemHeight);
            return itemSize;
        }
        default: {
            break;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}