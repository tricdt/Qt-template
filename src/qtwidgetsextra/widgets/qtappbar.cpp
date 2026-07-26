#include "qtappbar.h"

#include <QGuiApplication>
#include <QLabel>
#include <QMenu>
#include <QWindow>
#include "widgets/qticonbutton.h"
#include "widgets/qttext.h"
#include "qttheme.h"
#include "widgets/qttoolbutton.h"
#include "utils/qtwinshadowhelper.h"
#include <qobject.h>
#include "qtevent.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif
class QtAppBarPrivate
{
    Q_DECLARE_PUBLIC(QtAppBar)
public:
    QtAppBar* q_ptr;
    bool isStayTop;
    bool isFixedSize;
    bool isDefaultClosed;
    bool isOnlyAllowMinAndClose;
    int appBarHeight;
    QMenu* customMenu;

    QtThemeType::ThemeMode themeMode;
    QHBoxLayout* mainLayout{ nullptr };
    QVBoxLayout* iconLabelLayout{ nullptr };
    QVBoxLayout* titleLabelLayout{ nullptr };
    QtAppBarType::ButtonFlags buttonFlags;
    QtToolButton* routeBackButton{ nullptr };
    QtToolButton* routeForwardButton{ nullptr };
    QtToolButton* navigationButton{ nullptr };
    QtToolButton* themeChangeButton{ nullptr };
    QtToolButton* stayTopButton{ nullptr };
    QtToolButton* minButton{ nullptr };
    QtToolButton* maxButton{ nullptr };
    QtIconButton* closeButton{ nullptr };
    QList<QWidget*> customAreaWidgetList{ nullptr, nullptr, nullptr };
    QList<QObject*> customAreaHitTestObjectList{ nullptr, nullptr, nullptr };
    QStringList customAreaHitTestFunctionNameList{ "", "", "" };
    QList<QWidget*> clientWidgetList;
    QScreen* lastScreen{ nullptr };
    QtText* titleLabel{ nullptr };
    QLabel* iconLabel{ nullptr };
    qint64 currentWinID{ 0 };
    int lastMinTrackWidth{ 0 };
    quint64 clickTimer{ 0 };
    int edges{ 0 };
    int margins{ 8 };
    bool isHoverMaxButton{ false };
    int win7Margins{ 0 };
    QtAppBarPrivate(QtAppBar* q, QWidget* parent = nullptr);
    void changeMaxButtonAwesome(bool isMaximized);
    void showAppBarMenu(QPoint point);
    void updateCursor(int edges);
    bool containsCursorToItem(QWidget* item);
    void onThemeModeChange(QtThemeType::ThemeMode themeMode);
    int calculateMinimumWidth();
    QVBoxLayout* createVLayout(QWidget* widget);
    void onMinButtonClicked();
    void onMaxButtonClicked();
    void onCloseButtonClicked();
    void onStayTopButtonClicked();
};

