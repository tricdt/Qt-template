#include "qtnumberbox.h"
#include "qttheme.h"
#include <qevent.h>
#include <qlineedit.h>
#include <qpainterpath.h>

class QtNumberBoxPrivate
{
    Q_DECLARE_PUBLIC(QtNumberBox)
public:
    int borderRadius;
    double value;
    double minimum;
    double maximum;
    double step;
    int decimals;
    bool isWrapping;
    QtNumberBox* q_ptr;
    static const int buttonWidth = 32;
    bool isMinusHovered{ false };
    bool isPlusHovered{ false };
    bool isMinusPressed{ false };
    bool isPlusPressed{ false };
    bool isEditing{ false };
    QLineEdit* lineEdit{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QtNumberBoxPrivate(QtNumberBox* q);
    ~QtNumberBoxPrivate();
    QRect minusRect() const;
    QRect plusRect() const;
    QRect valueRect() const;
    QString formatValue() const;
    void clampValue();
    void startEditing();
    void finishEditing();
};

QtNumberBoxPrivate::QtNumberBoxPrivate(QtNumberBox* q)
    : q_ptr(q)
{
}
QtNumberBoxPrivate::~QtNumberBoxPrivate()
{
}

QRect QtNumberBoxPrivate::minusRect() const
{
    Q_Q(const QtNumberBox);
    return QRect(0, 0, buttonWidth, q->height());
}

QRect QtNumberBoxPrivate::plusRect() const
{
    Q_Q(const QtNumberBox);
    return QRect(q->width() - buttonWidth, 0, buttonWidth, q->height());
}

QRect QtNumberBoxPrivate::valueRect() const
{
    Q_Q(const QtNumberBox);
    return QRect(buttonWidth, 0, q->width() - buttonWidth * 2, q->height());
}

QString QtNumberBoxPrivate::formatValue() const
{
    return QString::number(value, 'f', decimals);
}

void QtNumberBoxPrivate::clampValue()
{
    if(value < minimum)
    {
        value = isWrapping ? maximum : minimum;
    }
    if(value > maximum)
    {
        value = isWrapping ? minimum : maximum;
    }
}

void QtNumberBoxPrivate::startEditing()
{
    Q_Q(QtNumberBox);
    isEditing = true;
    if(!lineEdit)
    {
        lineEdit = new QLineEdit(q);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setFrame(false);
        QFont f = q->font();
        f.setPixelSize(14);
        lineEdit->setFont(f);
        lineEdit->setStyleSheet("QLineEdit { background: transparent; border: none; }");
        lineEdit->installEventFilter(q);
    }
    lineEdit->setGeometry(valueRect());
    lineEdit->setText(formatValue());
    lineEdit->selectAll();
    lineEdit->show();
    lineEdit->setFocus();
}

void QtNumberBoxPrivate::finishEditing()
{
    Q_Q(QtNumberBox);
    if(!isEditing)
    {
        return;
    }
    isEditing = false;
    if(lineEdit)
    {
        bool ok;
        double newVal = lineEdit->text().toDouble(&ok);
        if(ok)
        {
            value = newVal;
            clampValue();
            Q_EMIT q->valueChanged();
            Q_EMIT q->valueChanged(value);
        }
        lineEdit->hide();
    }
    q->update();
}

QtNumberBox::QtNumberBox(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtNumberBoxPrivate(this))
{
    Q_D(QtNumberBox);
    d->borderRadius = 6;
    d->value = 0.0;
    d->minimum = 0.0;
    d->maximum = 100.0;
    d->step = 1.0;
    d->decimals = 0;
    d->isWrapping = false;
    setObjectName("QtNumberBox");
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(160, 36);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtNumberBox::~QtNumberBox()
{
}

void QtNumberBox::stepUp()
{
    Q_D(QtNumberBox);
    setValue(d->value + d->step);
}

void QtNumberBox::stepDown()
{
    Q_D(QtNumberBox);
    setValue(d->value - d->step);
}

void QtNumberBox::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtNumberBox);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QRect fullRect(0, 0, width(), height());
    QColor borderColor = QtThemeColor(d->themeMode, BasicBorder);
    QColor baseColor = QtThemeColor(d->themeMode, BasicBase);
    QColor hoverColor = QtThemeColor(d->themeMode, BasicHover);
    QColor pressColor = QtThemeColor(d->themeMode, BasicPress);
    QColor textColor = QtThemeColor(d->themeMode, BasicText);

