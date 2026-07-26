#pragma once

#include "qtappbar.h"
#include "qtwidgetsextra.h"
#include "widgets/qtappbar.h"
#include <QWidget>
#include <qmainwindow.h>
#include "qtsuggestbox.h"
class QtWindowPrivate;
class QTWIDGETSEXTRA_EXPORT QtWindow : public QMainWindow
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
    Q_PROPERTY(bool isStayTop READ isStayTop WRITE setIsStayTop NOTIFY isStayTopChanged)
    Q_PROPERTY(bool isFixedSize READ isFixedSize WRITE setIsFixedSize NOTIFY isFixedSizeChanged)
    Q_PROPERTY(bool isDefaultClosed READ isDefaultClosed WRITE setIsDefaultClosed NOTIFY
            isDefaultClosedChanged)
    Q_PROPERTY(int appBarHeight READ appBarHeight WRITE setAppBarHeight NOTIFY appBarHeightChanged)
    Q_PROPERTY(int themeChangeTime READ themeChangeTime WRITE setThemeChangeTime NOTIFY
            themeChangeTimeChanged)
    Q_PROPERTY(bool isCentralStackedWidgetTransparent READ isCentralStackedWidgetTransparent WRITE
            setIsCentralStackedWidgetTransparent NOTIFY isCentralStackedWidgetTransparentChanged)
    Q_PROPERTY(bool isAllowPageOpenInNewWindow READ isAllowPageOpenInNewWindow WRITE
            setIsAllowPageOpenInNewWindow NOTIFY isAllowPageOpenInNewWindowChanged)
    Q_PROPERTY(bool isNavigationBarEnable READ isNavigationBarEnable WRITE setIsNavigationBarEnable
            NOTIFY isNavigationBarEnableChanged)
    Q_PROPERTY(int navigationBarWidth READ navigationBarWidth WRITE setNavigationBarWidth NOTIFY
            navigationBarWidthChanged)
    Q_PROPERTY(int currentStackIndex READ currentStackIndex WRITE setCurrentStackIndex NOTIFY
            currentStackIndexChanged)
    Q_PROPERTY(QtNavigationType::NavigationDisplayMode navigationBarDisplayMode READ
            navigationBarDisplayMode WRITE setNavigationBarDisplayMode NOTIFY
                navigationBarDisplayModeChanged)
    Q_PROPERTY(QtWindowType::StackSwitchMode stackSwitchMode READ stackSwitchMode WRITE
            setStackSwitchMode NOTIFY stackSwitchModeChanged)

    Q_PROPERTY(QtWindowType::PaintMode windowPaintMode READ windowPaintMode WRITE setWindowPaintMode
            NOTIFY windowPaintModeChanged)