QtAppBarPrivate::QtAppBarPrivate(QtAppBar* q, QWidget* parent)
    : q_ptr(q)
{
    buttonFlags = QtAppBarType::RouteBackButtonHint | QtAppBarType::RouteForwardButtonHint |
                  QtAppBarType::StayTopButtonHint | QtAppBarType::ThemeChangeButtonHint |
                  QtAppBarType::MinimizeButtonHint | QtAppBarType::MaximizeButtonHint |
                  QtAppBarType::CloseButtonHint;
    q->window()->setAttribute(Qt::WA_Mapped);
    appBarHeight = 45;
    q->setFixedHeight(appBarHeight);
    q->window()->setContentsMargins(0, q->height(), 0, 0);
    isStayTop = false;
    isFixedSize = false;
    isDefaultClosed = true;
    isOnlyAllowMinAndClose = false;
    customMenu = nullptr;
    q->window()->installEventFilter(q);
#ifdef Q_OS_WIN
    if(!qtWinHelper->isWinVersionGreater10())
    {
        win7Margins = 8;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    q->window()->setWindowFlags(
        (q->window()->windowFlags()) | Qt::WindowMinimizeButtonHint | Qt::FramelessWindowHint);
#endif
#else
    q->window()->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::FramelessWindowHint |
                                Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint |
                                Qt::WindowSystemMenuHint | Qt::WindowFullscreenButtonHint |
                                Qt::WindowSystemMenuHint);
#endif
    q->setMouseTracking(true);
    q->setObjectName("QtAppBar");
    q->setStyleSheet("#QtAppBar{background-color:transparent;}");
    routeBackButton = new QtToolButton(q);
    routeBackButton->setQtIcon(QtIconType::ArrowLeft);
    routeBackButton->setFixedSize(35, 30);
    routeBackButton->setEnabled(false);
    QObject::connect(
        routeBackButton, &QtToolButton::clicked, q, [q]() { Q_EMIT q->routeBackButtonClicked(); });

    routeForwardButton = new QtToolButton(q);
    routeForwardButton->setQtIcon(QtIconType::ArrowRight);
    routeForwardButton->setFixedSize(35, 30);
    routeForwardButton->setEnabled(false);
    QObject::connect(routeForwardButton, &QtToolButton::clicked, q,
        [q]() { Q_EMIT q->routeForwardButtonClicked(); });

    navigationButton = new QtToolButton(q);
    navigationButton->setQtIcon(QtIconType::Bars);
    navigationButton->setFixedSize(40, 30);
    navigationButton->setObjectName("NavigationButton");
    navigationButton->setVisible(false);
    QObject::connect(
        navigationButton, &QtToolButton::clicked, q, [q]() { q->navigationButtonClicked(); });

    stayTopButton = new QtToolButton(q);
    stayTopButton->setQtIcon(QtIconType::Thumbtack, 45);
    stayTopButton->setFixedSize(40, 30);
    stayTopButton->setEnabled(true);
    QObject::connect(
        stayTopButton, &QtToolButton::clicked, q, [q]() { q->setIsStayTop(!q->isStayTop()); });
    QObject::connect(q, &QtAppBar::isStayTopChanged, q, [this]() { onStayTopButtonClicked(); });

    iconLabel = new QLabel(q);
    iconLabelLayout = createVLayout(iconLabel);

    if(parent->windowIcon().isNull())
    {
        iconLabel->setVisible(false);
    }
    else
    {
        iconLabel->setPixmap(parent->windowIcon().pixmap(18, 18));
        iconLabelLayout->setContentsMargins(10, 0, 0, 0);
    }
    QObject::connect(parent, &QWidget::windowIconChanged, [this](const QIcon& icon) {
        iconLabel->setPixmap(icon.pixmap(18, 18));
        iconLabel->setVisible(!icon.isNull());
        iconLabelLayout->setContentsMargins(icon.isNull() ? 0 : 10, 0, 0, 0);
    });

    titleLabel = new QtText(q);
    titleLabel->setIsWrapAnyWhere(false);
    titleLabel->setTextPixelSize(18);
    titleLabelLayout = createVLayout(titleLabel);
    if(parent->windowTitle().isEmpty())
    {
        titleLabel->setVisible(false);
    }
    else
    {
        titleLabel->setText(parent->windowTitle());
        titleLabelLayout->setContentsMargins(10, 0, 0, 0);
    }
    QObject::connect(parent, &QWidget::windowTitleChanged, q, [this](const QString& title) {
        titleLabel->setText(title);
        titleLabel->setVisible(!title.isEmpty());
        titleLabelLayout->setContentsMargins(title.isEmpty() ? 0 : 10, 0, 0, 0);
    });

    themeChangeButton = new QtToolButton(q);
    themeChangeButton->setQtIcon(QtIconType::MoonStars);
    themeChangeButton->setFixedSize(40, 30);
    QObject::connect(themeChangeButton, &QtToolButton::clicked, q, [q]() {
        Q_EMIT q->themeChangeButtonClicked();
    });

    QObject::connect(qtTheme, &QtTheme::themeModeChanged, q,
        [this](QtThemeType::ThemeMode themeMode) { onThemeModeChange(themeMode); });

    minButton = new QtToolButton(q);
    minButton->setQtIcon(QtIconType::Dash);
    minButton->setFixedSize(40, 30);
    QObject::connect(minButton, &QtToolButton::clicked, q, [this]() { onMinButtonClicked(); });

    maxButton = new QtToolButton(q);
    maxButton->setQtIcon(QtIconType::Square);
    maxButton->setFixedSize(40, 30);
    QObject::connect(maxButton, &QtToolButton::clicked, q, [this]() { onMaxButtonClicked(); });

    closeButton = new QtIconButton(QtIconType::Xmark, 18, 40, 30, q);
    closeButton->setLightHoverColor(QColor(0xE8, 0x11, 0x23));
    closeButton->setDarkHoverColor(QColor(0xE8, 0x11, 0x23));
    closeButton->setLightHoverIconColor(Qt::white);
    closeButton->setDarkHoverIconColor(Qt::white);
    QObject::connect(closeButton, &QtIconButton::clicked, q, [this]() { onCloseButtonClicked(); });

    mainLayout = new QHBoxLayout(q);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    QHBoxLayout* leftLayout = new QHBoxLayout();
    leftLayout->setSpacing(0);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setAlignment(Qt::AlignLeft);
    leftLayout->addLayout(createVLayout(routeBackButton));
    leftLayout->addLayout(createVLayout(routeForwardButton));
    leftLayout->addLayout(createVLayout(navigationButton));
    leftLayout->addLayout(iconLabelLayout);
    leftLayout->addLayout(titleLabelLayout);
    mainLayout->addLayout(leftLayout);

    auto leftAreaWidget = new QWidget(q);
    leftAreaWidget->setVisible(false);
    auto middleAreaWidget = new QWidget(q);
    middleAreaWidget->setVisible(false);
    auto rightAreaWidget = new QWidget(q);
    rightAreaWidget->setVisible(false);
    customAreaWidgetList[0] = leftAreaWidget;
    customAreaWidgetList[1] = middleAreaWidget;
    customAreaWidgetList[2] = rightAreaWidget;
    mainLayout->addWidget(leftAreaWidget);
    mainLayout->addWidget(middleAreaWidget);
    mainLayout->addWidget(rightAreaWidget);

    QHBoxLayout* rightLayout = new QHBoxLayout();
    rightLayout->setSpacing(0);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setAlignment(Qt::AlignRight);
    rightLayout->addLayout(createVLayout(stayTopButton));
    rightLayout->addLayout(createVLayout(themeChangeButton));
    rightLayout->addLayout(createVLayout(minButton));
    rightLayout->addLayout(createVLayout(maxButton));
    rightLayout->addLayout(createVLayout(closeButton));
    mainLayout->addLayout(rightLayout);

    clientWidgetList.append(routeBackButton);
    clientWidgetList.append(routeForwardButton);
    clientWidgetList.append(navigationButton);
    clientWidgetList.append(stayTopButton);
    clientWidgetList.append(themeChangeButton);
    clientWidgetList.append(minButton);
    clientWidgetList.append(maxButton);
    clientWidgetList.append(closeButton);

#ifdef Q_OS_WIN
    for(int i = 0; i < qApp->screens().count(); i++)
    {
        QObject::connect(qApp->screens().at(i), &QScreen::logicalDotsPerInchChanged, q, [this] {
            if(isFixedSize)
            {
                HWND hwnd = (HWND) (currentWinID);
                SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                    SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
            }
        });
    }
    QObject::connect(qApp, &QApplication::primaryScreenChanged, q, [this]() {
        HWND hwnd = (HWND) (currentWinID);
        ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
            SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
    });
    lastScreen = qApp->screenAt(q->window()->geometry().center());
#endif

    themeMode = qtTheme->getThemeMode();
    QObject::connect(qtTheme, &QtTheme::themeModeChanged, q, [this](QtThemeType::ThemeMode themeMode) {
        this->themeMode = themeMode;
        this->q_ptr->update();
    });
}

