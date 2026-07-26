#include "qtwindow.h"
#include "command/qtwindowstackchangecommand.h"
#include "qtapplication.h"
#include "qtcentralstackedwidget.h"
#include "qtdef.h"
#include "qtevent.h"
#include "qtmenu.h"
#include "qtnavigationbar.h"
#include "qttheme.h"
#include "qtthemeanimationwidget.h"
#include "qtwindow_p.h"
#include "styles/qtwindowstyle.h"
#include <qtmetamacros.h>
#include <qwidget.h>

QtWindowPrivate::QtWindowPrivate(QtWindow* q)
    : q_ptr(q)
{
}
QtWindowPrivate::~QtWindowPrivate()
{
}

qreal QtWindowPrivate::distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) +
                     (point1.y() - point2.y()) * (point1.y() - point2.y()));
}
void QtWindowPrivate::resetWindowLayout(bool isAnimation)
{
    if(isAnimation)
    {
        while(centerLayout->count() > 0)
        {
            centerLayout->takeAt(0);
        }
    }
    else
    {
        if(centerLayout->count() == 0)
        {
            // navigationBar->setIsTransparent(true);
            // _navigationBar->setDisplayMode(ElaNavigationType::Minimal, false);
            // _centerLayout->addWidget(_navigationBar);
            // _centerLayout->addWidget(_navigationCenterStackedWidget);
        }
    }
}
void QtWindowPrivate::doNavigationDisplayModeChange()
{
}

void QtWindowPrivate::onNavigationButtonClicked()
{
}
void QtWindowPrivate::onWMWindowClickedEvent(QVariantMap data)
{
}
void QtWindowPrivate::onThemeReadyChange()
{
    Q_Q(QtWindow);
    switch(qtApp->windowDisplayMode())
    {
        case QtApplicationType::Normal:
        case QtApplicationType::QtMica: {
            appBar->setIsOnlyAllowMinAndClose(true);
            if(!animationWidget)
            {
                QPoint centerPos = q->mapFromGlobal(QCursor::pos());
                animationWidget = new QtThemeAnimationWidget(q);
                QObject::connect(
                    animationWidget, &QtThemeAnimationWidget::animationFinished, q, [this]() {
                        appBar->setIsOnlyAllowMinAndClose(false);
                        animationWidget = nullptr;
                    });
                animationWidget->move(0, 0);
                animationWidget->setOldWindowBackground(q->grab(q->rect()).toImage());
                if(qtTheme->getThemeMode() == QtThemeType::Light)
                {
                    qtTheme->setThemeMode(QtThemeType::Dark);
                }
                else
                {
                    qtTheme->setThemeMode(QtThemeType::Light);
                }

                animationWidget->setCenter(centerPos);
                qreal topLeftDis = distance(centerPos, QPoint(0, 0));
                qreal topRightDis = distance(centerPos, QPoint(q->width(), 0));
                qreal bottomLeftDis = distance(centerPos, QPoint(0, q->height()));
                qreal bottomRightDis = distance(centerPos, QPoint(q->width(), q->height()));
                QList<qreal> disList{ topLeftDis, topRightDis, bottomLeftDis, bottomRightDis };
                std::sort(disList.begin(), disList.end());
                animationWidget->setEndRadius(disList[3]);
                animationWidget->resize(q->width(), q->height());
                animationWidget->startAnimation(themeChangeTime);
            }
            break;
        }
        default: {
            if(qtTheme->getThemeMode() == QtThemeType::Light)
            {
                qtTheme->setThemeMode(QtThemeType::Dark);
            }
            else
            {
                qtTheme->setThemeMode(QtThemeType::Light);
            }
            break;
        }
    }
}
void QtWindowPrivate::onThemeModeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtWindow);
    this->themeMode = themeMode;
    if(windowPaintMode == QtWindowType::PaintMode::Movie)
    {
        if(windowPaintMovie->state() == QMovie::Running)
        {
            windowPaintMovie->stop();
        }
        windowPaintMovie->setFileName(
            themeMode == QtThemeType::Light ? lightWindowMoviePath : darkWindowMoviePath);
        windowPaintMovie->start();
    }
    q->update();
}
void QtWindowPrivate::onWindowDisplayModeChanged()
{
}
void QtWindowPrivate::onNavigationNodeClicked(
    QtNavigationType::NavigationNodeType nodeType, QString nodeKey, bool isRouteBack)
{
}
void QtWindowPrivate::onNavigationNodeAdded(
    QtNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page)
{
}
void QtWindowPrivate::onNavigationNodeRemoved(
    QtNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
}
void QtWindowPrivate::onNavigationRouterStateChanged(
    const QString& domainName, QtActionCommanderType::CommanderState state)
{
}