    // Background
    QPainterPath clipPath;
    clipPath.addRoundedRect(fullRect, d->borderRadius, d->borderRadius);
    painter.setClipPath(clipPath);

    // Minus button
    QRect minusR = d->minusRect();
    QColor minusBg = baseColor;
    if(d->isMinusPressed)
        minusBg = pressColor;
    else if(d->isMinusHovered)
        minusBg = hoverColor;
    painter.setPen(Qt::NoPen);
    painter.setBrush(minusBg);
    painter.drawRect(minusR);

    // Plus button
    QRect plusR = d->plusRect();
    QColor plusBg = baseColor;
    if(d->isPlusPressed)
        plusBg = pressColor;
    else if(d->isPlusHovered)
        plusBg = hoverColor;
    painter.setBrush(plusBg);
    painter.drawRect(plusR);

    // Value area
    QRect valR = d->valueRect();
    painter.setBrush(QtThemeColor(d->themeMode, BasicBase));
    painter.drawRect(valR);

    // Reset clip, draw border
    painter.setClipPath(clipPath);
    painter.setPen(QPen(borderColor, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(
        QRectF(0.5, 0.5, width() - 1, height() - 1), d->borderRadius, d->borderRadius);

    // Separator lines
    painter.setPen(QPen(borderColor, 1));
    painter.drawLine(d->buttonWidth, 4, d->buttonWidth, height() - 4);
    painter.drawLine(width() - d->buttonWidth, 4, width() - d->buttonWidth, height() - 4);

    // Minus icon
    QFont btnFont = font();
    btnFont.setPixelSize(18);
    btnFont.setBold(true);
    painter.setFont(btnFont);
    bool atMin = d->value <= d->minimum && !d->isWrapping;
    painter.setPen(atMin ? QtThemeColor(d->themeMode, BasicTextDisable) : textColor);
    painter.drawText(minusR, Qt::AlignCenter, "-");

    // Plus icon
    bool atMax = d->value >= d->maximum && !d->isWrapping;
    painter.setPen(atMax ? QtThemeColor(d->themeMode, BasicTextDisable) : textColor);
    painter.drawText(plusR, Qt::AlignCenter, "+");

    // Value text (only when not editing)
    if(!d->isEditing)
    {
        QFont valFont = font();
        valFont.setPixelSize(14);
        painter.setFont(valFont);
        painter.setPen(textColor);
        painter.drawText(valR, Qt::AlignCenter, d->formatValue());
    }
}

void QtNumberBox::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtNumberBox);
    if(event->button() == Qt::LeftButton)
    {
        if(d->minusRect().contains(event->pos()))
        {
            d->isMinusPressed = true;
            update();
        }
        else if(d->plusRect().contains(event->pos()))
        {
            d->isPlusPressed = true;
            update();
        }
    }
    QWidget::mousePressEvent(event);
}

