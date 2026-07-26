#pragma once
#include "qtcentralstackedwidget.h"
#include "qtevent.h"
#include "qtnavigationbar.h"
#include "qtthemeanimationwidget.h"
#include "widgets/qtappbar.h"
#include <QWidget>
#include <qmainwindow.h>
#include "qtwindow.h"
class QtWindowPrivate
{
    Q_DECLARE_PUBLIC(QtWindow)
public:
    int themeChangeTime;
    QtNavigationType::NavigationDisplayMode navigationBarDisplayMode;
    QtWindowType::StackSwitchMode stackSwitchMode;
    QtWindowType::PaintMode windowPaintMode;
    bool isStayTop;
    bool isFixedSize;
    bool isDefaultClosed;
    int appBarHeight;
    bool isNavigationBarEnable;
    QtWindow* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtApplicationType::WindowDisplayMode windowDisplayMode;
    QMovie* windowPaintMovie{ nullptr };
    QString lightWindowMoviePath{ "" };
    QString darkWindowMoviePath{ "" };
    QPixmap* lightWindowPix;
    QPixmap* darkWindowPix;

    bool isWindowClosing{ false };

    bool isInitFinished{ false };
    QtEvent* focusEvent{ nullptr };
    QtNavigationBar* navigationBar{ nullptr };
    QtCentralStackedWidget* centerStackedWidget{ nullptr };
    QtCentralStackedWidget* navigationCenterStackedWidget{ nullptr };
    QtAppBar* appBar{ nullptr };
    QHBoxLayout* centerLayout{ nullptr };
    int contentsMargins{ 5 };
    bool isNavigationDisplayModeChanged{ false };
    bool isNavigationEnable{ true };
    bool isNavigationBarExpanded{ false };
    bool isNavigationBarFloat{ false };
    QtThemeAnimationWidget* animationWidget{ nullptr };

    QtNavigationType::NavigationDisplayMode currentNavigationBarDisplayMode{
        QtNavigationType::Maximal
    };
    QMap<QString, const QMetaObject*> pageMetaMap;
    QMap<QString, QWidget*> routeMap;
    int navigationTargetIndex{ 0 };
    QtWindowPrivate(QtWindow* q);
    ~QtWindowPrivate();

    void onNavigationButtonClicked();
    void onWMWindowClickedEvent(QVariantMap data);
    void onThemeReadyChange();
    void onThemeModeChanged(QtThemeType::ThemeMode themeMode);
    void onWindowDisplayModeChanged();
    void onNavigationNodeClicked(
        QtNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack);
    void onNavigationNodeAdded(
        QtNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    void onNavigationNodeRemoved(QtNavigationType::NavigationNodeType nodeType, QString nodeKey);
    void onNavigationRouterStateChanged(
        const QString& domainName, QtActionCommanderType::CommanderState state);

    qreal distance(QPoint point1, QPoint point2);
    void resetWindowLayout(bool isAnimation);
    void doNavigationDisplayModeChange();
};