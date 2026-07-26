#include "widgets/qtmessagebutton.h"
#include "qttheme.h"
#include "widgets/qtmessagebar.h"
#include <qevent.h>
class QtMessageButtonPrivate
{
    Q_DECLARE_PUBLIC(QtMessageButton)
public:
    int borderRadius;
    QString barTitle;
    QString barText;
    int displayMsec;
    QWidget* messageTargetWidget;
    QtMessageBarType::MessageMode messageMode;
    QtMessageBarType::PositionPolicy positionPolicy;

    bool isLeftButtonPress;
    int shadowBorderWidth;
    int penBorderWidth;
    QtThemeType::ThemeMode themeMode;

    QtMessageButton* q_ptr;
    QtMessageButtonPrivate(QtMessageButton* q);
    ~QtMessageButtonPrivate();
};

QtMessageButtonPrivate::QtMessageButtonPrivate(QtMessageButton* q)
    : isLeftButtonPress(false)
    , shadowBorderWidth(3)
    , penBorderWidth(1)
    , q_ptr(q)
{
}
QtMessageButtonPrivate::~QtMessageButtonPrivate()
{
}

QtMessageButton::QtMessageButton(QWidget* parent)
    : QPushButton{ parent }
    , d_ptr(new QtMessageButtonPrivate(this))
{
    Q_D(QtMessageButton);
    d->borderRadius = 3;
    setMouseTracking(true);
    setFixedSize(80, 38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setText("Message");
    setObjectName("QtMessageButton");
    setStyleSheet("#QtMessageButton{background-color:transparent;}");
    d->displayMsec = 2000;
    d->messageMode = QtMessageBarType::Success;
    d->positionPolicy = QtMessageBarType::TopRight;
    d->themeMode = qtTheme->getThemeMode();
    d->messageTargetWidget = nullptr;
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
    connect(this, &QtMessageButton::clicked, this, [this]() {
        switch(d_ptr->messageMode)
        {
            case QtMessageBarType::Success: {
                QtMessageBar::success(d_ptr->positionPolicy, d_ptr->barTitle, d_ptr->barText,
                    d_ptr->displayMsec, d_ptr->messageTargetWidget);
                break;
            }
            case QtMessageBarType::Warning: {
                QtMessageBar::warning(d_ptr->positionPolicy, d_ptr->barTitle, d_ptr->barText,
                    d_ptr->displayMsec, d_ptr->messageTargetWidget);
                break;
            }
            case QtMessageBarType::Information: {
                QtMessageBar::information(d_ptr->positionPolicy, d_ptr->barTitle, d_ptr->barText,
                    d_ptr->displayMsec, d_ptr->messageTargetWidget);
                break;
            }
            case QtMessageBarType::Error: {
                QtMessageBar::error(d_ptr->positionPolicy, d_ptr->barTitle, d_ptr->barText,
                    d_ptr->displayMsec, d_ptr->messageTargetWidget);
                break;
            }
        }
    });
}

QtMessageButton::QtMessageButton(QString text, QWidget* parent)
    : QtMessageButton(parent)
{
    setText(text);
}

QtMessageButton::~QtMessageButton()
{
}

int QtMessageButton::borderRadius() const
{
    Q_D(const QtMessageButton);
    return d->borderRadius;
}
void QtMessageButton::setBorderRadius(int value)
{
    Q_D(QtMessageButton);
    d->borderRadius = value;
}

QString QtMessageButton::barTitle() const
{
    Q_D(const QtMessageButton);
    return d->barTitle;
}
void QtMessageButton::setBarTitle(const QString& value)
{
    Q_D(QtMessageButton);
    d->barTitle = value;
}

QString QtMessageButton::barText() const
{
    Q_D(const QtMessageButton);
    return d->barText;
}
void QtMessageButton::setBarText(const QString& value)
{
    Q_D(QtMessageButton);
    d->barText = value;
}

int QtMessageButton::displayMsec() const
{
    Q_D(const QtMessageButton);
    return d->displayMsec;
}
void QtMessageButton::setDisplayMsec(int value)
{
    Q_D(QtMessageButton);
    d->displayMsec = value;
}

QWidget* QtMessageButton::messageTargetWidget() const
{
    Q_D(const QtMessageButton);
    return d->messageTargetWidget;
}
void QtMessageButton::setMessageTargetWidget(QWidget* value)
{
    Q_D(QtMessageButton);
    d->messageTargetWidget = value;
}

QtMessageBarType::MessageMode QtMessageButton::messageMode() const
{
    Q_D(const QtMessageButton);
    return d->messageMode;
}
void QtMessageButton::setMessageMode(QtMessageBarType::MessageMode value)
{
    Q_D(QtMessageButton);
    d->messageMode = value;
}

QtMessageBarType::PositionPolicy QtMessageButton::positionPolicy() const
{
    Q_D(const QtMessageButton);
    return d->positionPolicy;
}
void QtMessageButton::setPositionPolicy(QtMessageBarType::PositionPolicy value)
{
    Q_D(QtMessageButton);
    d->positionPolicy = value;
}

void QtMessageButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtMessageButton);
    if(event->button() == Qt::LeftButton)
    {
        d->isLeftButtonPress = true;
    }
    QPushButton::mousePressEvent(event);
}
void QtMessageButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtMessageButton);
    d->isLeftButtonPress = false;
    QPushButton::mouseReleaseEvent(event);
}
void QtMessageButton::paintEvent(QPaintEvent*)
{
    Q_D(QtMessageButton);
    QPainter painter(this);
    painter.setRenderHints(
        QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    qtTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);

    painter.save();
    QRect foregroundRect(d->penBorderWidth + d->shadowBorderWidth,
        d->penBorderWidth + d->shadowBorderWidth,
        width() - 2 * (d->penBorderWidth + d->shadowBorderWidth),
        height() - 2 * (d->penBorderWidth + d->shadowBorderWidth));
    painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), d->penBorderWidth));
    painter.setBrush(isEnabled() ? underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                                : QtThemeColor(d->themeMode, BasicBase)
                                 : QtThemeColor(d->themeMode, BasicDisable));
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);

    painter.setPen(isEnabled() ? d->isLeftButtonPress ? QtThemeColor(d->themeMode, BasicTextPress)
                                                      : QtThemeColor(d->themeMode, BasicText)
                               : QtThemeColor(d->themeMode, BasicTextDisable));
    painter.drawText(rect(), Qt::AlignCenter, text());
    painter.restore();
}