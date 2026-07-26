#include "qticonbutton.h"
#include "qttheme.h"
#include <QVariantAnimation>
#include <qvariant.h>

class QtIconButtonPrivate
{
    Q_DECLARE_PUBLIC(QtIconButton)
public:
    QtIconButton* q_ptr;
    QtThemeType::ThemeMode themeMode;
    int borderRadius;
    qreal opacity;
    QColor lightHoverColor;
    QColor darkHoverColor;
    QColor lightIconColor;
    QColor darkIconColor;
    QColor lightHoverIconColor;
    QColor darkHoverIconColor;
    bool isSelected;
    int hoverAlpha;
    QtIconType::IconName awesome;
    QPixmap iconPix;
    bool isAlphaAnimationFinished{ true };
    QtIconButtonPrivate(QtIconButton* q);
};

QtIconButtonPrivate::QtIconButtonPrivate(QtIconButton* q)
    : q_ptr(q)
{
}

QtIconButton::QtIconButton(QPixmap pix, QWidget* parent)
    : QPushButton(parent)
    , d_ptr(new QtIconButtonPrivate(this))
{
    Q_D(QtIconButton);
    d->iconPix = pix.copy();
    d->hoverAlpha = 0;
    d->opacity = 1;
    d->lightHoverColor = QtThemeColor(QtThemeType::Light, BasicHoverAlpha);
    d->darkHoverColor = QtThemeColor(QtThemeType::Dark, BasicHoverAlpha);
    d->lightIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->lightHoverIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkHoverIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->isSelected = false;
    d->borderRadius = 0;
    d->themeMode = qtTheme->getThemeMode();
    connect(this, &QtIconButton::isSelectedChanged, this, [this]() { update(); });
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [d](QtThemeType::ThemeMode themeMode) { d->themeMode = themeMode; });
}

QtIconButton::QtIconButton(QtIconType::IconName awesome, QWidget* parent)
    : QPushButton(parent)
    , d_ptr(new QtIconButtonPrivate(this))
{
    Q_D(QtIconButton);
    d->hoverAlpha = 0;
    d->opacity = 1;
    d->lightHoverColor = QtThemeColor(QtThemeType::Light, BasicHoverAlpha);
    d->darkHoverColor = QtThemeColor(QtThemeType::Dark, BasicHoverAlpha);
    d->lightIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->lightHoverIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkHoverIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->isSelected = false;
    d->borderRadius = 0;
    d->themeMode = qtTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(15);
    this->setFont(iconFont);
    d->awesome = awesome;
    this->setText(QChar(static_cast<ushort>(awesome)));
    connect(this, &QtIconButton::isSelectedChanged, this, [this]() { update(); });
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [d](QtThemeType::ThemeMode themeMode) { d->themeMode = themeMode; });
}
QtIconButton::QtIconButton(QtIconType::IconName awesome, int pixelSize, QWidget* parent)
    : QPushButton(parent)
    , d_ptr(new QtIconButtonPrivate(this))
{
    Q_D(QtIconButton);
    d->hoverAlpha = 0;
    d->opacity = 1;
    d->lightHoverColor = QtThemeColor(QtThemeType::Light, BasicHoverAlpha);
    d->darkHoverColor = QtThemeColor(QtThemeType::Dark, BasicHoverAlpha);
    d->lightIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->lightHoverIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkHoverIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->isSelected = false;
    d->borderRadius = 0;
    d->themeMode = qtTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->awesome = awesome;
    this->setText(QChar(static_cast<ushort>(awesome)));
    connect(this, &QtIconButton::isSelectedChanged, this, [this]() { update(); });
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [d](QtThemeType::ThemeMode themeMode) { d->themeMode = themeMode; });
}
QtIconButton::QtIconButton(
    QtIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent)
    : QPushButton(parent)
    , d_ptr(new QtIconButtonPrivate(this))
{
    Q_D(QtIconButton);
    d->hoverAlpha = 0;
    d->opacity = 1;
    d->lightHoverColor = QtThemeColor(QtThemeType::Light, BasicHoverAlpha);
    d->darkHoverColor = QtThemeColor(QtThemeType::Dark, BasicHoverAlpha);
    d->lightIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->lightHoverIconColor = QtThemeColor(QtThemeType::Light, BasicText);
    d->darkHoverIconColor = QtThemeColor(QtThemeType::Dark, BasicText);
    d->isSelected = false;
    d->borderRadius = 0;
    d->themeMode = qtTheme->getThemeMode();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->awesome = awesome;
    this->setText(QChar(static_cast<ushort>(awesome)));
    this->setFixedSize(fixedWidth, fixedHeight);
    connect(this, &QtIconButton::isSelectedChanged, this, [this]() { update(); });
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [d](QtThemeType::ThemeMode themeMode) { d->themeMode = themeMode; });
}