Q_TAKEOVER_NATIVEEVENT_CPP(QtWindow, d_func()->appBar);
QtWindow::QtWindow(QWidget* parent)
    : QMainWindow{ parent }
    , d_ptr(new QtWindowPrivate(this))
{
    Q_D(QtWindow);
    d->stackSwitchMode = QtWindowType::StackSwitchMode::Popup;
    setProperty("QtBaseClassName", "QtWindow");
    resize(1020, 680);
    d->themeChangeTime = 700;
    d->navigationBarDisplayMode = QtNavigationType::NavigationDisplayMode::Auto;
    d->appBar = new QtAppBar(this);
    d->appBar->setWindowButtonFlag(QtAppBarType::NavigationButtonHint);
    connect(d->appBar, &QtAppBar::routeBackButtonClicked, this,
        []() { QtActionCommander::getInstance()->undoCommand("QtWidgetsExtraAction"); });
    connect(d->appBar, &QtAppBar::routeForwardButtonClicked, this,
        []() { QtActionCommander::getInstance()->redoCommand("QtWidgetsExtraAction"); });
    connect(d->appBar, &QtAppBar::closeButtonClicked, this, &QtWindow::closeButtonClicked);
    d->navigationBar = new QtNavigationBar(this);
    connect(QtActionCommander::getInstance(), &QtActionCommander::commanderStateChanged, this,
        [this](const QString& domainName, QtActionCommanderType::CommanderState state) {
            d_ptr->onNavigationRouterStateChanged(domainName, state);
        });

    // connect(d->navigationBar, &QtNavigationBar::userInfoCardClicked, this,
    //     &ElaWindow::userInfoCardClicked);
    //     // 转发点击信号
    //     connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, this,
    //         &ElaWindow::navigationNodeClicked);
    //     // 跳转处理
    //     connect(d->_navigationBar, &ElaNavigationBar::navigationNodeClicked, d,
    //         &ElaWindowPrivate::onNavigationNodeClicked);
    //     // 新增窗口
    //     connect(d->_navigationBar, &ElaNavigationBar::navigationNodeAdded, d,
    //         &ElaWindowPrivate::onNavigationNodeAdded);
    //     // 移除窗口
    //     connect(d->_navigationBar, &ElaNavigationBar::navigationNodeRemoved, d,
    //         &ElaWindowPrivate::onNavigationNodeRemoved);
    //     // 在新窗口打开
    //     connect(d->_navigationBar, &ElaNavigationBar::pageOpenInNewWindow, this,
    //         &ElaWindow::pageOpenInNewWindow);

    d->navigationCenterStackedWidget = new QtCentralStackedWidget(this);
    d->navigationCenterStackedWidget->setContentsMargins(0, 0, 0, 0);
    QWidget* navigationCentralWidget = new QWidget(this);
    navigationCentralWidget->setObjectName("QtWindowNavigationCentralWidget");
    navigationCentralWidget->setStyleSheet(
        "#QtWindowNavigationCentralWidget{background-color:transparent;}");
    navigationCentralWidget->installEventFilter(this);
    d->centerLayout = new QHBoxLayout(navigationCentralWidget);
    d->centerLayout->setSpacing(5);
    d->centerLayout->addWidget(d->navigationBar);
    d->centerLayout->addWidget(d->navigationCenterStackedWidget);
    d->centerLayout->setContentsMargins(d->contentsMargins, 0, 0, 0);

    //     // 事件总线
    //     d->_focusEvent = new ElaEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    //     d->_focusEvent->registerAndInit();

    //     // 导航栏操作
    //     connect(d->_appBar, &ElaAppBar::navigationButtonClicked, d,
    //         &ElaWindowPrivate::onNavigationButtonClicked);

    //     // 主题变更动画
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeModeChanged(themeMode); });
    connect(d->appBar, &QtAppBar::themeChangeButtonClicked, this,
        [this]() { d_ptr->onThemeReadyChange(); });
    d->isInitFinished = true;

    d->centerStackedWidget = new QtCentralStackedWidget(this);
    d->centerStackedWidget->setIsTransparent(true);
    d->centerStackedWidget->containerStackedWidget()->addWidget(navigationCentralWidget);
    setCentralWidget(d->centerStackedWidget);
    setObjectName("QtWindow");
    setStyleSheet("#QtWindow{background-color:transparent;}");
    setStyle(new QtWindowStyle(style()));

    qtApp->syncWindowDisplayMode(this);
    d->windowDisplayMode = qtApp->windowDisplayMode();
    //     connect(eApp, &ElaApplication::pWindowDisplayModeChanged, d,
    //         &ElaWindowPrivate::onWindowDisplayModeChanged);

    //     d->_pWindowPaintMode = ElaWindowType::PaintMode::Normal;
    //     d->_lightWindowPix = new QPixmap();
    //     d->_darkWindowPix = new QPixmap();

    //     d->_windowPaintMovie = new QMovie(this);
    //     connect(d->_windowPaintMovie, &QMovie::frameChanged, this, [=]() { update(); });
}

