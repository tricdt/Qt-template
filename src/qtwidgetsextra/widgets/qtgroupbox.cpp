#include "qtgroupbox.h"
#include "qttheme.h"
#include "styles/qtgroupboxstyle.h"
class QtGroupBoxPrivate
{
    Q_DECLARE_PUBLIC(QtGroupBox)
public:
    int borderRadius {6};
    QtGroupBox* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtGroupBoxPrivate(QtGroupBox* q);
    ~QtGroupBoxPrivate();
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtGroupBoxPrivate::QtGroupBoxPrivate(QtGroupBox* q)
    : q_ptr(q)
{
}
QtGroupBoxPrivate::~QtGroupBoxPrivate()
{
}

void QtGroupBoxPrivate::onThemeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtGroupBox);
    this->themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, QtThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}

QtGroupBox::QtGroupBox(QWidget* parent)
    : QGroupBox{ parent }
    , d_ptr(new QtGroupBoxPrivate(this))
{
    Q_D(QtGroupBox);

    setMouseTracking(true);
    setObjectName("QtGroupBox");

    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);

    setStyle(new QtGroupBoxStyle(style()));
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
}

QtGroupBox::QtGroupBox(const QString& title, QWidget* parent)
    : QtGroupBox(parent)
{
    Q_D(QtGroupBox);
    setTitle(title);
}

QtGroupBox::~QtGroupBox()
{
    delete this->style();
}

int QtGroupBox::borderRadius() const
{
    Q_D(const QtGroupBox);
    return d->borderRadius;
}
void QtGroupBox::setBorderRadius(int value)
{
    Q_D(QtGroupBox);
    d->borderRadius = value;
}

void QtGroupBox::paintEvent(QPaintEvent* event)
{
    Q_D(QtGroupBox);
    if(palette().color(QPalette::WindowText) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QGroupBox::paintEvent(event);
}