QtIconButton::~QtIconButton()
{
}

int QtIconButton::borderRadius() const
{
    Q_D(const QtIconButton);
    return d->borderRadius;
}

void QtIconButton::setBorderRadius(int newBorderRadius)
{
    Q_D(QtIconButton);
    if(d->borderRadius == newBorderRadius)
        return;
    d->borderRadius = newBorderRadius;
    Q_EMIT borderRadiusChanged();
}

qreal QtIconButton::opacity() const
{
    Q_D(const QtIconButton);
    return d->opacity;
}

void QtIconButton::setOpacity(qreal newOpacity)
{
    Q_D(QtIconButton);
    if(qFuzzyCompare(d->opacity, newOpacity))
        return;
    d->opacity = newOpacity;
    Q_EMIT opacityChanged();
}

QColor QtIconButton::lightHoverColor() const
{
    Q_D(const QtIconButton);
    return d->lightHoverColor;
}

void QtIconButton::setLightHoverColor(const QColor& newLightHoverColor)
{
    Q_D(QtIconButton);
    if(d->lightHoverColor == newLightHoverColor)
        return;
    d->lightHoverColor = newLightHoverColor;
    Q_EMIT lightHoverColorChanged();
}

QColor QtIconButton::darkHoverColor() const
{
    Q_D(const QtIconButton);
    return d->darkHoverColor;
}

void QtIconButton::setDarkHoverColor(const QColor& newdarkHoverColor)
{
    Q_D(QtIconButton);
    if(d->darkHoverColor == newdarkHoverColor)
        return;
    d->darkHoverColor = newdarkHoverColor;
    Q_EMIT darkHoverColorChanged();
}

QColor QtIconButton::lightIconColor() const
{
    Q_D(const QtIconButton);
    return d->lightIconColor;
}

void QtIconButton::setLightIconColor(const QColor& newlightIconColor)
{
    Q_D(QtIconButton);
    if(d->lightIconColor == newlightIconColor)
        return;
    d->lightIconColor = newlightIconColor;
    Q_EMIT lightIconColorChanged();
}

QColor QtIconButton::darkIconColor() const
{
    Q_D(const QtIconButton);
    return d->darkIconColor;
}

void QtIconButton::setDarkIconColor(const QColor& newDarkIconColor)
{
    Q_D(QtIconButton);
    if(d->darkIconColor == newDarkIconColor)
        return;
    d->darkIconColor = newDarkIconColor;
    Q_EMIT darkIconColorChanged();
}

QColor QtIconButton::lightHoverIconColor() const
{
    Q_D(const QtIconButton);
    return d->lightHoverIconColor;
}

void QtIconButton::setLightHoverIconColor(const QColor& newLightHoverIconColor)
{
    Q_D(QtIconButton);
    if(d->lightHoverIconColor == newLightHoverIconColor)
        return;
    d->lightHoverIconColor = newLightHoverIconColor;
    Q_EMIT lightHoverIconColorChanged();
}

QColor QtIconButton::darkHoverIconColor() const
{
    Q_D(const QtIconButton);
    return d->darkHoverIconColor;
}

void QtIconButton::setDarkHoverIconColor(const QColor& newDarkHoverIconColor)
{
    Q_D(QtIconButton);
    if(d->darkHoverIconColor == newDarkHoverIconColor)
        return;
    d->darkHoverIconColor = newDarkHoverIconColor;
    Q_EMIT darkHoverIconColorChanged();
}

bool QtIconButton::isSelected() const
{
    Q_D(const QtIconButton);
    return d->isSelected;
}

