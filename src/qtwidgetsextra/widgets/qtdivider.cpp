#include "qtdivider.h"
#include "qttheme.h"
class QtDividerPrivate
{
    Q_DECLARE_PUBLIC(QtDivider)
public:
    Qt::Orientation orientation;
    QString text;
    int contentPosition;
    QtDivider* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtDividerPrivate(QtDivider* q);
    ~QtDividerPrivate();
};

QtDividerPrivate::QtDividerPrivate(QtDivider* q)
    : q_ptr(q)
{
}
QtDividerPrivate::~QtDividerPrivate()
{
}

QtDivider::QtDivider(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtDividerPrivate(this))
{
    Q_D(QtDivider);
    d->orientation = Qt::Horizontal;
    d->text = "";
    d->contentPosition = Center;
    setObjectName("QtDivider");
    setFixedHeight(20);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    connect(this, &QtDivider::orientationChanged, this, [this]() {
        if(d_ptr->orientation == Qt::Horizontal)
        {
            setFixedHeight(20);
            setMinimumWidth(0);
            setMaximumWidth(QWIDGETSIZE_MAX);
            setMaximumHeight(20);
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        }
        else
        {
            setFixedWidth(20);
            setMinimumHeight(0);
            setMaximumHeight(QWIDGETSIZE_MAX);
            setMaximumWidth(20);
            setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        }
        updateGeometry();
        update();
    });
    connect(this, &QtDivider::textChanged, this, [this]() { update(); });
    connect(this, &QtDivider::contentPositionChanged, this, [this]() { update(); });
}

QtDivider::QtDivider(const QString& text, QWidget* parent)
    : QtDivider(parent)
{
    Q_D(QtDivider);
    d->text = text;
}

QtDivider::~QtDivider()
{
}

void QtDivider::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtDivider);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QColor lineColor = QtThemeColor(d->themeMode, BasicBorder);
    QPen linePen(lineColor, 1);

    if(d->orientation == Qt::Vertical)
    {
        // Vertical line at horizontal center
        int centerX = width() / 2;
        painter.setPen(linePen);
        painter.drawLine(centerX, 0, centerX, height());
        return;
    }

    // Horizontal mode
    int centerY = height() / 2;

    if(d->text.isEmpty())
    {
        // Simple horizontal line
        painter.setPen(linePen);
        painter.drawLine(0, centerY, width(), centerY);
        return;
    }

    // Draw text with gap in line
    QFont textFont = this->font();
    textFont.setPixelSize(12);
    painter.setFont(textFont);
    QFontMetrics fm(textFont);
    int textWidth = fm.horizontalAdvance(d->text);
    int textHeight = fm.height();
    int padding = 8;

    int textX = 0;
    if(d->contentPosition == Left)
    {
        textX = 20;
    }
    else if(d->contentPosition == Center)
    {
        textX = (width() - textWidth) / 2;
    }
    else
    {
        // Right
        textX = width() - 20 - textWidth;
    }

    int gapLeft = textX - padding;
    int gapRight = textX + textWidth + padding;

    // Draw left line segment
    painter.setPen(linePen);
    if(gapLeft > 0)
    {
        painter.drawLine(0, centerY, gapLeft, centerY);
    }

    // Draw right line segment
    if(gapRight < width())
    {
        painter.drawLine(gapRight, centerY, width(), centerY);
    }

    // Draw text
    QColor textColor = QtThemeColor(d->themeMode, BasicDetailsText);
    painter.setPen(textColor);
    QRectF textRect(textX, (height() - textHeight) / 2, textWidth, textHeight);
    painter.drawText(textRect, Qt::AlignCenter, d->text);
}

QSize QtDivider::sizeHint() const
{
    Q_D(const QtDivider);
    if(d->orientation == Qt::Horizontal)
    {
        return QSize(200, 20);
    }
    return QSize(20, 200);
}

Qt::Orientation QtDivider::orientation() const
{
    Q_D(const QtDivider);
    return d->orientation;
}

void QtDivider::setOrientation(Qt::Orientation value)
{
    Q_D(QtDivider);
    if(d->orientation != value)
    {
        d->orientation = value;
        Q_EMIT orientationChanged();
    }
}

const QString& QtDivider::text() const
{
    Q_D(const QtDivider);
    return d->text;
}

void QtDivider::setText(const QString& value)
{
    Q_D(QtDivider);
    if(d->text != value)
    {
        d->text = value;
        Q_EMIT textChanged();
    }
}

int QtDivider::contentPosition() const
{
    Q_D(const QtDivider);
    return d->contentPosition;
}

void QtDivider::setContentPosition(int value)
{
    Q_D(QtDivider);
    if(d->contentPosition != value)
    {
        d->contentPosition = value;
        Q_EMIT contentPositionChanged();
    }
}