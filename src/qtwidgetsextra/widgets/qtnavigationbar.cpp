#include "qtnavigationbar.h"
#include "command/qtnavigationroutecommand.h"
#include "itemviews/delegates/qtfooterdelegate.h"
#include "itemviews/models/qtfootermodel.h"
#include "itemviews/models/qtnavigationmodel.h"
#include "itemviews/views//qtbaselistview.h"
#include "itemviews/views/qtnavigationview.h"
#include "qtdef.h"
#include "qtinteractivecard.h"
#include "qtsuggestbox.h"
#include "qttheme.h"
#include "utils/qtnavigationnode.h"
#include "widgets/qticonbutton.h"
#include "widgets/qtmenu.h"
#include <qboxlayout.h>

class QtNavigationBarPrivate
{
    Q_DECLARE_PUBLIC(QtNavigationBar)
public:
    bool isTransparent;
    bool isAllowPageOpenInNewWindow;
    int navigationBarWidth;
    QtNavigationBar* q_ptr;
    int navigationViewWidth;
    int userButtonSpacing;
    QtThemeType::ThemeMode themeMode;
    QList<QtSuggestBox::SuggestData> suggestDataList;
    QMap<QString, const QMetaObject*> pageMetaMap;
    QMap<QString, int> pageNewWindowCountMap;
    QMap<QtNavigationNode*, QtMenu*> compactMenuMap;
    QVBoxLayout* userCardLayout{ nullptr };
    QVBoxLayout* userButtonLayout{ nullptr };
    QtIconButton* userButton{ nullptr };
    QtNavigationModel* navigationModel{ nullptr };
    QtNavigationView* navigationView{ nullptr };
    QtBaseListView* footerView{ nullptr };
    QtFooterModel* footerModel{ nullptr };
    QtFooterDelegate* footerDelegate{ nullptr };
    QtInteractiveCard* userCard{ nullptr };
    bool isShowUserCard{ true };

    QList<QtNavigationNode*> lastExpandedNodesList;

    QtNavigationType::NavigationDisplayMode currentDisplayMode{
        QtNavigationType::NavigationDisplayMode::Maximal
    };
    QtNavigationBarPrivate(QtNavigationBar* q);
    ~QtNavigationBarPrivate();

    void onTreeViewClicked(
        const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);
    void onFooterViewClicked(
        const QModelIndex& index, bool isLogRoute = true, bool isRouteBack = false);

    void initNodeModelIndex(const QModelIndex& parentIndex);
    void resetNodeSelected();
    void expandSelectedNodeParent();
    void expandOrCollapseExpanderNode(QtNavigationNode* node, bool isExpand);

    void addStackedPage(QWidget* page, QString pageKey);
    void addFooterPage(QWidget* page, QString footKey);

    void raiseNavigationBar();
    void smoothScrollNavigationView(const QModelIndex& index);

