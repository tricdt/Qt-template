#include "qtcontentdialog.h"
#include "qtmaskwidget.h"
#include "qtpushbutton.h"
#include "qttheme.h"
#include "widgets/qttext.h"

class QtContentDialogPrivate
{
    Q_DECLARE_PUBLIC(QtContentDialog)
public:
    QtContentDialog* q_ptr;
    qint64 currentWinID{ 0 };
    QtAppBar* appBar{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QtMaskWidget* maskWidget{ nullptr };
    QWidget* centralWidget{ nullptr };
    QWidget* buttonWidget{ nullptr };
    QVBoxLayout* mainLayout{ nullptr };
    QHBoxLayout* buttonLayout{ nullptr };
    QString leftButtonText{ "cancel" };
    QString middleButtonText{ "minimum" };
    QString rightButtonText{ "exit" };
    QtPushButton* leftButton{ nullptr };
    QtPushButton* middleButton{ nullptr };
    QtPushButton* rightButton{ nullptr };
    bool isLeftButtonVisible{ true };
    bool isMiddleButtonVisible{ true };
    bool isRightButtonVisible{ true };
    QtContentDialogPrivate(QtContentDialog* q);
    ~QtContentDialogPrivate();
    void doCloseAnimation(bool isAccept);
    void moveToCenter();
};

void QtContentDialogPrivate::doCloseAnimation(bool isAccept)
{
    Q_Q(QtContentDialog);
    maskWidget->doMaskAnimation(0);
    isAccept ? q->accept() : q->reject();
    QApplication::processEvents();
    if(const auto windowHandle = q->windowHandle())
    {
        windowHandle->close();
    }
}

void QtContentDialogPrivate::moveToCenter()
{
    Q_Q(QtContentDialog);
    int width = q->width();
    int height = q->height();
    auto globalPos = maskWidget->mapToGlobal(QPoint{ 0, 0 });
    q->setGeometry(globalPos.x() + (maskWidget->width() - width) / 2,
        globalPos.y() + (maskWidget->height() - height) / 2, width, height);
}

QtContentDialogPrivate::QtContentDialogPrivate(QtContentDialog* q)
    : q_ptr(q)
{
}
QtContentDialogPrivate::~QtContentDialogPrivate()
{
}
Q_TAKEOVER_NATIVEEVENT_CPP(QtContentDialog, d_func()->appBar);
QtContentDialog::QtContentDialog(QWidget* parent)
    : QDialog{ parent }
    , d_ptr(new QtContentDialogPrivate(this))
{
    Q_D(QtContentDialog);
    d->q_ptr = this;

    d->maskWidget = new QtMaskWidget(parent);
    d->maskWidget->move(0, 0);
    d->maskWidget->setFixedSize(parent->size());
    d->maskWidget->setVisible(false);

    resize(400, height());
    setWindowModality(Qt::ApplicationModal);

    d->appBar = new QtAppBar(this);
    d->appBar->setWindowButtonFlags(QtAppBarType::NoneButtonHint);
    d->appBar->setIsFixedSize(true);
    d->appBar->setAppBarHeight(0);
#ifdef Q_OS_WIN
    createWinId();
#endif
    d->leftButton = new QtPushButton("cancel", this);
    connect(d->leftButton, &QtPushButton::clicked, this, [this]() {
        onLeftButtonClicked();
        d_ptr->doCloseAnimation(false);
        QTimer::singleShot(0, nullptr, [=]() { Q_EMIT leftButtonClicked(); });
    });
    d->leftButton->setMinimumSize(0, 0);
    d->leftButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->leftButton->setFixedHeight(38);
    d->leftButton->setBorderRadius(6);
    d->middleButton = new QtPushButton("minimum", this);
    connect(d->middleButton, &QtPushButton::clicked, this, [this]() {
        onMiddleButtonClicked();
        QTimer::singleShot(0, nullptr, [this]() { Q_EMIT middleButtonClicked(); });
    });
    d->middleButton->setMinimumSize(0, 0);
    d->middleButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->middleButton->setFixedHeight(38);
    d->middleButton->setBorderRadius(6);
    d->rightButton = new QtPushButton("exit", this);
    connect(d->rightButton, &QtPushButton::clicked, this, [this]() {
        onRightButtonClicked();
        d_ptr->doCloseAnimation(true);
        QTimer::singleShot(0, nullptr, [this]() { Q_EMIT rightButtonClicked(); });
    });
    d->rightButton->setLightDefaultColor(QtThemeColor(QtThemeType::Light, PrimaryNormal));
    d->rightButton->setLightHoverColor(QtThemeColor(QtThemeType::Light, PrimaryHover));
    d->rightButton->setLightPressColor(QtThemeColor(QtThemeType::Light, PrimaryPress));
    d->rightButton->setLightTextColor(Qt::white);
    d->rightButton->setDarkDefaultColor(QtThemeColor(QtThemeType::Dark, PrimaryNormal));
    d->rightButton->setDarkHoverColor(QtThemeColor(QtThemeType::Dark, PrimaryHover));
    d->rightButton->setDarkPressColor(QtThemeColor(QtThemeType::Dark, PrimaryPress));
    d->rightButton->setDarkTextColor(Qt::black);
    d->rightButton->setMinimumSize(0, 0);
    d->rightButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->rightButton->setFixedHeight(38);
    d->rightButton->setBorderRadius(6);

    d->centralWidget = new QWidget(this);
    QVBoxLayout* centralVLayout = new QVBoxLayout(d->centralWidget);
    centralVLayout->setContentsMargins(15, 25, 15, 10);
    QtText* title = new QtText("Exit", this);
    title->setTextStyle(QtTextType::Title);
    QtText* subTitle = new QtText("Are you sure you want to exit the application?", this);
    subTitle->setTextStyle(QtTextType::Body);
    centralVLayout->addWidget(title);
    centralVLayout->addSpacing(2);
    centralVLayout->addWidget(subTitle);
    centralVLayout->addStretch();

    d->mainLayout = new QVBoxLayout(this);
    d->mainLayout->setContentsMargins(0, 0, 0, 0);
    d->buttonWidget = new QWidget(this);
    d->buttonWidget->setFixedHeight(60);
    d->buttonLayout = new QHBoxLayout(d->buttonWidget);
    d->buttonLayout->addWidget(d->leftButton);
    d->buttonLayout->addWidget(d->middleButton);
    d->buttonLayout->addWidget(d->rightButton);
    d->mainLayout->addWidget(d->centralWidget);
    d->mainLayout->addWidget(d->buttonWidget);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtContentDialog::~QtContentDialog()
{
    Q_D(QtContentDialog);
    d->maskWidget->deleteLater();
}

void QtContentDialog::onLeftButtonClicked()
{
}

void QtContentDialog::onMiddleButtonClicked()
{
}

void QtContentDialog::onRightButtonClicked()
{
}

void QtContentDialog::setCentralWidget(QWidget* centralWidget)
{
    Q_D(QtContentDialog);
    d->mainLayout->takeAt(0);
    d->mainLayout->takeAt(0);
    delete d->centralWidget;
    d->centralWidget = centralWidget;
    d->mainLayout->addWidget(centralWidget);
    d->mainLayout->addWidget(d->buttonWidget);
}

void QtContentDialog::setLeftButtonText(QString text)
{
    Q_D(QtContentDialog);
    d->leftButton->setText(text);
}

void QtContentDialog::setMiddleButtonText(QString text)
{
    Q_D(QtContentDialog);
    d->middleButton->setText(text);
}

void QtContentDialog::setRightButtonText(QString text)
{
    Q_D(QtContentDialog);
    d->rightButton->setText(text);
}

void QtContentDialog::setLeftButtonVisible(bool visible)
{
    Q_D(QtContentDialog);
    if(d->isLeftButtonVisible == visible)
    {
        return;
    }
    d->isLeftButtonVisible = visible;
    if(visible)
    {
        d->buttonLayout->insertWidget(0, d->leftButton);
        d->leftButton->show();
    }
    else
    {
        d->buttonLayout->removeWidget(d->leftButton);
        d->leftButton->hide();
    }
}

void QtContentDialog::setMiddleButtonVisible(bool visible)
{
    Q_D(QtContentDialog);
    if(d->isMiddleButtonVisible == visible)
    {
        return;
    }
    d->isMiddleButtonVisible = visible;
    if(visible)
    {
        d->buttonLayout->insertWidget(1, d->middleButton);
        d->middleButton->show();
    }
    else
    {
        d->buttonLayout->removeWidget(d->middleButton);
        d->middleButton->hide();
    }
}

void QtContentDialog::setRightButtonVisible(bool visible)
{
    Q_D(QtContentDialog);
    if(d->isRightButtonVisible == visible)
    {
        return;
    }
    d->isRightButtonVisible = visible;
    if(visible)
    {
        d->buttonLayout->addWidget(d->rightButton);
        d->rightButton->show();
    }
    else
    {
        d->buttonLayout->removeWidget(d->rightButton);
        d->rightButton->hide();
    }
}

void QtContentDialog::close()
{
    Q_D(QtContentDialog);
    d->doCloseAnimation(false);
}

void QtContentDialog::showEvent(QShowEvent* event)
{
    Q_D(QtContentDialog);
    d->maskWidget->setVisible(true);
    d->maskWidget->raise();
    d->maskWidget->setFixedSize(parentWidget()->size());
    d->maskWidget->doMaskAnimation(90);
    d->moveToCenter();
    QDialog::showEvent(event);
}

void QtContentDialog::paintEvent(QPaintEvent* event)
{
    Q_D(QtContentDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, DialogBase));
    painter.drawRect(rect());
    painter.setBrush(QtThemeColor(d->themeMode, DialogLayoutArea));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    painter.restore();
}

void QtContentDialog::keyPressEvent(QKeyEvent* event)
{
    Q_D(QtContentDialog);
    switch(event->key())
    {
        case Qt::Key_Escape: {
            d->doCloseAnimation(false);
            break;
        }
        default: {
            break;
        }
    }
    event->accept();
}