void QtAppBarPrivate::changeMaxButtonAwesome(bool isMaximized)
{
    if(isMaximized)
    {
        maxButton->setQtIcon(QtIconType::WindowRestore);
    }
    else
    {
        maxButton->setQtIcon(QtIconType::Square);
    }
}
void QtAppBarPrivate::showAppBarMenu(QPoint point)
{
    Q_Q(const QtAppBar);
    if(customMenu)
    {
        customMenu->exec(point);
    }
    else
    {
#ifdef Q_OS_WIN
        QScreen* screen = qApp->screenAt(QCursor::pos());
        if(!screen)
        {
            screen = QGuiApplication::primaryScreen();
        }
        if(!screen)
        {
            return;
        }
        const QPoint origin = screen->geometry().topLeft();
        auto nativePos =
            QPointF(QPointF(point - origin) * screen->devicePixelRatio()).toPoint() + origin;
        HWND hwnd = reinterpret_cast<HWND>(q->window()->winId());
        const HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
        if(q->window()->isMaximized() || q->window()->isFullScreen())
        {
            ::EnableMenuItem(hMenu, SC_MOVE, MFS_DISABLED);
            ::EnableMenuItem(hMenu, SC_RESTORE, MFS_ENABLED);
        }
        else
        {
            ::EnableMenuItem(hMenu, SC_MOVE, MFS_ENABLED);
            ::EnableMenuItem(hMenu, SC_RESTORE, MFS_DISABLED);
        }
        if(!isFixedSize && !q->window()->isMaximized() && !q->window()->isFullScreen())
        {
            ::EnableMenuItem(hMenu, SC_SIZE, MFS_ENABLED);
            ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_ENABLED);
        }
        else
        {
            ::EnableMenuItem(hMenu, SC_SIZE, MFS_DISABLED);
            ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_DISABLED);
        }
        const int result = ::TrackPopupMenu(hMenu,
            (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)),
            nativePos.x(), nativePos.y(), 0, hwnd, nullptr);
        if(result != FALSE)
        {
            ::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0);
        }
