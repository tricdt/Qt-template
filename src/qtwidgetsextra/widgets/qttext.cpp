#include "qttext.h"
#include <QChar>
#include <QFont>
#include <QPainter>
#include "qttheme.h"

class QtTextPrivate
{
    Q_DECLARE_PUBLIC(QtText);

public:
    QtText* q_ptr;
    bool isWrapAnywhere{ false };
    QtThemeType::ThemeMode themeMode;
    qreal textSpacing{ 0.5 };
    QtTextType::TextStyle textStyle;
    QtIconType::IconName qtIcon;
    QtTextPrivate(QtText* q);
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtTextPrivate::QtTextPrivate(QtText* q)
    : q_ptr(q)
{
}

void QtTextPrivate::onThemeChanged(QtThemeType::ThemeMode theme)
{
    Q_Q(QtText);
    this->themeMode = theme;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, QtThemeColor(this->themeMode, BasicText));
    q->setPalette(palette);
}

QtText::QtText(QWidget* parent)
    : QLabel(parent)
    , d_ptr(new QtTextPrivate(this))
{
    Q_D(QtText);
    d->textStyle = QtTextType::NoStyle;
    d->qtIcon = QtIconType::None;
    setObjectName("QtText");
    setStyleSheet("#QtText{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
    textFont.setPixelSize(28);
    setFont(textFont);
    setWordWrap(true);
    d->themeMode = qtTheme->getThemeMode();
    d->onThemeChanged(d->themeMode);
    connect(qtTheme, &QtTheme::themeModeChanged,
        [=](QtThemeType::ThemeMode themeMode) { d->onThemeChanged(themeMode); });
}

QtText::QtText(QString text, QWidget* parent)
    : QtText(parent)
{
    setText(text);
}
QtText::QtText(QString text, int pixelSize, QWidget* parent)
    : QtText(text, parent)
{
    QFont font = this->font();
    font.setPixelSize(pixelSize);
    setFont(font);
}

QtText::~QtText()
{
}

bool QtText::isWrapAnyWhere() const
{
    Q_D(const QtText);
    return d->isWrapAnywhere;
}

void QtText::setIsWrapAnyWhere(bool value)
{
    Q_D(QtText);
    setWordWrap(value);
    if(d->isWrapAnywhere == value)
        return;
    d->isWrapAnywhere = value;
    Q_EMIT isWrapAnyWhereChanged();
}

int QtText::textPixelSize() const
{
    return this->font().pixelSize();
}

void QtText::setTextPixelSize(int value)
{
    QFont font = this->font();
    font.setPixelSize(value);
    setFont(font);
    Q_EMIT textPixelSizeChanged();
}

QtTextType::TextStyle QtText::textStyle() const
{
    Q_D(const QtText);
    return d->textStyle;
}

void QtText::setTextStyle(QtTextType::TextStyle value)
{
    Q_D(QtText);
    if(d->textStyle == value)
        return;
    d->textStyle = value;
    QFont textFont = font();
    switch(d->textStyle)
    {
        case QtTextType::NoStyle: {
            break;
        }
        case QtTextType::Caption: {
            textFont.setPixelSize(12);
            break;
        }
        case QtTextType::Body: {
            textFont.setPixelSize(13);
            break;
        }
        case QtTextType::BodyStrong: {
            textFont.setPixelSize(13);
            textFont.setWeight(QFont::DemiBold);
            break;
        }
        case QtTextType::Subtitle: {
            textFont.setPixelSize(20);
            textFont.setWeight(QFont::DemiBold);
            break;
        }
        case QtTextType::Title: {
            textFont.setPixelSize(28);
            textFont.setWeight(QFont::DemiBold);
            break;
        }
        case QtTextType::TitleLarge: {
            textFont.setPixelSize(40);
            textFont.setWeight(QFont::DemiBold);
            break;
        }
        case QtTextType::Display: {
            textFont.setPixelSize(48);
            textFont.setWeight(QFont::DemiBold);
            break;
        }
    }
    setFont(textFont);
    Q_EMIT textStyleChanged();
}

QtIconType::IconName QtText::qtIcon() const
{
    Q_D(const QtText);
    return d->qtIcon;
}

void QtText::setQtIcon(QtIconType::IconName value)
{
    Q_D(QtText);
    if(d->qtIcon == value)
        return;
    d->qtIcon = value;
    update();
    Q_EMIT qtIconChanged();
}

int QtText::textPointSize() const
{
    return this->font().pointSize();
}

void QtText::setTextPointSize(int value)
{
    QFont font = this->font();
    font.setPointSize(value);
    Q_EMIT textPointSizeChanged();
}

void QtText::paintEvent(QPaintEvent* event)
{
    Q_D(QtText);
    if(palette().color(QPalette::WindowText) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    if(d->qtIcon != QtIconType::None)
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(
            QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(this->font().pixelSize());
        painter.setFont(iconFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        painter.drawText(rect(), Qt::AlignCenter, QChar(static_cast<char16_t>(d->qtIcon)));
        painter.restore();
    }
    else
    {
        if(wordWrap() && d->isWrapAnywhere)
        {
            QPainter painter(this);
            painter.save();
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
            painter.setPen(QtThemeColor(d->themeMode, BasicText));
            painter.drawText(rect(),
                Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere, text());
            painter.restore();
        }
        else
        {
            QLabel::paintEvent(event);
        }
    }
}