void QtNumberBox::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtNumberBox);
    if(event->button() == Qt::LeftButton)
    {
        if(d->isMinusPressed && d->minusRect().contains(event->pos()))
        {
            stepDown();
        }
        if(d->isPlusPressed && d->plusRect().contains(event->pos()))
        {
            stepUp();
        }
        d->isMinusPressed = false;
        d->isPlusPressed = false;
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void QtNumberBox::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtNumberBox);
    bool minusHover = d->minusRect().contains(event->pos());
    bool plusHover = d->plusRect().contains(event->pos());
    if(d->isMinusHovered != minusHover || d->isPlusHovered != plusHover)
    {
        d->isMinusHovered = minusHover;
        d->isPlusHovered = plusHover;
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void QtNumberBox::leaveEvent(QEvent* event)
{
    Q_D(QtNumberBox);
    d->isMinusHovered = false;
    d->isPlusHovered = false;
    d->isMinusPressed = false;
    d->isPlusPressed = false;
    update();
    QWidget::leaveEvent(event);
}

void QtNumberBox::wheelEvent(QWheelEvent* event)
{
    if(event->angleDelta().y() > 0)
    {
        stepUp();
    }
    else if(event->angleDelta().y() < 0)
    {
        stepDown();
    }
    event->accept();
}

void QtNumberBox::keyPressEvent(QKeyEvent* event)
{
    Q_D(QtNumberBox);
    switch(event->key())
    {
        case Qt::Key_Up:
            stepUp();
            return;
        case Qt::Key_Down:
            stepDown();
            return;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if(d->isEditing)
            {
                d->finishEditing();
            }
            else
            {
                d->startEditing();
            }
            return;
        case Qt::Key_Escape:
            if(d->isEditing)
            {
                d->isEditing = false;
                if(d->lineEdit)
                {
                    d->lineEdit->hide();
                }
                setFocus();
                update();
            }
            return;
        default:
            break;
    }
    QWidget::keyPressEvent(event);
}

void QtNumberBox::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_D(QtNumberBox);
    if(d->valueRect().contains(event->pos()))
    {
        d->startEditing();
    }
    QWidget::mouseDoubleClickEvent(event);
}

bool QtNumberBox::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtNumberBox);
    if(watched == d->lineEdit)
    {
        if(event->type() == QEvent::FocusOut)
        {
            d->finishEditing();
        }
        else if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if(ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter)
            {
                d->finishEditing();
                setFocus();
                return true;
            }
            if(ke->key() == Qt::Key_Escape)
            {
                d->isEditing = false;
                d->lineEdit->hide();
                setFocus();
                update();
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

int QtNumberBox::borderRadius() const
{
    Q_D(const QtNumberBox);
    return d->borderRadius;
}

void QtNumberBox::setBorderRadius(int value)
{
    Q_D(QtNumberBox);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

double QtNumberBox::value() const
{
    Q_D(const QtNumberBox);
    return d->value;
}

void QtNumberBox::setValue(double value)
{
    Q_D(QtNumberBox);
    d->value = value;
    d->clampValue();
    Q_EMIT valueChanged();
    Q_EMIT valueChanged(d->value);
    update();
}

double QtNumberBox::minimum() const
{
    Q_D(const QtNumberBox);
    return d->minimum;
}

void QtNumberBox::setMinimum(double value)
{
    Q_D(QtNumberBox);
    if(d->minimum != value)
    {
        d->minimum = value;
        Q_EMIT minimumChanged();
    }
}

double QtNumberBox::maximum() const
{
    Q_D(const QtNumberBox);
    return d->maximum;
}

void QtNumberBox::setMaximum(double value)
{
    Q_D(QtNumberBox);
    if(d->maximum != value)
    {
        d->maximum = value;
        Q_EMIT maximumChanged();
    }
}

double QtNumberBox::step() const
{
    Q_D(const QtNumberBox);
    return d->step;
}

void QtNumberBox::setStep(double value)
{
    Q_D(QtNumberBox);
    if(d->step != value)
    {
        d->step = value;
        Q_EMIT stepChanged();
    }
}

int QtNumberBox::decimals() const
{
    Q_D(const QtNumberBox);
    return d->decimals;
}

void QtNumberBox::setDecimals(int value)
{
    Q_D(QtNumberBox);
    if(d->decimals != value)
    {
        d->decimals = value;
        Q_EMIT decimalsChanged();
    }
}

bool QtNumberBox::isWrapping() const
{
    Q_D(const QtNumberBox);
    return d->isWrapping;
}

void QtNumberBox::setIsWrapping(bool value)
{
    Q_D(QtNumberBox);
    if(d->isWrapping != value)
    {
        d->isWrapping = value;
        Q_EMIT isWrappingChanged();
    }
}