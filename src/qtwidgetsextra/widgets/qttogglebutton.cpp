#include "qttogglebutton.h"
#include "qttheme.h"
#include <qcoreevent.h>
#include <qpropertyanimation.h>
#include <qvariantanimation.h>
class QtToggleButtonPrivate
{
    Q_DECLARE_PUBLIC(QtToggleButton)
public:
    int borderRadius;
    QString text;
    int toggleAlpha;

    bool isAlphaAnimationFinished;
    bool isToggled;
    bool isPressed;
    QtThemeType::ThemeMode themeMode;

    QtToggleButton* q_ptr;
    QtToggleButtonPrivate(QtToggleButton* q);
    ~QtToggleButtonPrivate();
};

QtToggleButtonPrivate::QtToggleButtonPrivate(QtToggleButton* q)
    : isAlphaAnimationFinished(false)
    , isToggled(false)
    , isPressed(false)
    , q_ptr(q)
{
}
QtToggleButtonPrivate::~QtToggleButtonPrivate()
{
}

QtToggleButton::QtToggleButton(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtToggleButtonPrivate(this))
{
    Q_D(QtToggleButton);
    d->borderRadius = 3;
    d->themeMode = qtTheme->getThemeMode();
    d->toggleAlpha = 0;
    setMouseTracking(true);
    setFixedSize(80, 32);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("QtToggleButton");
    setStyleSheet("#QtToggleButton{background-color:transparent;}");
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtToggleButton::QtToggleButton(QString text, QWidget* parent)
    : QtToggleButton(parent)
{
    Q_D(QtToggleButton);
    d->text = text;
}

QtToggleButton::~QtToggleButton()
{
}

int QtToggleButton::borderRadius() const
{
    Q_D(const QtToggleButton);
    return d->borderRadius;
}
void QtToggleButton::setBorderRadius(int radius)
{
    Q_D(QtToggleButton);
    d->borderRadius = radius;
}

QString QtToggleButton::text() const
{
    Q_D(const QtToggleButton);
    return d->text;
}
void QtToggleButton::setText(const QString& text)
{
    Q_D(QtToggleButton);
    d->text = text;
}

void QtToggleButton::setIsToggled(bool isToggled)
{
    Q_D(QtToggleButton);
    d->isToggled = isToggled;
    d->toggleAlpha = isToggled ? 255 : 0;
    d->isAlphaAnimationFinished = true;
    update();
    Q_EMIT toggled(isToggled);
}
bool QtToggleButton::isToggled() const
{
    Q_D(const QtToggleButton);
    return d->isToggled;
}

bool QtToggleButton::event(QEvent* event)
{
    Q_D(QtToggleButton);
    switch(event->type())
    {
        case QEvent::Enter:
        case QEvent::Leave: {
            update();
            break;
        }
        default: {
            break;
        }
    }
    return QWidget::event(event);
}
void QtToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtToggleButton);
    d->isPressed = true;
    update();
    QWidget::mouseReleaseEvent(event);
}
void QtToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtToggleButton);
    d->isPressed = false;
    d->isAlphaAnimationFinished = false;
    d->isToggled = !d->isToggled;
    QVariantAnimation* alphaAnimation = new QVariantAnimation(this);
    connect(alphaAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        d_ptr->toggleAlpha = value.toInt();
        update();
    });
    connect(alphaAnimation, &QPropertyAnimation::finished, this,
        [=]() { d->isAlphaAnimationFinished = true; });
    alphaAnimation->setDuration(250);
    alphaAnimation->setStartValue(d->toggleAlpha);
    if(d->isToggled)
    {
        alphaAnimation->setEndValue(255);
    }
    else
    {
        alphaAnimation->setEndValue(0);
    }
    alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    Q_EMIT this->toggled(d->isToggled);
    QWidget::mouseReleaseEvent(event);
}
void QtToggleButton::paintEvent(QPaintEvent* event)
{
    Q_D(QtToggleButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    if(d->isAlphaAnimationFinished)
    {
        if(d->isToggled)
        {
            painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
            painter.setBrush(isEnabled()
                                 ? d->isPressed
                                       ? QtThemeColor(d->themeMode, PrimaryPress)
                                       : (underMouse() ? QtThemeColor(d->themeMode, PrimaryHover)
                                                       : QtThemeColor(d->themeMode, PrimaryNormal))
                                 : QtThemeColor(d->themeMode, BasicDisable));
        }
        else
        {
            painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
            painter.setBrush(
                isEnabled() ? d->isPressed ? QtThemeColor(d->themeMode, BasicPress)
                                           : (underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                                           : QtThemeColor(d->themeMode, BasicBase))
                            : QtThemeColor(d->themeMode, BasicDisable));
        }
    }
    else
    {
        painter.setPen(Qt::NoPen);
        QColor toggleColor = QtThemeColor(d->themeMode, PrimaryNormal);
        toggleColor.setAlpha(d->toggleAlpha);
        painter.setBrush(toggleColor);
    }
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    // 底边线绘制
    if(!d->isPressed && !d->isToggled)
    {
        painter.setPen(QtThemeColor(d->themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->borderRadius, height() - 1,
            foregroundRect.x() + foregroundRect.width() - d->borderRadius, height() - 1);
    }

    // 文字绘制
    painter.setPen(isEnabled() ? d->isToggled ? QtThemeColor(d->themeMode, BasicTextInvert)
                                              : QtThemeColor(d->themeMode, BasicText)
                               : QtThemeColor(d->themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, d->text);
    painter.restore();
}