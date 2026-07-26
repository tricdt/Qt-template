#pragma once

#include "qtdef.h"
#include "qtsuggestbox.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtNavigationBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged)
    Q_PROPERTY(bool isAllowPageOpenInNewWindow READ isAllowPageOpenInNewWindow WRITE
            setIsAllowPageOpenInNewWindow NOTIFY isAllowPageOpenInNewWindowChanged)
    Q_PROPERTY(bool navigationBarWidth READ navigationBarWidth WRITE setNavigationBarWidth NOTIFY
            navigationBarWidthChanged)
public:
    explicit QtNavigationBar(QWidget* parent = nullptr);
    ~QtNavigationBar();

    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);

    QtNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString&
    expanderKey,
        QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addExpanderNode(const QString& expanderTitle, QString&
    expanderKey,
        const QString& targetExpanderKey, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(
        const QString& pageTitle, QWidget* page, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page, int keyPoints,
        QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page,
        const QString& targetExpanderKey, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page,
        const QString& targetExpanderKey, int keyPoints,
        QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey,
        int keyPoints = 0, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addFooterNode(const QString& footerTitle, QWidget* page,
        QString& footerKey, int keyPoints = 0, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addCategoryNode(
        const QString& categoryTitle, QString& categoryKey);
    QtNavigationType::NodeResult addCategoryNode(
        const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey);

    bool nodeIsExpanded(QString expanderKey) const;
    void expandNode(QString expanderKey);
    void collapseNode(QString expanderKey);
    void removeNode(QString nodeKey);

    // void setNodeKeyPoints(QString nodeKey, int keyPoints);
    // int nodeKeyPoints(QString nodeKey) const;

    // void setNodeTitle(QString nodeKey, QString nodeTitle);
    // QString nodeTitle(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true, bool isRouteBack = false);
    // void setDisplayMode(
    //     QtNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);
    // QtNavigationType::NavigationDisplayMode displayMode() const;

    // int pageOpenInNewWindowCount(QString nodeKey) const;

    // QList<QtSuggestBox::SuggestData> suggestDataList() const;

    QtNavigationType::NavigationDisplayMode currentDisplayMode();

    bool isTransparent() const;
    void setIsTransparent(bool value);

    bool isAllowPageOpenInNewWindow() const;
    void setIsAllowPageOpenInNewWindow(bool value);

    int navigationBarWidth() const;
    void setNavigationBarWidth(int value);

Q_SIGNALS:
    void userInfoCardClicked();
    void navigationNodeClicked(
        QtNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack);
    void navigationNodeAdded(
        QtNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void navigationNodeRemoved(QtNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void isTransparentChanged();
    void isAllowPageOpenInNewWindowChanged();
    void navigationBarWidthChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtNavigationBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationBar)
};