QtWindow::~QtWindow()
{
}

void QtWindow::moveToCenter()
{
    if(isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2,
        (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void QtWindow::setCustomWidget(QtAppBarType::CustomArea customArea, QWidget* widget,
    QObject* hitTestObject, const QString& hitTestFunctionName)
{
    Q_D(QtWindow);
    d->appBar->setCustomWidget(customArea, widget, hitTestObject, hitTestFunctionName);
    Q_EMIT customWidgetChanged();
}

QWidget* QtWindow::customWidget(QtAppBarType::CustomArea customArea) const
{
    Q_D(const QtWindow);
    return d->appBar->customWidget(customArea);
}

void QtWindow::setCentralCustomWidget(QWidget* customWidget)
{
    Q_D(QtWindow);
    d->navigationCenterStackedWidget->setCustomWidget(customWidget);
    Q_EMIT centralCustomWidgetChanged();
}
QWidget* QtWindow::centralCustomWidget() const
{
    Q_D(const QtWindow);
    return d->navigationCenterStackedWidget->customWidget();
}

void QtWindow::setCustomMenu(QMenu* customMenu)
{
    Q_D(const QtWindow);
    d->appBar->setCustomMenu(customMenu);
    Q_EMIT customMenuChanged();
}
QMenu* QtWindow::customMenu() const
{
    Q_D(const QtWindow);
    return d->appBar->customMenu();
}

bool QtWindow::isStayTop() const
{
    Q_D(const QtWindow);
    return d->isStayTop;
}
void QtWindow::setIsStayTop(bool value)
{
    Q_D(QtWindow);
    d->isStayTop = value;
    Q_EMIT isStayTopChanged();
}

bool QtWindow::isFixedSize() const
{
    Q_D(const QtWindow);
    return d->isFixedSize;
}
void QtWindow::setIsFixedSize(bool value)
{
    Q_D(QtWindow);
    d->isFixedSize = value;
    Q_EMIT isFixedSizeChanged();
}

bool QtWindow::isDefaultClosed() const
{
    Q_D(const QtWindow);
    return d->isDefaultClosed;
}
void QtWindow::setIsDefaultClosed(bool value)
{
    Q_D(QtWindow);
    d->isDefaultClosed = value;
    Q_EMIT isDefaultClosedChanged();
}

int QtWindow::appBarHeight() const
{
    Q_D(const QtWindow);
    return d->appBarHeight;
}
void QtWindow::setAppBarHeight(int value)
{
    Q_D(QtWindow);
    d->appBarHeight = value;
    Q_EMIT appBarHeightChanged();
}

int QtWindow::themeChangeTime() const
{
    Q_D(const QtWindow);
    return d->themeChangeTime;
}
void QtWindow::setThemeChangeTime(int value)
{
    Q_D(QtWindow);
    d->themeChangeTime = value;
    Q_EMIT themeChangeTimeChanged();
}

bool QtWindow::isCentralStackedWidgetTransparent() const
{
    Q_D(const QtWindow);
    return d->navigationCenterStackedWidget->isTransparent();
}
void QtWindow::setIsCentralStackedWidgetTransparent(bool value)
{
    Q_D(QtWindow);
    d->navigationCenterStackedWidget->setIsTransparent(value);
    Q_EMIT isCentralStackedWidgetTransparentChanged();
}

bool QtWindow::isAllowPageOpenInNewWindow() const
{
    Q_D(const QtWindow);
    return d->navigationBar->isAllowPageOpenInNewWindow();
}
void QtWindow::setIsAllowPageOpenInNewWindow(bool value)
{
    Q_D(QtWindow);
    d->navigationBar->setIsAllowPageOpenInNewWindow(value);
    Q_EMIT isAllowPageOpenInNewWindowChanged();
}

bool QtWindow::isNavigationBarEnable() const
{
    Q_D(const QtWindow);
    return d->isNavigationBarEnable;
}
void QtWindow::setIsNavigationBarEnable(bool value)
{
    Q_D(QtWindow);
    d->isNavigationBarEnable = value;
    d->navigationBar->setVisible(value);
    d->centerLayout->setContentsMargins(value ? d->contentsMargins : 0, 0, 0, 0);
    d->navigationCenterStackedWidget->setIsHasRadius(value);
    Q_EMIT isNavigationBarEnableChanged();
}

int QtWindow::navigationBarWidth() const
{
    Q_D(const QtWindow);
    return d->navigationBar->navigationBarWidth();
}
void QtWindow::setNavigationBarWidth(int value)
{
    Q_D(QtWindow);
    d->navigationBar->setNavigationBarWidth(value);
    Q_EMIT navigationBarWidthChanged();
}

int QtWindow::currentStackIndex() const
{
    Q_D(const QtWindow);
    return d->centerStackedWidget->containerStackedWidget()->currentIndex();
}
void QtWindow::setCurrentStackIndex(int value)
{
    Q_D(QtWindow);
    if(value >= d->centerStackedWidget->containerStackedWidget()->count() || value < 0 ||
        value == d->centerStackedWidget->lastTargetIndex())
    {
        return;
    }
    int currentCenterStackedWidgetIndex =
        d->centerStackedWidget->containerStackedWidget()->currentIndex();
    auto command = new QtWindowStackChangeCommand(this);
    command->setWindowPrivate(d);
    command->setUndoStackIndex(currentCenterStackedWidgetIndex);
    command->setRedoStackIndex(value);
    QtActionCommander::getInstance()->recordCommand("QtWidgetsExtraAction", command);
    Q_EMIT currentStackIndexChanged();
}

QtNavigationType::NavigationDisplayMode QtWindow::navigationBarDisplayMode() const
{
    Q_D(const QtWindow);
    return d->navigationBarDisplayMode;
}
void QtWindow::setNavigationBarDisplayMode(QtNavigationType::NavigationDisplayMode value)
{
    Q_D(QtWindow);
    // d->navigationBarDisplayMode = value;
    // d->currentNavigationBarDisplayMode = d->navigationBarDisplayMode;
    // bool isVisible = this->isVisible();
    // switch(d->navigationBarDisplayMode)
    // {
    //     case QtNavigationType::Auto: {
    //         d->doNavigationDisplayModeChange();
    //         break;
    //     }
    //     case QtNavigationType::Minimal: {
    //         d->navigationBar->setDisplayMode(QtNavigationType::Minimal, isVisible);
    //         break;
    //     }
    //     case QtNavigationType::Compact: {
    //         d->navigationBar->setDisplayMode(QtNavigationType::Compact, isVisible);
    //         break;
    //     }
    //     case QtNavigationType::Maximal: {
    //         d->navigationBar->setDisplayMode(QtNavigationType::Maximal, isVisible);
    //         break;
    //     }
    // }
    // Q_EMIT navigationBarDisplayModeChanged();
}

QtWindowType::StackSwitchMode QtWindow::stackSwitchMode() const
{
    Q_D(const QtWindow);
    return d->stackSwitchMode;
}
void QtWindow::setStackSwitchMode(const QtWindowType::StackSwitchMode& value)
{
    Q_D(QtWindow);
    d->stackSwitchMode = value;
    Q_EMIT stackSwitchModeChanged();
}

QtWindowType::PaintMode QtWindow::windowPaintMode() const
{
    Q_D(const QtWindow);
    return d->windowPaintMode;
}
void QtWindow::setWindowPaintMode(QtWindowType::PaintMode value)
{
    Q_D(QtWindow);
    d->windowPaintMode = value;
    Q_EMIT windowPaintModeChanged();
}

bool QtWindow::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtWindow);
    return QMainWindow::eventFilter(watched, event);
}
QMenu* QtWindow::createPopupMenu()
{
    Q_D(QtWindow);
    QtMenu* menu = nullptr;
    return menu;
}
void QtWindow::paintEvent(QPaintEvent* event)
{
    Q_D(QtWindow);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    switch(d->windowDisplayMode)
    {
        case QtApplicationType::Normal: {
            switch(d->windowPaintMode)
            {
                case QtWindowType::Normal: {
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QtThemeColor(d->themeMode, WindowBase));
                    painter.drawRect(rect());
                    break;
                }
                case QtWindowType::Pixmap: {
                    QPixmap* pix =
                        d->themeMode == QtThemeType::Light ? d->lightWindowPix : d->darkWindowPix;
                    qreal windowAspectRatio = (qreal) rect().width() / rect().height();
                    qreal pixAspectRatio = (qreal) pix->width() / pix->height();
                    int targetPixWidth, targetPixHeight;
                    if(windowAspectRatio < pixAspectRatio)
                    {
                        targetPixWidth = qRound(pix->width() * windowAspectRatio / pixAspectRatio);
                        targetPixHeight = pix->height();
                    }
                    else
                    {
                        targetPixWidth = pix->width();
                        targetPixHeight =
                            qRound(pix->height() * pixAspectRatio / windowAspectRatio);
                    }
                    painter.drawPixmap(rect(), *pix,
                        QRect((pix->width() - targetPixWidth) / 2,
                            (pix->height() - targetPixHeight) / 2, targetPixWidth,
                            targetPixHeight));
                    break;
                }
                case QtWindowType::Movie: {
                    QPixmap pix = d->windowPaintMovie->currentPixmap();
                    qreal windowAspectRatio = (qreal) rect().width() / rect().height();
                    qreal pixAspectRatio = (qreal) pix.width() / pix.height();
                    int targetPixWidth, targetPixHeight;
                    if(windowAspectRatio < pixAspectRatio)
                    {
                        targetPixWidth = qRound(pix.width() * windowAspectRatio / pixAspectRatio);
                        targetPixHeight = pix.height();
                    }
                    else
                    {
                        targetPixWidth = pix.width();
                        targetPixHeight = qRound(pix.height() * pixAspectRatio / windowAspectRatio);
                    }
                    painter.drawPixmap(rect(), pix,
                        QRect((pix.width() - targetPixWidth) / 2,
                            (pix.height() - targetPixHeight) / 2, targetPixWidth, targetPixHeight));
                    break;
                }
                default: {
                    break;
                }
            }
        }
        default: {
            break;
        }
    }
    painter.restore();
}