#endif
    }
}
void QtAppBarPrivate::updateCursor(int _edges)
{
    Q_Q(const QtAppBar);
    switch(_edges)
    {
        case 0: {
            q->window()->setCursor(Qt::ArrowCursor);
            break;
        }
        case Qt::LeftEdge:
        case Qt::RightEdge: {
            q->window()->setCursor(Qt::SizeHorCursor);
            break;
        }
        case Qt::TopEdge:
        case Qt::BottomEdge: {
            q->window()->setCursor(Qt::SizeVerCursor);
            break;
        }
        case Qt::LeftEdge | Qt::TopEdge:
        case Qt::RightEdge | Qt::BottomEdge: {
            q->window()->setCursor(Qt::SizeFDiagCursor);
            break;
        }
        case Qt::RightEdge | Qt::TopEdge:
        case Qt::LeftEdge | Qt::BottomEdge: {
            q->window()->setCursor(Qt::SizeBDiagCursor);
            break;
        }
        default: {
            break;
        }
    }
}
bool QtAppBarPrivate::containsCursorToItem(QWidget* item)
{
    Q_Q(const QtAppBar);
    if(!item || !item->isVisible())
    {
        return false;
    }
    QRect itemRect = QRect(item->mapToGlobal(QPoint(0, 0)), item->size());
    if(item == q)
    {
        for(int i = 0; i < clientWidgetList.count(); i++)
        {
            if(containsCursorToItem(clientWidgetList[i]))
            {
                return false;
            }
        }
        for(int i = 0; i < customAreaWidgetList.count(); i++)
        {
            QWidget* customAreaWidget = customAreaWidgetList[i];
            if(containsCursorToItem(customAreaWidget))
            {
                QObject* customAreaHitTestObject = customAreaHitTestObjectList[i];
                if(customAreaHitTestObject)
                {
                    bool isContainsInAppBar = false;
                    QMetaObject::invokeMethod(customAreaHitTestObject,
                        customAreaHitTestFunctionNameList[i].toLocal8Bit().constData(),
                        Qt::AutoConnection, Q_RETURN_ARG(bool, isContainsInAppBar));
                    return isContainsInAppBar;
                }
                return false;
            }
        }
    }
    else if(item == maxButton)
    {
        itemRect.adjust(0, 8, 0, 0);
    }
    if(itemRect.contains(QCursor::pos()))
    {
        return true;
    }
    return false;
}
void QtAppBarPrivate::onThemeModeChange(QtThemeType::ThemeMode _themeMode)
{
    if(_themeMode == QtThemeType::Light)
    {
        themeChangeButton->setQtIcon(QtIconType::MoonStars);
    }
    else
    {
        themeChangeButton->setQtIcon(QtIconType::SunBright);
    }
}
int QtAppBarPrivate::calculateMinimumWidth()
{
    Q_Q(QtAppBar);
    int appBarWidth = 0;
    if(titleLabel->isVisible())
    {
        appBarWidth += titleLabel->width();
        appBarWidth += 10;
    }
    if(iconLabel->isVisible())
    {
        appBarWidth += iconLabel->width();
        appBarWidth += 10;
    }
    for(int i = 0; i < customAreaWidgetList.count(); i++)
    {
        auto customAreaWidget = customAreaWidgetList[i];
        if(customAreaWidget->isVisible())
        {
            appBarWidth += customAreaWidget->minimumWidth();
        }
    }
    QList<QAbstractButton*> buttonList = q->findChildren<QAbstractButton*>();
    for(const auto clientWidget : clientWidgetList)
    {
        if(clientWidget->isVisible())
        {
            appBarWidth += clientWidget->minimumWidth();
        }
    }
    auto windowMinWidth = q->parentWidget()->minimumWidth() + 20;
    return qMax(appBarWidth, windowMinWidth);
}
QVBoxLayout* QtAppBarPrivate::createVLayout(QWidget* widget)
{
    if(!widget)
    {
        return nullptr;
    }
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    if(widget == iconLabel || widget == titleLabel)
    {
        vLayout->addSpacing(6);
    }
    vLayout->addWidget(widget);
    vLayout->addStretch();
    return vLayout;
}

