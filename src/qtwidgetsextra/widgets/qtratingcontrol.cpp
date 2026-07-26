#include "qtratingcontrol.h"
#include "qttheme.h"
#include <qevent.h>
class QtRatingControlPrivate
{
    Q_DECLARE_PUBLIC(QtRatingControl)
public:
    int maxRating;
    int rating;
    int starSize;
    int spacing;
    bool isReadOnly;
    QtRatingControl* q_ptr;
    int hoveredStar{ -1 };
    QtThemeType::ThemeMode themeMode;
    QtRatingControlPrivate(QtRatingControl* q);
    ~QtRatingControlPrivate();
};

QtRatingControlPrivate::QtRatingControlPrivate(QtRatingControl* q)
    : q_ptr(q)
{
}
QtRatingControlPrivate::~QtRatingControlPrivate()
{
}

QtRatingControl::QtRatingControl(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtRatingControlPrivate(this))
{
    Q_D(QtRatingControl);
    d->q_ptr = this;
    d->maxRating = 5;
    d->rating = 0;
    d->starSize = 24;
    d->spacing = 4;
    d->isReadOnly = false;
    setObjectName("QtRatingControl");
    setMouseTracking(true);
    setFixedHeight(d->starSize + 4);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtRatingControl::~QtRatingControl()
{
}

void QtRatingControl::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtRatingControl);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(d->starSize - 4);
    painter.setFont(iconFont);

    int effectiveRating = d->rating;
    if(!d->isReadOnly && d->hoveredStar >= 0)
    {
        effectiveRating = d->hoveredStar + 1;
    }

    for(int i = 0; i < d->maxRating; ++i)
    {
        QRect starRect(i * (d->starSize + d->spacing), 2, d->starSize, d->starSize);
        if(i < effectiveRating)
        {
            painter.setPen(QtThemeColor(d->themeMode, PrimaryNormal));
        }
        else
        {
            painter.setPen(QtThemeColor(d->themeMode, BasicBorderDeep));
        }
        painter.drawText(starRect, Qt::AlignCenter, QChar((unsigned short) QtIconType::Star));
    }
}

void QtRatingControl::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtRatingControl);
    if(d->isReadOnly)
    {
        QWidget::mouseMoveEvent(event);
        return;
    }
    int x = event->pos().x();
    int starUnit = d->starSize + d->spacing;
    int hoveredStar = -1;
    if(x >= 0 && starUnit > 0)
    {
        hoveredStar = x / starUnit;
        if(hoveredStar >= d->maxRating)
        {
            hoveredStar = -1;
        }
    }
    if(d->hoveredStar != hoveredStar)
    {
        d->hoveredStar = hoveredStar;
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void QtRatingControl::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtRatingControl);
    if(!d->isReadOnly && d->hoveredStar >= 0)
    {
        setRating(d->hoveredStar + 1);
    }
    QWidget::mousePressEvent(event);
}

void QtRatingControl::leaveEvent(QEvent* event)
{
    Q_D(QtRatingControl);
    d->hoveredStar = -1;
    update();
    QWidget::leaveEvent(event);
}

QSize QtRatingControl::sizeHint() const
{
    Q_D(const QtRatingControl);
    int w = d->maxRating * (d->starSize + d->spacing) - d->spacing;
    int h = d->starSize + 4;
    return QSize(w, h);
}

int QtRatingControl::maxRating() const
{
    Q_D(const QtRatingControl);
    return d->maxRating;
}

void QtRatingControl::setMaxRating(int value)
{
    Q_D(QtRatingControl);
    if(d->maxRating != value)
    {
        d->maxRating = value;
        Q_EMIT maxRatingChanged();
    }
}

int QtRatingControl::rating() const
{
    Q_D(const QtRatingControl);
    return d->rating;
}

void QtRatingControl::setRating(int value)
{
    Q_D(QtRatingControl);
    if(d->rating == value)
    {
        return;
    }
    d->rating = value;
    Q_EMIT ratingChanged();
    Q_EMIT ratingChanged(value);
    update();
}

int QtRatingControl::starSize() const
{
    Q_D(const QtRatingControl);
    return d->starSize;
}

void QtRatingControl::setStarSize(int value)
{
    Q_D(QtRatingControl);
    if(d->starSize != value)
    {
        d->starSize = value;
        Q_EMIT starSizeChanged();
    }
}

int QtRatingControl::spacing() const
{
    Q_D(const QtRatingControl);
    return d->spacing;
}

void QtRatingControl::setSpacing(int value)
{
    Q_D(QtRatingControl);
    if(d->spacing != value)
    {
        d->spacing = value;
        Q_EMIT spacingChanged();
    }
}

bool QtRatingControl::isReadOnly() const
{
    Q_D(const QtRatingControl);
    return d->isReadOnly;
}

void QtRatingControl::setIsReadOnly(bool value)
{
    Q_D(QtRatingControl);
    if(d->isReadOnly != value)
    {
        d->isReadOnly = value;
        Q_EMIT isReadOnlyChanged();
    }
}