#include "qtwidget.h"
#include "qtappbar.h"
#include "qtdef.h"
#include "qtapplication.h"
#include "qttheme.h"

class QtWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtWidget)
public:
    QtWidget* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtAppBar* appBar;
    QtApplicationType::WindowDisplayMode windowDisplayMode;
    QtWidgetPrivate(QtWidget* q);
};

QtWidgetPrivate::QtWidgetPrivate(QtWidget* q)
    : q_ptr(q)
{
    q->resize(500, 500); // 默认宽高
    q->setWindowTitle("QtWidget");
    q->setObjectName("QtWidget");
#if (QT_VERSION < QT_VERSION_CHECK(6, 5, 3) || QT_VERSION > QT_VERSION_CHECK(6, 6, 1))
    // q->setStyleSheet("#QtWidget{background-color:transparent;}");
#endif
    appBar = new QtAppBar(q);
    appBar->setIsStayTop(true);
    appBar->setWindowButtonFlags(QtAppBarType::StayTopButtonHint |
                                 QtAppBarType::MinimizeButtonHint |
                                 QtAppBarType::MaximizeButtonHint | QtAppBarType::CloseButtonHint |
                                 QtAppBarType::ThemeChangeButtonHint);
    // appBar->setAppBarHeight(45);
    QObject::connect(appBar, &QtAppBar::routeBackButtonClicked, q,
        [this]() { Q_EMIT q_ptr->routeBackButtonClicked(); });
    QObject::connect(appBar, &QtAppBar::routeForwardButtonClicked, q,
        [this]() { Q_EMIT q_ptr->navigationButtonClicked(); });
    QObject::connect(appBar, &QtAppBar::themeChangeButtonClicked, q,
        [this]() { Q_EMIT q_ptr->themeChangeButtonClicked(); });
    QObject::connect(
        appBar, &QtAppBar::closeButtonClicked, q, [=]() { Q_EMIT q->closeButtonClicked(); });
    themeMode = qtTheme->getThemeMode();
    QObject::connect(qtTheme, &QtTheme::themeModeChanged, q, [this](QtThemeType::ThemeMode themeMode) {
        this->themeMode = themeMode;
        q_ptr->update();
    });

    windowDisplayMode = qtApp->windowDisplayMode();
    QObject::connect(qtApp, &QtApplication::windowDisplayModeChanged, q, [this]() {
        windowDisplayMode = qtApp->windowDisplayMode();
        q_ptr->update();
    });
    qtApp->syncWindowDisplayMode(q);
}

Q_TAKEOVER_NATIVEEVENT_CPP(QtWidget, d_func()->appBar);
QtWidget::QtWidget(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new QtWidgetPrivate(this))
{
}

QtWidget::~QtWidget()
{
    qtApp->syncWindowDisplayMode(this, false);
}

bool QtWidget::isStayTop() const
{
    Q_D(const QtWidget);
    return d->appBar->isStayTop();
}

void QtWidget::setIsStayTop(bool newIsStayTop)
{
    Q_D(QtWidget);
    if(d->appBar->isStayTop() == newIsStayTop)
        return;
    d->appBar->setIsStayTop(newIsStayTop);
    Q_EMIT isStayTopChanged();
}

bool QtWidget::isFixedSize() const
{
    Q_D(const QtWidget);
    return d->appBar->isFixedSize();
}

void QtWidget::setIsFixedSize(bool newIsFixedSize)
{
    Q_D(QtWidget);
    if(d->appBar->isFixedSize() == newIsFixedSize)
        return;
    d->appBar->setIsFixedSize(newIsFixedSize);
    Q_EMIT isFixedSizeChanged();
}

bool QtWidget::isDefaultClosed() const
{
    Q_D(const QtWidget);
    return d->appBar->isDefaultClosed();
}

void QtWidget::setIsDefaultClosed(bool newIsDefaultClosed)
{
    Q_D(QtWidget);
    if(d->appBar->isDefaultClosed() == newIsDefaultClosed)
        return;
    d->appBar->setIsDefaultClosed(newIsDefaultClosed);
    Q_EMIT isDefaultClosedChanged();
}

int QtWidget::appBarHeight() const
{
    Q_D(const QtWidget);
    return d->appBar->appBarHeight();
}

void QtWidget::setAppBarHeight(int newAppBarHeight)
{
    Q_D(QtWidget);
    if(d->appBar->appBarHeight() == newAppBarHeight)
        return;
    d->appBar->setAppBarHeight(newAppBarHeight);
    Q_EMIT appBarHeightChanged();
}

void QtWidget::moveToCenter()
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
void QtWidget::setWindowButtonFlag(QtAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(QtWidget);
    d->appBar->setWindowButtonFlag(buttonFlag, isEnable);
}
void QtWidget::setWindowButtonFlags(QtAppBarType::ButtonFlags buttonFlags)
{
    Q_D(QtWidget);
    d->appBar->setWindowButtonFlags(buttonFlags);
}
QtAppBarType::ButtonFlags QtWidget::windowButtonFlags() const
{
    Q_D(const QtWidget);
    return d->appBar->windowButtonFlags();
}

void QtWidget::paintEvent(QPaintEvent* event)
{
    Q_D(QtWidget);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    if(d->windowDisplayMode != QtApplicationType::WindowDisplayMode::QtMica)
#else
    if(d->windowDisplayMode == QtApplicationType::WindowDisplayMode::Normal)
#endif
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QtThemeColor(d->themeMode, WindowBase));
        painter.drawRect(rect());
        painter.restore();
    }
    QWidget::paintEvent(event);
}

#ifdef Q_OS_MACOS
void QtWidget::resizeEvent(QResizeEvent *event){
    Q_D(QtWidget);
    d->appBar->setFixedWidth(width());
}
#endif