void QtIconButton::setIsSelected(bool newIsSelected)
{
    Q_D(QtIconButton);
    if(d->isSelected == newIsSelected)
        return;
    d->isSelected = newIsSelected;
    Q_EMIT isSelectedChanged();
}

void QtIconButton::setAwesome(QtIconType::IconName awesome)
{
    Q_D(QtIconButton);
    if(d->awesome == awesome)
        return;
    d->awesome = awesome;
    this->setText(QChar(static_cast<ushort>(awesome)));
}
QtIconType::IconName QtIconButton::awesome() const
{
    Q_D(const QtIconButton);
    return d->awesome;
}

void QtIconButton::setPixmap(QPixmap pix)
{
    Q_D(QtIconButton);
    d->iconPix = pix;
}

bool QtIconButton::event(QEvent* event)
{
    Q_D(QtIconButton);
    switch(event->type())
    {
        case QEvent::Enter: {
            if(isEnabled() && !d->isSelected)
            {
                d->isAlphaAnimationFinished = false;
                QVariantAnimation* alphaAnimation = new QVariantAnimation(this);
                connect(alphaAnimation, &QVariantAnimation::valueChanged, this,
                    [this](const QVariant& value) {
                        this->d_ptr->hoverAlpha = value.toInt();
                        update();
                    });
                connect(alphaAnimation, &QVariantAnimation::finished, this,
                    [d]() { d->isAlphaAnimationFinished = true; });
                alphaAnimation->setDuration(175);
                alphaAnimation->setStartValue(d->hoverAlpha);
                alphaAnimation->setEndValue(d->themeMode == QtThemeType::Light
                                                ? d->lightHoverColor.alpha()
                                                : d->darkHoverColor.alpha());
                alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }
            break;
        }
        case QEvent::Leave: {
            if(isEnabled() && !d->isSelected)
            {
                d->isAlphaAnimationFinished = false;
                QVariantAnimation* alphaAnimation = new QVariantAnimation(this);
                connect(alphaAnimation, &QVariantAnimation::valueChanged, this,
                    [this](const QVariant& value) {
                        this->d_ptr->hoverAlpha = value.toInt();
                        update();
                    });
                connect(alphaAnimation, &QVariantAnimation::finished, this,
                    [d]() { d->isAlphaAnimationFinished = true; });
                alphaAnimation->setDuration(175);
                alphaAnimation->setStartValue(d->hoverAlpha);
                alphaAnimation->setEndValue(0);
                alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }
            break;
        }
        default: {
            break;
        }
    }
    return QPushButton::event(event);
}
void QtIconButton::paintEvent(QPaintEvent*)
{
    Q_D(QtIconButton);
    QPainter painter(this);
    painter.save();
    painter.setOpacity(d->opacity);
    painter.setRenderHints(
        QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    if(d->isAlphaAnimationFinished || d->isSelected)
    {
        painter.setBrush(
            d->isSelected
                ? d->themeMode == QtThemeType::Light ? d->lightHoverColor : d->darkHoverColor
            : isEnabled() ? underMouse() ? d->themeMode == QtThemeType::Light ? d->lightHoverColor
                                                                              : d->darkHoverColor
                                         : Qt::transparent
                          : Qt::transparent);
    }
    else
    {
        QColor hoverColor =
            d->themeMode == QtThemeType::Light ? d->lightHoverColor : d->darkHoverColor;
        hoverColor.setAlpha(d->hoverAlpha);
        painter.setBrush(hoverColor);
    }
    painter.drawRoundedRect(rect(), d->borderRadius, d->borderRadius);
    // 图标绘制
    if(!d->iconPix.isNull())
    {
        QPainterPath path;
        path.addEllipse(rect());
        painter.setClipPath(path);
        painter.drawPixmap(rect(), d->iconPix);
    }
    else
    {
        painter.setPen(isEnabled() ? d->themeMode == QtThemeType::Light
                                         ? underMouse() ? d->lightHoverIconColor : d->lightIconColor
                                     : underMouse() ? d->darkHoverIconColor
                                                    : d->darkIconColor
                                   : QtThemeColor(d->themeMode, BasicTextDisable));
        painter.drawText(rect(), Qt::AlignCenter, QChar(static_cast<ushort>(d->awesome)));
    }
    painter.restore();
}