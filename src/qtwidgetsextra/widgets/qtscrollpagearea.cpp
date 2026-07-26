#include "qtscrollpagearea.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qtpreprocessorsupport.h>

class QtScrollPageAreaPrivate
{
    Q_DECLARE_PUBLIC(QtScrollPageArea)
public:
    int borderRadius;
    QtThemeType::ThemeMode themeMode;
    QtScrollPageArea* q_ptr;
    QtScrollPageAreaPrivate(QtScrollPageArea* q);
    ~QtScrollPageAreaPrivate();
};

QtScrollPageAreaPrivate::QtScrollPageAreaPrivate(QtScrollPageArea* q)
    : q_ptr(q)
{
}
QtScrollPageAreaPrivate::~QtScrollPageAreaPrivate()
{
}

QtScrollPageArea::QtScrollPageArea(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtScrollPageAreaPrivate(this))
{
    Q_D(QtScrollPageArea);
    d->borderRadius = 6;
    setFixedHeight(75);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtScrollPageArea::~QtScrollPageArea()
{
}

int QtScrollPageArea::borderRadius() const
{
    Q_D(const QtScrollPageArea);
    return d->borderRadius;
}
void QtScrollPageArea::setBorderRadius(int value)
{
    Q_D(QtScrollPageArea);
    d->borderRadius = value;
}

void QtScrollPageArea::paintEvent(QPaintEvent*)
{
    Q_D(QtScrollPageArea);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    painter.restore();
}