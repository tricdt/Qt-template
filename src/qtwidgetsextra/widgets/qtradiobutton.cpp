#include "qtradiobutton.h"
#include "qttheme.h"
#include "styles/qtradiobuttonstyle.h"

class QtRadioButtonPrivate
{
    Q_DECLARE_PUBLIC(QtRadioButton)
public:
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
    QtRadioButton* q_ptr;
    QtRadioButtonPrivate(QtRadioButton* q);
    ~QtRadioButtonPrivate();
    QtThemeType::ThemeMode themeMode;
};

QtRadioButtonPrivate::QtRadioButtonPrivate(QtRadioButton* q)
    : q_ptr(q)
{
}
QtRadioButtonPrivate::~QtRadioButtonPrivate()
{
}

void QtRadioButtonPrivate::onThemeChanged(QtThemeType::ThemeMode _themeMode)
{
    Q_Q(QtRadioButton);
    themeMode = _themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, QtThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}

QtRadioButton::QtRadioButton(QWidget* parent)
    : QRadioButton{ parent }
    , d_ptr(new QtRadioButtonPrivate(this))
{
    Q_D(QtRadioButton);
    setFixedHeight(20);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setStyle(new QtRadioButtonStyle(style()));
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](const QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
}

QtRadioButton::QtRadioButton(const QString& text, QWidget* parent)
    : QtRadioButton(parent)
{
    setText(text);
}

QtRadioButton::~QtRadioButton()
{
    delete this->style();
}

void QtRadioButton::paintEvent(QPaintEvent* event)
{
    Q_D(QtRadioButton);
    if(palette().color(QPalette::WindowText) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QRadioButton::paintEvent(event);
}