void QtAppBarPrivate::onMinButtonClicked()
{
    Q_Q(QtAppBar);
    q->window()->showMinimized();
}
void QtAppBarPrivate::onMaxButtonClicked()
{
    Q_Q(QtAppBar);
    bool isMaximized = q->window()->isMaximized();
    isMaximized ? q->window()->showNormal() : q->window()->showMaximized();
#ifndef Q_OS_WIM
    changeMaxButtonAwesome(!isMaximized);
#endif
}
void QtAppBarPrivate::onCloseButtonClicked()
{
    Q_Q(QtAppBar);
    if(isDefaultClosed)
    {
        q->window()->close();
    }
    else
    {
        Q_EMIT q_ptr->closeButtonClicked();
    }
}
void QtAppBarPrivate::onStayTopButtonClicked()
{
#ifdef Q_OS_WIN
    HWND hwnd = (HWND) currentWinID;
    ::SetWindowPos(
        hwnd, isStayTop ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#else
    Q_Q(QtAppBar);
    if(q->window()->isVisible())
    {
        q->window()->windowHandle()->setFlag(Qt::WindowStaysOnTopHint, isStayTop);
        q->window()->update();
    }
    else
    {
        q->window()->setWindowFlag(Qt::WindowStaysOnTopHint, isStayTop);
    }
#endif
    stayTopButton->setIsSelected(isStayTop);
    stayTopButton->update();
}

QtAppBar::QtAppBar(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new QtAppBarPrivate(this, parent))
{
}

QtAppBar::~QtAppBar()
{
}

bool QtAppBar::isStayTop() const
{
    Q_D(const QtAppBar);
    return d->isStayTop;
}

void QtAppBar::setIsStayTop(bool newIsStayTop)
{
    Q_D(QtAppBar);
    if(d->isStayTop == newIsStayTop)
        return;
    d->isStayTop = newIsStayTop;
    Q_EMIT isStayTopChanged();
}

bool QtAppBar::isFixedSize() const
{
    Q_D(const QtAppBar);
    return d->isFixedSize;
}

void QtAppBar::setIsFixedSize(bool newIsFixedSize)
{
    Q_D(QtAppBar);
    if(d->isFixedSize == newIsFixedSize)
        return;
    d->isFixedSize = newIsFixedSize;
    Q_EMIT isFixedSizeChanged();
}

bool QtAppBar::isDefaultClosed() const
{
    Q_D(const QtAppBar);
    return d->isDefaultClosed;
}

void QtAppBar::setIsDefaultClosed(bool newIsDefaultClosed)
{
    Q_D(QtAppBar);
    if(d->isDefaultClosed == newIsDefaultClosed)
        return;
    d->isDefaultClosed = newIsDefaultClosed;
    Q_EMIT isDefaultClosedChanged();
}

bool QtAppBar::isOnlyAllowMinAndClose() const
{
    Q_D(const QtAppBar);
    return d->isOnlyAllowMinAndClose;
}

void QtAppBar::setIsOnlyAllowMinAndClose(bool newIsOnlyAllowMinAndClose)
{
    Q_D(QtAppBar);
    if(d->isOnlyAllowMinAndClose == newIsOnlyAllowMinAndClose)
        return;
    d->isOnlyAllowMinAndClose = newIsOnlyAllowMinAndClose;
    Q_EMIT isOnlyAllowMinAndCloseChanged();
}

int QtAppBar::appBarHeight() const
{
    Q_D(const QtAppBar);
    return d->appBarHeight;
}

void QtAppBar::setAppBarHeight(int newAppBarHeight)
{
    Q_D(QtAppBar);
    if(d->appBarHeight == newAppBarHeight)
        return;
    d->appBarHeight = newAppBarHeight;
    setFixedHeight(d->appBarHeight);
    window()->setContentsMargins(0, height(), 0, 0);
    Q_EMIT appBarHeightChanged();
}

void QtAppBar::setCustomWidget(QtAppBarType::CustomArea customArea, QWidget* widget,
    QObject* hitTestObject, const QString& hitTestFunctionName)
{
    Q_D(QtAppBar);
    if(!widget || widget == this)
    {
        return;
    }
    widget->setMinimumHeight(0);
    widget->setMaximumHeight(height());
    widget->setParent(this);
    int customAreaIndex = (int) customArea - 1;
    d->mainLayout->removeWidget(d->customAreaWidgetList[customAreaIndex]);
    d->mainLayout->insertWidget(customAreaIndex + 1, widget);
    d->customAreaWidgetList[customAreaIndex] = widget;
    d->customAreaHitTestObjectList[customAreaIndex] = hitTestObject;
    d->customAreaHitTestFunctionNameList[customAreaIndex] = hitTestFunctionName;
    Q_EMIT customWidgetChanged();
}

QWidget* QtAppBar::customWidget(QtAppBarType::CustomArea customArea) const
{
    Q_D(const QtAppBar);
    int customAreaIndex = (int) customArea - 1;
    return d->customAreaWidgetList[customAreaIndex];
}

void QtAppBar::setCustomMenu(QMenu* customMenu)
{
    Q_D(QtAppBar);
    d->customMenu = customMenu;
    Q_EMIT customMenuChanged();
}

QMenu* QtAppBar::customMenu() const
{
    Q_D(const QtAppBar);
    return d->customMenu;
}

void QtAppBar::setWindowButtonFlag(QtAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(QtAppBar);
    if(isEnable)
    {
        setWindowButtonFlags(d->buttonFlags | buttonFlag);
    }
    else
    {
        setWindowButtonFlags(d->buttonFlags & ~buttonFlag);
    }
}

void QtAppBar::setWindowButtonFlags(QtAppBarType::ButtonFlags buttonFlags)
{
    Q_D(QtAppBar);
    d->buttonFlags = buttonFlags;
    if(d->buttonFlags.testFlag(QtAppBarType::NoneButtonHint))
    {
        d->routeBackButton->setVisible(false);
        d->routeForwardButton->setVisible(false);
        d->navigationButton->setVisible(false);
        d->stayTopButton->setVisible(false);
        d->themeChangeButton->setVisible(false);
        d->minButton->setVisible(false);
        d->maxButton->setVisible(false);
        d->closeButton->setVisible(false);
    }
    else
    {
        d->routeBackButton->setVisible(d->buttonFlags.testFlag(QtAppBarType::RouteBackButtonHint));
        d->routeForwardButton->setVisible(
            d->buttonFlags.testFlag(QtAppBarType::RouteForwardButtonHint));
        d->navigationButton->setVisible(
            d->buttonFlags.testFlag(QtAppBarType::NavigationButtonHint));
        d->stayTopButton->setVisible(d->buttonFlags.testFlag(QtAppBarType::StayTopButtonHint));
        d->themeChangeButton->setVisible(
            d->buttonFlags.testFlag(QtAppBarType::ThemeChangeButtonHint));
        d->minButton->setVisible(d->buttonFlags.testFlag(QtAppBarType::MinimizeButtonHint));
        d->maxButton->setVisible(d->buttonFlags.testFlag(QtAppBarType::MaximizeButtonHint));
        d->closeButton->setVisible(d->buttonFlags.testFlag(QtAppBarType::CloseButtonHint));
    }
}

QtAppBarType::ButtonFlags QtAppBar::windowButtonFlags() const
{
    Q_D(const QtAppBar);
    return d->buttonFlags;
}

void QtAppBar::setRouteBackButtonEnable(bool isEnable)
{
    Q_D(QtAppBar);
    d->routeBackButton->setEnabled(isEnable);
}

void QtAppBar::setRouteForwardButtonEnable(bool isEnable)
{
    Q_D(QtAppBar);
    d->routeForwardButton->setEnabled(isEnable);
}

void QtAppBar::closeWindow()
{
    Q_D(QtAppBar);
    QPropertyAnimation* closeOpacityAnimation = new QPropertyAnimation(window(), "windowOpacity");
    connect(
        closeOpacityAnimation, &QPropertyAnimation::finished, this, [this]() { window()->close(); });
    closeOpacityAnimation->setStartValue(1);
    closeOpacityAnimation->setEndValue(0);
    closeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    closeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    if(window()->isMaximized() || window()->isFullScreen() || d->isFixedSize)
    {
        return;
    }
    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(window(), "geometry");
    QRect geometry = window()->geometry();
    geometryAnimation->setStartValue(geometry);
    qreal targetWidth = (geometry.width() - d->lastMinTrackWidth) * 0.7 + d->lastMinTrackWidth;
    qreal targetHeight =
        (geometry.height() - window()->minimumHeight()) * 0.7 + window()->minimumHeight();
    geometryAnimation->setEndValue(QRectF(geometry.center().x() - targetWidth / 2,
        geometry.center().y() - targetHeight / 2, targetWidth, targetHeight));
    geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
int QtAppBar::takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
int QtAppBar::takeOverNativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
    Q_D(QtAppBar);
    
    if((eventType != "windows_generic_MSG") || !message)
    {
        return 0;
    }
    const auto msg = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    if(!hwnd || !msg)
    {
        return 0;
    }
    d->currentWinID = (qint64) hwnd;
    const UINT uMsg = msg->message;
    const WPARAM wParam = msg->wParam;
    const LPARAM lParam = msg->lParam;
    switch(uMsg)
    {
        case WM_WINDOWPOSCHANGING: {
            WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
            if(wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
            {
                wp->flags |= SWP_NOCOPYBITS;
                *result = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
                return 1;
            }
            return 0;
        }
        case WM_NCPAINT: {
            if(!qtWinHelper->isCompositionEnabled())
            {
                *result = FALSE;
                return 0;
            }
            else
            {
                return -1;
            }
        }
        case WM_NCACTIVATE: {
            if(qtWinHelper->isCompositionEnabled())
            {
                *result = ::DefWindowProcW(hwnd, WM_NCACTIVATE, wParam, -1);
            }
            else
            {
                *result = TRUE;
            }
            return 1;
        }
        case WM_SIZE: {
            if(wParam == SIZE_RESTORED)
            {
                d->changeMaxButtonAwesome(false);
            }
            else if(wParam == SIZE_MAXIMIZED)
            {
                d->changeMaxButtonAwesome(true);
            }
            return 0;
        }
#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
        case WM_SHOWWINDOW: {
            if(wParam == FALSE)
            {
                return 0;
            }
            RECT windowRect{};
            ::GetWindowRect(hwnd, &windowRect);
            int windowWidth = windowRect.right - windowRect.left;
            int windowHeight = windowRect.bottom - windowRect.top;
            static UINT swpFlags =
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE;
            ::SetWindowPos(hwnd, nullptr, 0, 0, windowWidth + 1, windowHeight, swpFlags);
            ::SetWindowPos(hwnd, nullptr, 0, 0, windowWidth, windowHeight, swpFlags);
            return -1;
        }
#endif
        case WM_NCCALCSIZE: {
            if(wParam == FALSE)
            {
                return 0;
            }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
            if(::IsZoomed(hwnd))
            {
                this->move(7, 7);
                window()->setContentsMargins(8, 8 + height(), 8, 8);
            }
            else
            {
                this->move(0, 0);
                window()->setContentsMargins(0, height(), 0, 0);
            }
            *result = 0;
            return 1;
#else
            RECT* clientRect = &((NCCALCSIZE_PARAMS*) (lParam))->rgrc[0];
            if(qtWinHelper->isWinVersionGreater10())
            {
                const LONG originTop = clientRect->top;
                const LRESULT hitTestResult = ::DefWindowProcW(hwnd, WM_NCCALCSIZE, wParam, lParam);
                if((hitTestResult != HTERROR) && (hitTestResult != HTNOWHERE))
                {
                    *result = static_cast<long>(hitTestResult);
                    return 1;
                }
                clientRect->top = originTop;
            }
            if(::IsZoomed(hwnd))
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
                auto geometry = window()->screen()->geometry();
#else
                QScreen* screen = qApp->screenAt(window()->geometry().center());
                QRect geometry;
                if(!screen)
                {
                    screen = qApp->screenAt(QCursor::pos());
                }
                geometry = screen->geometry();
#endif
                clientRect->top = geometry.top();
                if(!qtWinHelper->isWinVersionGreater10())
                {
                    quint32 borderThickness = qtWinHelper->resizeBorderThickness(hwnd);
                    clientRect->left = geometry.left();
                    clientRect->bottom -= borderThickness;
                    clientRect->right -= borderThickness;
                }
            }
            *result = WVR_REDRAW;
            return 1;
#endif
        }
        case WM_MOVE: {
            QScreen* currentScreen = qApp->screenAt(window()->geometry().center());
            if(currentScreen && currentScreen != d->lastScreen)
            {
                if(d->lastScreen)
                {
                    ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                        SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE |
                            SWP_FRAMECHANGED);
                    ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
                }
                d->lastScreen = currentScreen;
            }
            break;
        }
        case WM_NCHITTEST: {
            if(d->containsCursorToItem(d->maxButton))
            {
                if(*result == HTNOWHERE)
                {
                    if(!d->isHoverMaxButton)
                    {
                        d->isHoverMaxButton = true;
                        d->maxButton->setIsSelected(true);
                        d->maxButton->update();
                    }
                    *result = HTZOOM;
                }
                return 1;
            }
            else
            {
                if(d->isHoverMaxButton)
                {
                    d->isHoverMaxButton = false;
                    d->maxButton->setIsSelected(false);
                    d->maxButton->update();
                }
            }
            POINT nativeLocalPos{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ::ScreenToClient(hwnd, &nativeLocalPos);
            RECT clientRect{ 0, 0, 0, 0 };
            ::GetClientRect(hwnd, &clientRect);
            auto clientWidth = clientRect.right - clientRect.left;
            auto clientHeight = clientRect.bottom - clientRect.top;
            bool left = nativeLocalPos.x < d->win7Margins;
            bool right = nativeLocalPos.x > clientWidth - d->win7Margins;
            bool top = nativeLocalPos.y < d->margins;
            bool bottom = nativeLocalPos.y > clientHeight - d->win7Margins;
            *result = HTNOWHERE;
            if(!d->isOnlyAllowMinAndClose && !d->isFixedSize && !window()->isFullScreen() &&
                !window()->isMaximized())
            {
                if(left && top)
                {
                    *result = HTTOPLEFT;
                }
                else if(left && bottom)
                {
                    *result = HTBOTTOMLEFT;
                }
                else if(right && top)
                {
                    *result = HTTOPRIGHT;
                }
                else if(right && bottom)
                {
                    *result = HTBOTTOMRIGHT;
                }
                else if(left)
                {
                    *result = HTLEFT;
                }
                else if(right)
                {
                    *result = HTRIGHT;
                }
                else if(top)
                {
                    *result = HTTOP;
                }
                else if(bottom)
                {
                    *result = HTBOTTOM;
                }
            }
            if(0 != *result)
            {
                return 1;
            }
            if(d->containsCursorToItem(this) && !window()->isFullScreen())
            {
                *result = HTCAPTION;
                return 1;
            }
            *result = HTCLIENT;
            return 1;
        }
        case WM_GETMINMAXINFO: {
            MINMAXINFO* minmaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
            RECT rect;
            SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
            d->lastMinTrackWidth = d->calculateMinimumWidth();
            minmaxInfo->ptMinTrackSize.x = d->lastMinTrackWidth * qApp->devicePixelRatio();
            minmaxInfo->ptMinTrackSize.y =
                parentWidget()->minimumHeight() * qApp->devicePixelRatio();
            minmaxInfo->ptMaxPosition.x = rect.left;
            minmaxInfo->ptMaxPosition.y = rect.top;
            return 1;
        }
        case WM_LBUTTONDBLCLK: {
            QVariantMap postData;
            postData.insert("WMClickType", QtAppBarType::WMLBUTTONDBLCLK);
            QtEventBus::getInstance()->post("WMWindowClicked", postData);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            QVariantMap postData;
            postData.insert("WMClickType", QtAppBarType::WMLBUTTONDOWN);
            QtEventBus::getInstance()->post("WMWindowClicked", postData);
            return 0;
        }
        case WM_LBUTTONUP: {
            QVariantMap postData;
            postData.insert("WMClickType", QtAppBarType::WMLBUTTONUP);
            QtEventBus::getInstance()->post("WMWindowClicked", postData);
            return 0;
        }
        case WM_NCLBUTTONDOWN: {
            QVariantMap postData;
            postData.insert("WMClickType", QtAppBarType::WMNCLBUTTONDOWN);
            QtEventBus::getInstance()->post("WMWindowClicked", postData);
            if(d->containsCursorToItem(d->maxButton) || d->isOnlyAllowMinAndClose)
            {
                return 1;
            }
            break;
        }
        case WM_NCLBUTTONUP: {
            QVariantMap postData;
            postData.insert("WMClickType", QtAppBarType::WMNCLBUTTONDOWN);
            QtEventBus::getInstance()->post("WMWindowClicked", postData);
            if(d->containsCursorToItem(d->maxButton) && !d->isOnlyAllowMinAndClose)
            {
                d->onMaxButtonClicked();
                return 1;
            }
            break;
        }
        case WM_NCLBUTTONDBLCLK: {
            if(!d->isOnlyAllowMinAndClose && !d->isFixedSize)
            {
                return 0;
            }
            return 1;
        }
        case WM_NCRBUTTONDOWN: {
            if(wParam == HTCAPTION && !d->isOnlyAllowMinAndClose)
            {
                d->showAppBarMenu(QCursor::pos());
            }
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            if((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 0x8000) &&
                !d->isOnlyAllowMinAndClose)
            {
                auto pos = window()->geometry().topLeft();
                d->showAppBarMenu(QPoint(pos.x(), pos.y() + this->height()));
            }
            break;
        }
    } 
    return -1;
}
#endif

bool QtAppBar::eventFilter(QObject* obj, QEvent* event)
{
    Q_D(QtAppBar);
    switch(event->type())
    {
        case QEvent::Resize: {
            QSize size = parentWidget()->size();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    #ifdef Q_OS_WIN
            if(::IsZoomed((HWND) d->currentWinID))
            {
                this->resize(size.width() - 14, this->height());
            }
            else
            {
                this->resize(size.width(), this->height());
            }
    #endif
#else
            this->resize(size.width(), this->height());
#endif
            break;
        }
#ifdef Q_OS_WIN
        case QEvent::Show: {
            if(!d->isFixedSize && !d->isOnlyAllowMinAndClose)
            {
                HWND hwnd = (HWND) d->currentWinID;
                DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
                style &= ~WS_SYSMENU;
                ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
                if(!qtWinHelper->isWinVersionGreater10())
                {
                    SetClassLong(hwnd, GCL_STYLE, GetClassLong(hwnd, GCL_STYLE) | CS_DROPSHADOW);
                }
            }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
            HWND hwnd = (HWND) d->currentWinID;
            QtWinShadowHelper::getInstance()->setWindowShadow(d->currentWinID);
            DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
            bool hasCaption = (style & WS_CAPTION) == WS_CAPTION;
            if(!hasCaption)
            {
                ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CAPTION);
            }
#endif
            break;
        }
#endif
        case QEvent::Close: {
            QCloseEvent* closeEvent = dynamic_cast<QCloseEvent*>(event);
            if(!d->isDefaultClosed && closeEvent->spontaneous())
            {
                event->ignore();
                if(window()->isMinimized())
                {
                    window()->showNormal();
                }
                d->onCloseButtonClicked();
                return true;
            }
            else
            {
                break;
            }
        }
#ifndef Q_OS_WIN
        case QEvent::MouseButtonPress: {
            if(d->edges != 0)
            {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                if(mouseEvent->button() == Qt::LeftButton)
                {
                    d->updateCursor(d->edges);
                    window()->windowHandle()->startSystemResize(Qt::Edges(d->edges));
                }
            }
            else
            {
                if(d->containsCursorToItem(this))
                {
                    qint64 clickTimer = QDateTime::currentMSecsSinceEpoch();
                    qint64 offset = clickTimer - d->clickTimer;
                    d->clickTimer = clickTimer;
                    if(offset > 300)
                    {
                        window()->windowHandle()->startSystemMove();
                    }
                }
            }
            break;
        }
        case QEvent::MouseButtonDblClick: {
            if(d->containsCursorToItem(this))
            {
                if(window()->isMaximized())
                {
                    window()->showNormal();
                }
                else
                {
                    window()->showMaximized();
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            d->edges = 0;
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::RightButton)
            {
                d->showAppBarMenu(QCursor::pos());
            }
            break;
        }
        case QEvent::HoverMove: {
            if(window()->isMaximized() || window()->isFullScreen())
            {
                break;
            }
            if(d->isFixedSize)
            {
                break;
            }
            QHoverEvent* mouseEvent = static_cast<QHoverEvent*>(event);
            QPoint p =
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                mouseEvent->pos();
#else
                mouseEvent->position().toPoint();
#endif
            if(p.x() >= d->margins && p.x() <= (window()->width() - d->margins) &&
                p.y() >= d->margins && p.y() <= (window()->height() - d->margins))
            {
                if(d->edges != 0)
                {
                    d->edges = 0;
                    d->updateCursor(d->edges);
                }
                break;
            }
            d->edges = 0;
            if(p.x() < d->margins)
            {
                d->edges |= Qt::LeftEdge;
            }
            if(p.x() > (window()->width() - d->margins))
            {
                d->edges |= Qt::RightEdge;
            }
            if(p.y() < d->margins)
            {
                d->edges |= Qt::TopEdge;
            }
            if(p.y() > (window()->height() - d->margins))
            {
                d->edges |= Qt::BottomEdge;
            }
            d->updateCursor(d->edges);
            break;
        }
#endif
        default: {
            break;
        }
    }
    return QObject::eventFilter(obj, event);
}
#ifdef Q_OS_WIN
void QtAppBar::paintEvent(QPaintEvent*)
{
    if(qtWinHelper->isWinVersionGreater10() && !qtWinHelper->isWinVersionGreater11())
    {
        Q_D(QtAppBar);
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing);
        auto borderWidth = qtWinHelper->systemMetricsForDpi((HWND) d->currentWinID, SM_CXBORDER);
        painter.setPen(
            QPen(window()->isActiveWindow() ? QtThemeColor(d->themeMode, Win10BorderActive)
                                            : QtThemeColor(d->themeMode, Win10BorderInactive),
                borderWidth));
        painter.drawLine(QPoint(0, 0), QPoint(window()->width(), 0));
        painter.restore();
    }
}
#endif
