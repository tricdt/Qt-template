#include "qtcustomwidget.h"
#include "qtapplication.h"
#include "qttheme.h"

class QtCustomWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtCustomWidget)
public:
    QtCustomWidget* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtApplicationType::WindowDisplayMode windowDisplayMode;
    QtCustomWidgetPrivate(QtCustomWidget* q);
    ~QtCustomWidgetPrivate();
};

QtCustomWidgetPrivate::QtCustomWidgetPrivate(QtCustomWidget* q)
    : q_ptr(q)
{
}
QtCustomWidgetPrivate::~QtCustomWidgetPrivate()
{
}
Q_TAKEOVER_NATIVEEVENT_CPP(QtCustomWidget, appBar);
QtCustomWidget::QtCustomWidget(QWidget* parent)
    : QDialog{ parent }
    , d_ptr(new QtCustomWidgetPrivate(this))
{
    Q_D(QtCustomWidget);
    resize(500, 500);
    setObjectName("QtCustomWidget");
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                   Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint |
                   Qt::WindowFullscreenButtonHint);
#if (QT_VERSION < QT_VERSION_CHECK(6, 5, 3) || QT_VERSION > QT_VERSION_CHECK(6, 6, 1))
    setStyleSheet("#QtCustomWidget{background-color:transparent;}");
#endif
    appBar = new QtAppBar(this);
    appBar->setWindowButtonFlags(QtAppBarType::MinimizeButtonHint |
                                 QtAppBarType::MaximizeButtonHint | QtAppBarType::CloseButtonHint);
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });

    d->windowDisplayMode = qtApp->windowDisplayMode();
    connect(qtApp, &QtApplication::windowDisplayModeChanged, this, [this]() {
        d_ptr->windowDisplayMode = qtApp->windowDisplayMode();
        update();
    });
    qtApp->syncWindowDisplayMode(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

QtCustomWidget::~QtCustomWidget()
{
    qtApp->syncWindowDisplayMode(this, false);
}

void QtCustomWidget::setCentralWidget(QWidget* widget)
{
    Q_D(QtCustomWidget);
    if (!widget)
    {
        return;
    }
    centralWidget = widget;
    mainLayout->addWidget(widget);
    widget->setVisible(true);
}

void QtCustomWidget::paintEvent(QPaintEvent* event)
{
    Q_D(QtCustomWidget);
#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 3) && QT_VERSION <= QT_VERSION_CHECK(6, 6, 1))
    if(d->windowDisplayMode != QtApplicationType::WindowDisplayMode::QtMica)
#else
    if(d->windowDisplayMode == QtApplicationType::WindowDisplayMode::Normal)
#endif
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QtThemeColor(d->themeMode, WindowBase));
        painter.drawRect(rect());
        painter.restore();
    }
}