    void doComponentAnimation(
        QtNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void handleNavigationExpandState(bool isSave);
    void resetLayout();

    void doNavigationBarWidthAnimation(
        QtNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void doNavigationViewWidthAnimation(bool isAnimation);
    void doUserButtonAnimation(bool isCompact, bool isAnimation);
};

QtNavigationBarPrivate::QtNavigationBarPrivate(QtNavigationBar* q)
    : q_ptr(q)
{
}
QtNavigationBarPrivate::~QtNavigationBarPrivate()
{
}

void QtNavigationBarPrivate::onTreeViewClicked(
    const QModelIndex& index, bool isLogRoute, bool isRouteBack)
{
    Q_Q(QtNavigationBar);
    if(index.isValid())
    {
        QtNavigationNode* node = static_cast<QtNavigationNode*>(index.internalPointer());
        if(!node)
        {
            return;
        }
        if(node->isCategoryNode())
        {
            Q_EMIT q->navigationNodeClicked(
                QtNavigationType::CategoryNode, node->nodeKey(), isRouteBack);
            return;
        }
        if(node->isExpanderNode())
        {
            expandOrCollapseExpanderNode(node, !navigationView->isExpanded(index));
        }
        else
        {
            if(node->keyPoints())
            {
                node->setKeyPoints(0);
                navigationView->update();
            }
            QtNavigationNode* selectedNode = navigationModel->selectedNode();
            if(selectedNode != node)
            {
                if(isLogRoute)
                {
                    QString backPageKey;
                    if(selectedNode)
                    {
                        backPageKey = selectedNode->nodeKey();
                    }
                    else
                    {
                        if(footerModel->selectedNode())
                        {
                            backPageKey = footerModel->selectedNode()->nodeKey();
                        }
                    }
                    QtNavigationRouteCommand* command = new QtNavigationRouteCommand(q);
                    command->setNavigationBar(q);
                    command->setUndoPageKey(backPageKey);
                    command->setRedoPageKey(node->nodeKey());
                    QtActionCommander::getInstance()->recordCommand(
                        "QtWidgetsExtraAction", command, false);
                }
                Q_EMIT q->navigationNodeClicked(
                    QtNavigationType::PageNode, node->nodeKey(), isRouteBack);

                if(footerModel->selectedNode())
                {
                    footerView->clearSelection();
                    QVariantMap footerPostData = QVariantMap();
                    footerPostData.insert("SelectMarkChanged", true);
                    footerPostData.insert(
                        "LastSelectedNode", QVariant::fromValue(footerModel->selectedNode()));
                    footerPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                    footerModel->setSelectedNode(nullptr);
                    footerDelegate->navigationNodeStateChange(footerPostData);
                }
                QVariantMap postData = QVariantMap();
                postData.insert("SelectMarkChanged", true);
                if(navigationModel->selectedExpandedNode())
                {
                    postData.insert("LastSelectedNode",
                        QVariant::fromValue(navigationModel->selectedExpandedNode()));
                }
                else
                {
                    postData.insert(
                        "LastSelectedNode", QVariant::fromValue(navigationModel->selectedNode()));
                }
                if(currentDisplayMode == QtNavigationType::Compact)
                {
                    QtNavigationNode* originNode = node->originalNode();
                    if(originNode == node)
                    {
                        postData.insert("SelectedNode", QVariant::fromValue(node));
                    }
                    else
                    {
                        if(originNode == navigationModel->selectedExpandedNode())
                        {
                            navigationModel->setSelectedNode(node);
                            resetNodeSelected();
                            return;
                        }
                        navigationModel->setSelectedExpandedNode(originNode);
                        postData.insert("SelectedNode", QVariant::fromValue(originNode));
                    }
                }
                else
                {
                    postData.insert("SelectedNode", QVariant::fromValue(node));
                }
                navigationModel->setSelectedNode(node);
                navigationView->navigationNodeStateChange(postData);
                if(!node->isVisible() && currentDisplayMode != QtNavigationType::Compact)
                {
                    expandSelectedNodeParent();
                }
            }
        }
        resetNodeSelected();
    }
}
void QtNavigationBarPrivate::onFooterViewClicked(
    const QModelIndex& index, bool isLogRoute, bool isRouteBack)
{
    Q_Q(QtNavigationBar);
    QtNavigationNode* node = index.data(Qt::UserRole).value<QtNavigationNode*>();
    if(node->keyPoints())
    {
        node->setKeyPoints(0);
        navigationView->update();
    }
    QtNavigationNode* selectedNode = footerModel->selectedNode();

    if(selectedNode != node)
    {
        if(isLogRoute && node->isHasFooterPage())
        {
            QVariantMap routeData = QVariantMap();
            QString backPageKey;
            if(selectedNode)
            {
                backPageKey = selectedNode->nodeKey();
            }
            else
            {
                if(navigationModel->selectedNode())
                {
                    backPageKey = navigationModel->selectedNode()->nodeKey();
                }
            }
            // QtNavigationRouteCommand* command = new QtNavigationRouteCommand(this);
            // command->setNavigationBar(q);
            // command->setUndoPageKey(backPageKey);
            // command->setRedoPageKey(node->getNodeKey());
            // ElaActionCommander::getInstance()->recordCommand(
            //     "ElaWidgetToolsAction", command, false);
        }
        // Q_EMIT q->navigationNodeClicked(
        //     QtNavigationType::FooterNode, node->getNodeKey(), isRouteBack);

        if(node->isHasFooterPage())
        {
            // if(navigationModel->getSelectedNode() ||
            // navigationModel->getSelectedExpandedNode())
            // {
            //     QVariantMap mainPostData = QVariantMap();
            //     mainPostData.insert("SelectMarkChanged", true);
            //     mainPostData.insert("LastSelectedNode",
            //         QVariant::fromValue(navigationModel->getSelectedExpandedNode()
            //                                 ? navigationModel->getSelectedExpandedNode()
            //                                 : navigationModel->getSelectedNode()));
            //     mainPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
            //     navigationView->clearSelection();
            //     navigationView->navigationNodeStateChange(mainPostData);
            //     navigationModel->setSelectedExpandedNode(nullptr);
            //     navigationModel->setSelectedNode(nullptr);
            // }
            footerView->clearSelection();
            footerView->selectionModel()->select(index, QItemSelectionModel::Select);
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(footerModel->selectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(node));
            footerDelegate->navigationNodeStateChange(postData);
            footerModel->setSelectedNode(node);
        }
    }
}
void QtNavigationBarPrivate::initNodeModelIndex(const QModelIndex& parentIndex)
{
    int rowCount = navigationModel->rowCount(parentIndex);
    for(int row = 0; row < rowCount; ++row)
    {
        QModelIndex index = navigationModel->index(row, 0, parentIndex);
        QtNavigationNode* childNode = static_cast<QtNavigationNode*>(index.internalPointer());
        childNode->setModelIndex(index);
        if(navigationModel->hasChildren(index))
        {
            initNodeModelIndex(index);
        }
    }
}
void QtNavigationBarPrivate::resetNodeSelected()
{
    navigationView->clearSelection();
    QtNavigationNode* selectedNode = navigationModel->selectedNode();
    if(!selectedNode || !selectedNode->modelIndex().isValid())
    {
        return;
    }
    if(selectedNode->parentNode()->isRootNode() || selectedNode->isVisible())
    {
        navigationView->selectionModel()->select(
            selectedNode->modelIndex(), QItemSelectionModel::Select);
        if(navigationModel->selectedExpandedNode())
        {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert(
                "LastSelectedNode", QVariant::fromValue(navigationModel->selectedExpandedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(selectedNode));
            navigationView->navigationNodeStateChange(postData);
        }
        navigationModel->setSelectedExpandedNode(nullptr);
    }
    else
    {
        QtNavigationNode* parentNode = selectedNode->parentNode();
        while(parentNode && !parentNode->parentNode()->isRootNode() && !parentNode->isVisible())
        {
            parentNode = parentNode->parentNode();
        }
        if(!parentNode)
        {
            return;
        }
        if(!navigationModel->selectedExpandedNode())
        {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert(
                "LastSelectedNode", QVariant::fromValue(navigationModel->selectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(parentNode));
            navigationView->navigationNodeStateChange(postData);
        }
        else
        {
            if(navigationModel->selectedExpandedNode() != parentNode)
            {
                if(navigationModel->selectedExpandedNode()->originalNode() ==
                    parentNode->originalNode())
                {
                    QVariantMap postData = QVariantMap();
                    postData.insert("SelectMarkChanged", true);
                    postData.insert("LastSelectedNode",
                        QVariant::fromValue(navigationModel->selectedExpandedNode()));
                    postData.insert("SelectedNode", QVariant::fromValue(parentNode));
                    navigationView->navigationNodeStateChange(postData);
                }
            }
        }
        navigationModel->setSelectedExpandedNode(parentNode);
        navigationView->selectionModel()->select(
            parentNode->modelIndex(), QItemSelectionModel::Select);
    }
}
void QtNavigationBarPrivate::expandSelectedNodeParent()
{
    QtNavigationNode* parentNode = navigationModel->selectedNode()->parentNode();
    while(parentNode && !parentNode->isRootNode())
    {
        QVariantMap data;
        data.insert("Expand", QVariant::fromValue(parentNode));
        navigationView->navigationNodeStateChange(data);
        parentNode->setIsExpanded(true);
        navigationView->expand(parentNode->modelIndex());
        parentNode = parentNode->parentNode();
    }
}
void QtNavigationBarPrivate::expandOrCollapseExpanderNode(QtNavigationNode* node, bool isExpand)
{
    if(currentDisplayMode == QtNavigationType::Compact)
    {
        if(node->isHasPageChild())
        {
            QtMenu* menu = compactMenuMap.value(node);
            if(menu)
            {
                QPoint nodeTopRight = navigationView->mapToGlobal(
                    navigationView->visualRect(node->modelIndex()).topRight());
                menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
            }
        }
    }
    else
    {
        QModelIndex index = node->modelIndex();
        bool isExpanded = navigationView->isExpanded(index);
        if(node->isHasChild() && isExpand != isExpanded)
        {
            QVariantMap data;
            if(isExpanded)
            {
                data.insert("Collapse", QVariant::fromValue(node));
                node->setIsExpanded(isExpand);
                navigationView->navigationNodeStateChange(data);
                navigationView->collapse(index);
            }
            else
            {
                data.insert("Expand", QVariant::fromValue(node));
                node->setIsExpanded(true);
                navigationView->navigationNodeStateChange(data);
                navigationView->expand(index);
            }
        }
    }
}

void QtNavigationBarPrivate::addStackedPage(QWidget* page, QString pageKey)
{
}
void QtNavigationBarPrivate::addFooterPage(QWidget* page, QString footKey)
{
    Q_Q(QtNavigationBar);
    Q_EMIT q->navigationNodeAdded(QtNavigationType::FooterNode, footKey, page);
    if(page)
    {
        page->setProperty("QtPageKey", footKey);
    }
    footerView->setFixedHeight(40 * footerModel->footerNodeCount());
    QtNavigationNode* node = footerModel->navigationNode(footKey);
    QVariantMap suggestData;
    suggestData.insert("QtPageKey", footKey);
    suggestDataList.append(
        QtSuggestBox::SuggestData(node->awesome(), node->nodeTitle(), suggestData));
}

void QtNavigationBarPrivate::raiseNavigationBar()
{
}
void QtNavigationBarPrivate::smoothScrollNavigationView(const QModelIndex& index)
{
}

void QtNavigationBarPrivate::doComponentAnimation(
    QtNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
}
void QtNavigationBarPrivate::handleNavigationExpandState(bool isSave)
{
}
void QtNavigationBarPrivate::resetLayout()
{
}

void QtNavigationBarPrivate::doNavigationBarWidthAnimation(
    QtNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
}
void QtNavigationBarPrivate::doNavigationViewWidthAnimation(bool isAnimation)
{
}
void QtNavigationBarPrivate::doUserButtonAnimation(bool isCompact, bool isAnimation)
{
}

QtNavigationBar::QtNavigationBar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtNavigationBarPrivate(this))
{
    Q_D(QtNavigationBar);
    d->isAllowPageOpenInNewWindow = true;
    d->navigationBarWidth = 300;
    setFixedWidth(300);
    d->isTransparent = true;

    d->userCard = new QtInteractiveCard(this);
    d->userCard->setMinimumWidth(0);
    d->userCard->setCardPixmap(QPixmap(":/include/Cirno.png"));
    d->userCard->setTitle("Ela Tool");
    d->userCard->setSubTitle("Liniyous@gmail.com");
    connect(d->userCard, &QtInteractiveCard::clicked, this, &QtNavigationBar::userInfoCardClicked);
    d->userButton = new QtIconButton(QPixmap(":/include/Cirno.png"), this);
    d->userButton->setFixedSize(36, 36);
    d->userButton->setVisible(false);
    d->userButton->setBorderRadius(8);
    d->userButtonLayout = new QVBoxLayout();
    d->userButtonLayout->setAlignment(Qt::AlignLeft);
    d->userButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->userButtonLayout->setSpacing(0);
    d->userButtonLayout->addWidget(d->userButton);

    connect(d->userButton, &QtIconButton::clicked, this, &QtNavigationBar::userInfoCardClicked);
    d->userCardLayout = new QVBoxLayout();
    d->userCardLayout->setContentsMargins(3, 10, 0, 5);
    d->userCardLayout->addLayout(d->userButtonLayout);
    d->userCardLayout->addWidget(d->userCard);

    // //    connect(d->_navigationSuggestBox, &ElaSuggestBox::suggestionClicked, this, [=](QString
    // //    suggestText, QVariantMap suggestData) {
    // //        navigation(suggestData.value("ElaPageKey").toString());
    // //    });

    // // 导航模型
    d->navigationModel = new QtNavigationModel(this);
    d->navigationView = new QtNavigationView(this);
    d->navigationView->setNavigationBar(this);
    d->navigationView->setModel(d->navigationModel);
    connect(d->navigationView, &QtNavigationView::navigationClicked, this,
        [this](const QModelIndex& index) { d_ptr->onTreeViewClicked(index); });
    // connect(d->navigationView, &ElaNavigationView::navigationOpenNewWindow, d,
    //     &QtNavigationBarPrivate::onNavigationOpenNewWindow);

    d->footerView = new QtBaseListView(this);
    d->footerView->setFixedHeight(0);
    d->footerModel = new QtFooterModel(this);
    d->footerView->setModel(d->footerModel);
    d->footerDelegate = new QtFooterDelegate(this);
    d->footerDelegate->setQtListView(d->footerView);
    d->footerView->setItemDelegate(d->footerDelegate);
    connect(d->footerView, &QtBaseListView::mousePress, this, [this](const QModelIndex& index) {
        d_ptr->footerDelegate->setPressIndex(index);
        d_ptr->footerView->viewport()->update();
    });
    connect(
        d->footerView, &QtBaseListView::mouseDoubleClick, this, [this](const QModelIndex& index) {
            d_ptr->footerDelegate->setPressIndex(index);
            d_ptr->footerView->viewport()->update();
        });
    connect(d->footerView, &QtBaseListView::mouseRelease, this, [this](const QModelIndex&) {
        d_ptr->footerDelegate->setPressIndex(QModelIndex());
        d_ptr->footerView->viewport()->update();
    });
    connect(d->footerView, &QtBaseListView::clicked, this,
        [this](const QModelIndex& index) { d_ptr->onFooterViewClicked(index); });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    // mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(d->userCardLayout);
    mainLayout->addWidget(d->navigationView);
    mainLayout->addWidget(d->footerView);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtNavigationBar::~QtNavigationBar()
{
}

void QtNavigationBar::setUserInfoCardVisible(bool isVisible)
{
    Q_D(QtNavigationBar);
    d->isShowUserCard = isVisible;
    if(d->currentDisplayMode == QtNavigationType::NavigationDisplayMode::Compact)
    {
        d->userButton->setVisible(isVisible);
    }
    else
    {
        d->userCard->setVisible(isVisible);
    }
    if(isVisible)
    {
        d->userCardLayout->setContentsMargins(3, 10, 5, 5);
    }
    else
    {
        d->userCardLayout->setContentsMargins(0, 0, 0, 0);
    }
}

void QtNavigationBar::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(QtNavigationBar);
    d->userCard->setCardPixmap(pix);
    d->userButton->setPixmap(pix);
}

void QtNavigationBar::setUserInfoCardTitle(QString title)
{
    Q_D(QtNavigationBar);
    d->userCard->setTitle(title);
}

void QtNavigationBar::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(QtNavigationBar);
    d->userCard->setSubTitle(subTitle);
}

QtNavigationType::NodeResult QtNavigationBar::addExpanderNode(
    const QString& expanderTitle, QString& expanderKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    QtNavigationType::NodeResult result =
        d->navigationModel->addExpanderNode(expanderTitle, expanderKey, awesome);
    if(result == QtNavigationType::Success)
    {
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}
QtNavigationType::NodeResult QtNavigationBar::addExpanderNode(const QString& expanderTitle,
    QString& expanderKey, const QString& targetExpanderKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    QtNavigationType::NodeResult result =
        d->navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
    if(result == QtNavigationType::Success)
    {
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}

QtNavigationType::NodeResult QtNavigationBar::addPageNode(
    const QString& pageTitle, QWidget* page, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    if(!page)
    {
        return QtNavigationType::PageInvalid;
    }
    QString pageKey;
    QtNavigationType::NodeResult result =
        d_ptr->navigationModel->addPageNode(pageTitle, pageKey, awesome);
    if(result == QtNavigationType::Success)
    {
        d->pageMetaMap.insert(pageKey, page->metaObject());
        d->pageNewWindowCountMap.insert(pageKey, 0);
        d->addStackedPage(page, pageKey);
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}

QtNavigationType::NodeResult QtNavigationBar::addPageNode(
    const QString& pageTitle, QWidget* page, int keyPoints, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    if(!page)
    {
        return QtNavigationType::PageInvalid;
    }
    QString pageKey;
    QtNavigationType::NodeResult result =
        d_ptr->navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awesome);
    if(result == QtNavigationType::Success)
    {
        d->pageMetaMap.insert(pageKey, page->metaObject());
        d->pageNewWindowCountMap.insert(pageKey, 0);
        d->addStackedPage(page, pageKey);
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}

QtNavigationType::NodeResult QtNavigationBar::addPageNode(const QString& pageTitle, QWidget* page,
    const QString& targetExpanderKey, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    if(!page)
    {
        return QtNavigationType::PageInvalid;
    }
    if(targetExpanderKey.isEmpty())
    {
        return QtNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    QtNavigationType::NodeResult result =
        d->navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if(result == QtNavigationType::NodeResult::Success)
    {
        d->pageMetaMap.insert(pageKey, page->metaObject());
        d->pageNewWindowCountMap.insert(pageKey, 0);
        QtNavigationNode* node = d->navigationModel->navigationNode(pageKey);
        QtNavigationNode* originalNode = node->originalNode();
        if(d->compactMenuMap.contains(originalNode))
        {
            QtMenu* menu = d->compactMenuMap.value(originalNode);
            QAction* action = menu->addQtIconAction(node->awesome(), node->nodeTitle());
            connect(action, &QAction::triggered, this,
                [this, node]() { d_ptr->onTreeViewClicked(node->modelIndex()); });
        }
        else
        {
            QtMenu* menu = new QtMenu(const_cast<QtNavigationBar*>(this));
            QAction* action = menu->addQtIconAction(node->awesome(), node->nodeTitle());
            connect(action, &QAction::triggered, this,
                [this, node]() { d_ptr->onTreeViewClicked(node->modelIndex()); });
            d_ptr->compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->addStackedPage(page, pageKey);
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}

QtNavigationType::NodeResult QtNavigationBar::addPageNode(const QString& pageTitle, QWidget* page,
    const QString& targetExpanderKey, int keyPoints, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    if(!page)
    {
        return QtNavigationType::PageInvalid;
    }
    if(targetExpanderKey.isEmpty())
    {
        return QtNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    QtNavigationType::NodeResult result = d_ptr->navigationModel->addPageNode(
        pageTitle, pageKey, targetExpanderKey, keyPoints, awesome);
    if(result == QtNavigationType::Success)
    {
        d->pageMetaMap.insert(pageKey, page->metaObject());
        d->pageNewWindowCountMap.insert(pageKey, 0);
        QtNavigationNode* node = d_ptr->navigationModel->navigationNode(pageKey);
        QtNavigationNode* originalNode = node->originalNode();
        if(d_ptr->compactMenuMap.contains(originalNode))
        {
            QtMenu* menu = d_ptr->compactMenuMap.value(originalNode);
            QAction* action = menu->addQtIconAction(node->awesome(), node->nodeTitle());
            connect(action, &QAction::triggered, this,
                [this, node]() { d_ptr->onTreeViewClicked(node->modelIndex()); });
        }
        else
        {
            QtMenu* menu = new QtMenu(const_cast<QtNavigationBar*>(this));
            QAction* action = menu->addQtIconAction(node->awesome(), node->nodeTitle());
            connect(action, &QAction::triggered, this,
                [this, node]() { d_ptr->onTreeViewClicked(node->modelIndex()); });
            d_ptr->compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->addStackedPage(page, pageKey);
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}

QtNavigationType::NodeResult QtNavigationBar::addFooterNode(
    const QString& footerTitle, QString& footerKey, int keyPoints, QtIconType::IconName awesome)
{
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

QtNavigationType::NodeResult QtNavigationBar::addFooterNode(const QString& footerTitle,
    QWidget* page, QString& footerKey, int keyPoints, QtIconType::IconName awesome)
{
    Q_D(QtNavigationBar);
    QtNavigationType::NodeResult result =
        d->footerModel->addFooterNode(footerTitle, footerKey, page != nullptr, keyPoints, awesome);
    if(result == QtNavigationType::Success)
    {
        d->addFooterPage(page, footerKey);
    }
    return result;
}

QtNavigationType::NodeResult QtNavigationBar::addCategoryNode(
    const QString& categoryTitle, QString& categoryKey)
{
    Q_D(QtNavigationBar);
    QtNavigationType::NodeResult result =
        d->navigationModel->addCategoryNode(categoryTitle, categoryKey);
    if(result == QtNavigationType::Success)
    {
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
        Q_EMIT navigationNodeAdded(QtNavigationType::CategoryNode, categoryKey, nullptr);
    }
    return result;
}
QtNavigationType::NodeResult QtNavigationBar::addCategoryNode(
    const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey)
{
    Q_D(QtNavigationBar);
    QtNavigationType::NodeResult result =
        d_ptr->navigationModel->addCategoryNode(categoryTitle, categoryKey, targetExpanderKey);
    if(result == QtNavigationType::Success)
    {
        d->initNodeModelIndex(QModelIndex());
        d->resetNodeSelected();
    }
    return result;
}

bool QtNavigationBar::nodeIsExpanded(QString expanderKey) const
{
    Q_D(const QtNavigationBar);
    QtNavigationNode* node = d->navigationModel->navigationNode(expanderKey);
    if(!node || !node->isExpanderNode())
    {
        return false;
    }
    return d->navigationView->isExpanded(node->modelIndex());
}
void QtNavigationBar::expandNode(QString expanderKey)
{
    Q_D(QtNavigationBar);
    QtNavigationNode* node = d->navigationModel->navigationNode(expanderKey);
    if(!node || !node->isExpanderNode())
    {
        return;
    }
    d->expandOrCollapseExpanderNode(node, true);
    d->resetNodeSelected();
}
void QtNavigationBar::collapseNode(QString expanderKey)
{
    Q_D(QtNavigationBar);
    QtNavigationNode* node = d->navigationModel->navigationNode(expanderKey);
    if(!node || !node->isExpanderNode())
    {
        return;
    }
    d->expandOrCollapseExpanderNode(node, false);
    d->resetNodeSelected();
}
void QtNavigationBar::removeNode(QString nodeKey)
{
    Q_D(QtNavigationBar);
    QtNavigationNode* node = d->navigationModel->navigationNode(nodeKey);
    if(!node)
    {
        node = d->footerModel->navigationNode(nodeKey);
    }
    if(!node)
    {
        return;
    }
    if(node->isFooterNode())
    {
        Q_EMIT navigationNodeRemoved(QtNavigationType::FooterNode, nodeKey);
        d->footerModel->removeNavigationNode(nodeKey);
        d->footerView->setFixedHeight(40 * d->footerModel->footerNodeCount());
    }
    else
    {
        if(node->isCategoryNode())
        {
            QStringList removeKeyList = d->navigationModel->removeNavigationNode(nodeKey);
            d->initNodeModelIndex(QModelIndex());
            for(const auto& removeKey : removeKeyList)
            {
                Q_EMIT navigationNodeRemoved(QtNavigationType::CategoryNode, removeKey);
            }
        }
        else
        {
            QStringList removeKeyList = d->navigationModel->removeNavigationNode(nodeKey);
            d->initNodeModelIndex(QModelIndex());
            for(const auto& removeKey : removeKeyList)
            {
                d->pageMetaMap.remove(removeKey);
                d->pageNewWindowCountMap.remove(removeKey);
                Q_EMIT navigationNodeRemoved(QtNavigationType::PageNode, removeKey);
            }
        }
    }
    for(int i = 0; i < d->suggestDataList.count(); i++)
    {
        auto& suggestData = d->suggestDataList[i];
        if(suggestData.suggestData().value("QtPageKey").toString() == nodeKey)
        {
            d->suggestDataList.removeAt(i);
            break;
        }
    }
}

void QtNavigationBar::navigation(QString pageKey, bool isLogClicked, bool isRouteBack)
{
    Q_D(QtNavigationBar);
    QtNavigationNode* node = d->navigationModel->navigationNode(pageKey);
    if(!node)
    {
        node = d->footerModel->navigationNode(pageKey);
    }
    if(node)
    {
        if(node->isFooterNode())
        {
            d->onFooterViewClicked(node->modelIndex(), isLogClicked, isRouteBack);
        }
        else
        {
            if(!node->isExpanderNode())
            {
                d->onTreeViewClicked(node->modelIndex(), isLogClicked, isRouteBack);
                d->smoothScrollNavigationView(node->modelIndex());
            }
        }
    }
}

QtNavigationType::NavigationDisplayMode QtNavigationBar::currentDisplayMode()
{
    Q_D(QtNavigationBar);
    return d->currentDisplayMode;
}

bool QtNavigationBar::isTransparent() const
{
    Q_D(const QtNavigationBar);
    return d->isTransparent;
}
void QtNavigationBar::setIsTransparent(bool value)
{
    Q_D(QtNavigationBar);
    d->isTransparent = value;
}

bool QtNavigationBar::isAllowPageOpenInNewWindow() const
{
    Q_D(const QtNavigationBar);
    return d->isAllowPageOpenInNewWindow;
}
void QtNavigationBar::setIsAllowPageOpenInNewWindow(bool value)
{
    Q_D(QtNavigationBar);
    d->isAllowPageOpenInNewWindow = value;
}

int QtNavigationBar::navigationBarWidth() const
{
    Q_D(const QtNavigationBar);
    return d->navigationBarWidth;
}
void QtNavigationBar::setNavigationBarWidth(int value)
{
    Q_D(QtNavigationBar);
    d->navigationBarWidth = value;
}

void QtNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(QtNavigationBar);
    if(!d->isTransparent)
    {
        QPainter painter(this);
        painter.save();
        painter.setPen(QtThemeColor(d->themeMode, PopupBorder));
        painter.setBrush(QtThemeColor(d->themeMode, PopupBase));
        QRect baseRect = this->rect();
        baseRect.adjust(-1, 0, -1, 0);
        QPainterPath path;
        path.moveTo(baseRect.topLeft());
        path.lineTo(QPoint(baseRect.right() - 8, baseRect.y()));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.y(), 16, 16), 90, -90);
        path.lineTo(QPoint(baseRect.right(), baseRect.bottom() - 8));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.bottom() - 16, 16, 16), 0, -90);
        path.lineTo(baseRect.bottomLeft());
        path.closeSubpath();
        painter.drawPath(path);
        painter.restore();
    }
    QWidget::paintEvent(event);
}
