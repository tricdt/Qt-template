#include "qtmessagedialog.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qboxlayout.h>
#include <qevent.h>

class QtMessageDialogButtonPrivate
{
    Q_DECLARE_PUBLIC(QtMessageDialogButton)
public:
    QtMessageDialogButton* q_ptr;
    QtMessageDialogButton::ButtonType buttonType;
    bool isPressed{ false };
    bool isHovered{ false };
    QtThemeType::ThemeMode themeMode;
    QtMessageDialogButtonPrivate(QtMessageDialogButton* q);
    ~QtMessageDialogButtonPrivate();
};

QtMessageDialogButtonPrivate::QtMessageDialogButtonPrivate(QtMessageDialogButton* q)
    : q_ptr(q)
{
}
QtMessageDialogButtonPrivate::~QtMessageDialogButtonPrivate()
{
}

QtMessageDialogButton::QtMessageDialogButton(ButtonType type, QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtMessageDialogButtonPrivate(this))
{
    Q_D(QtMessageDialogButton);
    d->themeMode = qtTheme->getThemeMode();
    d->buttonType = type;
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    setCursor(Qt::PointingHandCursor);
}

QtMessageDialogButton::~QtMessageDialogButton()
{
}

void QtMessageDialogButton::paintEvent(QPaintEvent* event)
{
    Q_D(QtMessageDialogButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if(d->isPressed)
    {
        painter.fillRect(rect(), QColor(0, 0, 0, 20));
    }
    else if(d->isHovered)
    {
        painter.fillRect(rect(), QColor(0, 0, 0, 10));
    }

    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.setPen(
        QPen(QtThemeColor(d->themeMode, BasicText), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if(d->buttonType == Confirm)
    {
        QPoint p1(centerX - 6, centerY);
        QPoint p2(centerX - 2, centerY + 4);
        QPoint p3(centerX + 6, centerY - 4);

        painter.drawLine(p1, p2);
        painter.drawLine(p2, p3);
    }
    else
    {
        int offset = 5;
        painter.drawLine(centerX - offset, centerY - offset, centerX + offset, centerY + offset);
        painter.drawLine(centerX - offset, centerY + offset, centerX + offset, centerY - offset);
    }
}

void QtMessageDialogButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtMessageDialogButton);
    if(event->button() == Qt::LeftButton)
    {
        d->isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void QtMessageDialogButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtMessageDialogButton);
    if(event->button() == Qt::LeftButton && d->isPressed)
    {
        d->isPressed = false;
        update();
        if(rect().contains(event->pos()))
        {
            Q_EMIT clicked();
        }
    }
    QWidget::mouseReleaseEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QtMessageDialogButton::enterEvent(QEnterEvent* event)
#else
void QtMessageDialogButton::enterEvent(QEvent* event)
#endif
{
    Q_D(QtMessageDialogButton);
    d->isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void QtMessageDialogButton::leaveEvent(QEvent* event)
{
    Q_D(QtMessageDialogButton);
    d->isHovered = false;
    d->isPressed = false;
    update();
    QWidget::leaveEvent(event);
}

class QtMessageDialogPrivate
{
    Q_DECLARE_PUBLIC(QtMessageDialog)
public:
    int borderRadius;
    QString title;
    QString content;
    int titlePixelSize;
    int contentPixelSize;
    QtMessageDialog* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtMessageDialogButton* confirmButton{ nullptr };
    QtMessageDialogButton* cancelButton{ nullptr };
    QtMessageDialogPrivate(QtMessageDialog* q);
    ~QtMessageDialogPrivate();
};

QtMessageDialogPrivate::QtMessageDialogPrivate(QtMessageDialog* q)
    : q_ptr(q)
{
}
QtMessageDialogPrivate::~QtMessageDialogPrivate()
{
}

QtMessageDialog::QtMessageDialog(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtMessageDialogPrivate(this))
{
    Q_D(QtMessageDialog);
    d->borderRadius = 8;
    d->title = "标题";
    d->content = "";
    d->titlePixelSize = 15;
    d->contentPixelSize = 13;

    setMinimumSize(280, 150);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });

    d->confirmButton = new QtMessageDialogButton(QtMessageDialogButton::Confirm, this);
    d->confirmButton->setFixedHeight(40);
    d->cancelButton = new QtMessageDialogButton(QtMessageDialogButton::Cancel, this);
    d->cancelButton->setFixedHeight(40);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addStretch();

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(d->confirmButton, 1);
    buttonLayout->addWidget(d->cancelButton, 1);

    mainLayout->addLayout(buttonLayout);

    connect(d->confirmButton, &QtMessageDialogButton::clicked, this, [this]() {
        Q_EMIT confirmed();
        hide();
    });
    connect(d->cancelButton, &QtMessageDialogButton::clicked, this, [this]() {
        Q_EMIT cancelled();
        hide();
    });
}

QtMessageDialog::~QtMessageDialog()
{
}

void QtMessageDialog::paintEvent(QPaintEvent* event)
{
    Q_D(QtMessageDialog);
    QPainter painter(this);
    painter.setRenderHints(
        QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, BasicBase));
    painter.drawRoundedRect(rect(), d->borderRadius, d->borderRadius);

    QFont titleFont = this->font();
    titleFont.setPixelSize(d->titlePixelSize);
    titleFont.setWeight(QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QtThemeColor(d->themeMode, BasicText));
    painter.drawText(QRect(15, 15, width() - 30, 25), Qt::AlignLeft | Qt::AlignTop, d->title);

    QFont contentFont = this->font();
    contentFont.setPixelSize(d->contentPixelSize);
    contentFont.setWeight(QFont::Normal);
    painter.setFont(contentFont);
    painter.setPen(QtThemeColor(d->themeMode, BasicText));
    int contentHeight = height() - 40 - 45 - 15;
    painter.drawText(QRect(15, 45, width() - 30, contentHeight),
        Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, d->content);

    painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), 1));
    painter.drawLine(0, height() - 40, width(), height() - 40);
}

int QtMessageDialog::borderRadius() const
{
    Q_D(const QtMessageDialog);
    return d->borderRadius;
}
void QtMessageDialog::setBorderRadius(int value)
{
    Q_D(QtMessageDialog);
    d->borderRadius = value;
}
QString QtMessageDialog::title() const
{
    Q_D(const QtMessageDialog);
    return d->title;
}
void QtMessageDialog::setTitle(QString value)
{
    Q_D(QtMessageDialog);
    d->title = value;
}
QString QtMessageDialog::content() const
{
    Q_D(const QtMessageDialog);
    return d->content;
}
void QtMessageDialog::setContent(QString value)
{
    Q_D(QtMessageDialog);
    d->content = value;
}
int QtMessageDialog::titlePixelSize() const
{
    Q_D(const QtMessageDialog);
    return d->titlePixelSize;
}
void QtMessageDialog::setTitlePixelSize(int value)
{
    Q_D(QtMessageDialog);
    d->titlePixelSize = value;
}
int QtMessageDialog::contentPixelSize() const
{
    Q_D(const QtMessageDialog);
    return d->contentPixelSize;
}
void QtMessageDialog::setContentPixelSize(int value)
{
    Q_D(QtMessageDialog);
    d->contentPixelSize = value;
}