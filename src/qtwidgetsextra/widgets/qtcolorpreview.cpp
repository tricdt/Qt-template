#include "qtcolorpreview.h"
#include "qtdef.h"
#include "qttheme.h"

class QtColorPreviewPrivate
{
    Q_DECLARE_PUBLIC(QtColorPreview)
public:
    QColor baseColor;
    QtColorPreview* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtColorPreviewPrivate(QtColorPreview* q);
    ~QtColorPreviewPrivate();
};

QtColorPreviewPrivate::QtColorPreviewPrivate(QtColorPreview* q)
    : q_ptr(q)
{
}
QtColorPreviewPrivate::~QtColorPreviewPrivate()
{
}

QtColorPreview::QtColorPreview(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtColorPreviewPrivate(this))
{
    Q_D(QtColorPreview);
    setFixedSize(50, 260);
    d->themeMode = qtTheme->getThemeMode();
}

QtColorPreview::~QtColorPreview()
{
}

QColor QtColorPreview::baseColor() const
{
    Q_D(const QtColorPreview);
    return d->baseColor;
}
void QtColorPreview::setBaseColor(QColor value)
{
    Q_D(QtColorPreview);
    d->baseColor = value;
}

void QtColorPreview::paintEvent(QPaintEvent* event)
{
    Q_D(QtColorPreview);
    int penWidth = 3;
    int borderRadius = 5;
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    QRect viewRect = rect();
    viewRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
    painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), penWidth));
    painter.setBrush(d->baseColor);
    painter.drawRoundedRect(viewRect, borderRadius, borderRadius);
    painter.restore();
}