public:
    explicit QtWindow(QWidget* parent = nullptr);
    ~QtWindow();

    void moveToCenter();

    void setCustomWidget(QtAppBarType::CustomArea customArea, QWidget* customWidget,
        QObject* hitTestObject = nullptr, const QString& hitTestFunctionName = "");
    QWidget* customWidget(QtAppBarType::CustomArea customArea) const;

    void setCentralCustomWidget(QWidget* customWidget);
    QWidget* centralCustomWidget() const;

    void setCustomMenu(QMenu* customMenu);
    QMenu* customMenu() const;

    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);

    QtNavigationType::NodeResult addExpanderNode(const QString& expanderTitle,
        QString& expanderKey, QtIconType::IconName awesome = QtIconType::None) const;
    QtNavigationType::NodeResult addExpanderNode(const QString& expanderTitle,
        QString& expanderKey, const QString& targetExpanderKey,
        QtIconType::IconName awesome = QtIconType::None) const;
    QtNavigationType::NodeResult addPageNode(
        const QString& pageTitle, QWidget* page, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page,
        int keyPoints, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page,
        const QString& targetExpanderKey, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addPageNode(const QString& pageTitle, QWidget* page,
        const QString& targetExpanderKey, int keyPoints,
        QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addFooterNode(const QString& footerTitle, QString& footerKey,
        int keyPoints = 0, QtIconType::IconName awesome = QtIconType::None) const;
    QtNavigationType::NodeResult addFooterNode(const QString& footerTitle, QWidget* page,
        QString& footerKey, int keyPoints = 0, QtIconType::IconName awesome = QtIconType::None);
    QtNavigationType::NodeResult addCategoryNode(
        const QString& categoryTitle, QString& categoryKey);
    QtNavigationType::NodeResult addCategoryNode(
        const QString& categoryTitle, QString& categoryKey, const QString& targetExpanderKey);

//     void addCentralWidget(QWidget* centralWidget);
//     QWidget* getCentralWidget(int index) const;

//     bool navigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
//     void collapseNavigationNode(QString expanderKey);
//     void removeNavigationNode(QString nodeKey) const;
//     int pageOpenInNewWindowCount(QString nodeKey) const;
    void backtrackNavigationNode(QString nodeKey);

//     void setNodeKeyPoints(QString nodeKey, int keyPoints);
//     int nodeKeyPoints(QString nodeKey) const;

//     void setNavigationNodeTitle(QString nodeKey, QString nodeTitle);
//     QString navigationNodeTitle(QString nodeKey) const;

//     void navigation(QString pageKey);
//     int currentNavigationIndex() const;
//     QString currentNavigationPageKey() const;

//     QList<QtSuggestBox::SuggestData> navigationSuggestDataList() const;

//     void setWindowButtonFlag(QtAppBarType::ButtonType buttonFlag, bool isEnable = true);
//     void setWindowButtonFlags(QtAppBarType::ButtonFlags buttonFlags);
//     QtAppBarType::ButtonFlags windowButtonFlags() const;

//     void setWindowMoviePath(QtThemeType::ThemeMode themeMode, const QString& moviePath);
//     QString windowMoviePath(QtThemeType::ThemeMode themeMode) const;

//     void setWindowPixmap(QtThemeType::ThemeMode themeMode, const QPixmap& pixmap);
//     QPixmap windowPixmap(QtThemeType::ThemeMode themeMode) const;

//     void setWindowMovieRate(qreal rate);
//     qreal windowMovieRate() const;

//     void closeWindow();

    bool isStayTop() const;
    void setIsStayTop(bool value);

    bool isFixedSize() const;
    void setIsFixedSize(bool value);

    bool isDefaultClosed() const;
    void setIsDefaultClosed(bool value);

    int appBarHeight() const;
    void setAppBarHeight(int value);

    int themeChangeTime() const;
    void setThemeChangeTime(int value);

    bool isCentralStackedWidgetTransparent() const;
    void setIsCentralStackedWidgetTransparent(bool value);

    bool isAllowPageOpenInNewWindow() const;
    void setIsAllowPageOpenInNewWindow(bool value);

    bool isNavigationBarEnable() const;
    void setIsNavigationBarEnable(bool value);

    int navigationBarWidth() const;
    void setNavigationBarWidth(int value);

    int currentStackIndex() const;
    void setCurrentStackIndex(int value);

    QtNavigationType::NavigationDisplayMode navigationBarDisplayMode() const;
    void setNavigationBarDisplayMode(QtNavigationType::NavigationDisplayMode value);

    QtWindowType::StackSwitchMode stackSwitchMode() const;
    void setStackSwitchMode(const QtWindowType::StackSwitchMode& value);

    QtWindowType::PaintMode windowPaintMode() const;
    void setWindowPaintMode(QtWindowType::PaintMode value);

Q_SIGNALS:
    void closeButtonClicked();
    void customWidgetChanged();
    void centralCustomWidgetChanged();
    void customMenuChanged();

    void isStayTopChanged();
    void isFixedSizeChanged();
    void isDefaultClosedChanged();
    void appBarHeightChanged();
    void themeChangeTimeChanged();
    void isCentralStackedWidgetTransparentChanged();
    void isAllowPageOpenInNewWindowChanged();
    void isNavigationBarEnableChanged();
    void navigationBarWidthChanged();
    void currentStackIndexChanged();
    void navigationBarDisplayModeChanged();
    void stackSwitchModeChanged();
    void windowPaintModeChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWindow)
};
