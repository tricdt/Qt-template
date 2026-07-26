#include "qtpushbutton.h"
#include "qtdef.h"
#include "qttheme.h"
class QtPushButtonPrivate
{
    Q_DECLARE_PUBLIC(QtPushButton)
public:
    int borderRadius;
    QColor lightDefaultColor;
    QColor darkDefaultColor;
    QColor lightHoverColor;
    QColor darkHoverColor;
    QColor lightPressColor;
    QColor darkPressColor;
    QtPushButton* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QColor lightTextColor;
    QColor darkTextColor;
    bool isPressed{ false };
    int shadowBorderWidth{ 3 };
    QtPushButtonPrivate(QtPushButton* q);
    ~QtPushButtonPrivate();
};

QtPushButtonPrivate::QtPushButtonPrivate(QtPushButton* q)
    : q_ptr(q)
{
}
QtPushButtonPrivate::~QtPushButtonPrivate()
{
}

QtPushButton::QtPushButton(QWidget* parent)
    : QPushButton{ parent }
    , d_ptr(new QtPushButtonPrivate(this))
{
    Q_D(QtPushButton);
    d->q_ptr = this;
    d->borderRadius = 3;
    d->themeMode = qtTheme->getThemeMode();
    d->lightDefaultColor = QtThemeColor(QtThemeType::Light, BasicBase);
    d->darkDefaultColor = QtThemeColor(QtThemeType::Dark, BasicBase);
    d->lightHoverColor = QtThemeColor(QtThemeType::Light, BasicHover);
    d->darkHoverColor = QtThemeColor(QtThemeType::Dark, BasicHover);
    d->lightPressColor = QtThemeColor(QtThemeType::Light, BasicPress);
    d->darkPressColor = QtThemeColor(QtThemeType::Dark, BasicPress);
    d->lightTextColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkTextColor = QtThemeColor(QtThemeType::Dark, BasicText);
    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("QtPushButton");
    setStyleSheet("#ElaPushButton{background-color:transparent;}");
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtPushButton::QtPushButton(QString text, QWidget* parent)
    : QtPushButton(parent)
{
    setText(text);
}

QtPushButton::~QtPushButton()
{
}

int QtPushButton::borderRadius() const
{
    Q_D(const QtPushButton);
    return d->borderRadius;
}
void QtPushButton::setborderRadius(int value)
{
    Q_D(QtPushButton);
    d->borderRadius = value;
}

QColor QtPushButton::lightDefaultColor() const
{
    Q_D(const QtPushButton);
    return d->lightDefaultColor;
}
void QtPushButton::setLightDefaultColor(QColor value)
{
    Q_D(QtPushButton);
    d->lightDefaultColor = value;
}

QColor QtPushButton::darkDefaultColor() const
{
    Q_D(const QtPushButton);
    return d->darkDefaultColor;
}
void QtPushButton::setdarkDefaultColor(QColor value)
{
    Q_D(QtPushButton);
    d->darkDefaultColor = value;
}

QColor QtPushButton::lightHoverColor() const
{
    Q_D(const QtPushButton);
    return d->lightHoverColor;
}
void QtPushButton::setlightHoverColor(QColor value)
{
    Q_D(QtPushButton);
    d->lightHoverColor = value;
}

QColor QtPushButton::darkHoverColor() const
{
    Q_D(const QtPushButton);
    return d->darkHoverColor;
}
void QtPushButton::setdarkHoverColor(QColor value)
{
    Q_D(QtPushButton);
    d->darkHoverColor = value;
}

QColor QtPushButton::lightPressColor() const
{
    Q_D(const QtPushButton);
    return d->lightPressColor;
}
void QtPushButton::setLightPressColor(QColor value)
{
    Q_D(QtPushButton);
    d->lightPressColor = value;
}

QColor QtPushButton::darkPressColor() const
{
    Q_D(const QtPushButton);
    return d->darkPressColor;
}
void QtPushButton::setDarkPressColor(QColor value)
{
    Q_D(QtPushButton);
    d->darkPressColor = value;
}

void QtPushButton::setLightTextColor(QColor value)
{
    Q_D(QtPushButton);
    d->lightTextColor = value;
}
QColor QtPushButton::lightTextColor() const
{
    Q_D(const QtPushButton);
    return d->lightTextColor;
}

void QtPushButton::setDarkTextColor(QColor value)
{
    Q_D(QtPushButton);
    d->darkTextColor = value;
}
QColor QtPushButton::darkTextColor() const
{
    Q_D(const QtPushButton);
    return d->darkTextColor;
}

void QtPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtPushButton);
    d->isPressed = true;
    QPushButton::mousePressEvent(event);
}

void QtPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtPushButton);
    d->isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void QtPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(QtPushButton);
    QPainter painter(this);
    painter.setRenderHints(
        QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    qtTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);

    painter.save();
    QRect foregroundRect(d->shadowBorderWidth, d->shadowBorderWidth,
        width() - 2 * (d->shadowBorderWidth), height() - 2 * d->shadowBorderWidth);
    if(d->themeMode == QtThemeType::Light)
    {
        painter.setPen(QtThemeColor(QtThemeType::Light, BasicBorder));
        painter.setBrush(
            isEnabled() ? d->isPressed ? d->lightPressColor
                                       : (underMouse() ? d->lightHoverColor : d->lightDefaultColor)
                        : QtThemeColor(d->themeMode, BasicDisable));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? d->isPressed ? d->darkPressColor
                                                    : (underMouse() ? d->darkHoverColor
                                                                    : d->darkDefaultColor)
                                     : QtThemeColor(d->themeMode, BasicDisable));
    }
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    if(!d->isPressed)
    {
        painter.setPen(QtThemeColor(d->themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->borderRadius, height() - d->shadowBorderWidth,
            foregroundRect.width(), height() - d->shadowBorderWidth);
    }
    painter.setPen(
        isEnabled() ? d->themeMode == QtThemeType::Light ? d->lightTextColor : d->darkTextColor
                    : QtThemeColor(d->themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}
