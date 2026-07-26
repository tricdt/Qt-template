#include "qtinteractivecard.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qpainterpath.h>

class QtInteractiveCardPrivate
{
    Q_DECLARE_PUBLIC(QtInteractiveCard)
public:
    int borderRadius;
    QString title;
    QString subTitle;
    int titlePixelSize;
    int subTitlePixelSize;
    int titleSpacing;
    QPixmap cardPixmap;
    QSize cardPixmapSize;
    int cardPixmapBorderRadius;
    QtCardPixType::PixMode cardPixMode;
    QtInteractiveCard* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtInteractiveCardPrivate(QtInteractiveCard* q);
    ~QtInteractiveCardPrivate();
};

QtInteractiveCardPrivate::QtInteractiveCardPrivate(QtInteractiveCard* q)
    : q_ptr(q)
{
}
QtInteractiveCardPrivate::~QtInteractiveCardPrivate()
{
}

QtInteractiveCard::QtInteractiveCard(QWidget* parent)
    : QPushButton{ parent }
    , d_ptr(new QtInteractiveCardPrivate(this))
{
    Q_D(QtInteractiveCard);
    d->borderRadius = 6;
    d->titlePixelSize = 15;
    d->subTitlePixelSize = 12;
    setMinimumSize(270, 80);
    d->cardPixmapSize = QSize(64, 64);
    d->titleSpacing = 2;
    d->cardPixmapBorderRadius = 6;
    d->cardPixMode = QtCardPixType::PixMode::Ellipse;
    d->themeMode = qtTheme->getThemeMode();
    setMouseTracking(true);
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtInteractiveCard::~QtInteractiveCard()
{
}

void QtInteractiveCard::setCardPixmapSize(int width, int height)
{
    Q_D(QtInteractiveCard);
    d->cardPixmapSize = QSize(width, height);
    Q_EMIT cardPixmapSizeChanged();
}

int QtInteractiveCard::borderRadius() const
{
    Q_D(const QtInteractiveCard);
    return d->borderRadius;
}
void QtInteractiveCard::setBorderRadius(int value)
{
    Q_D(QtInteractiveCard);
    d->borderRadius = value;
}

QString QtInteractiveCard::title() const
{
    Q_D(const QtInteractiveCard);
    return d->title;
}
void QtInteractiveCard::setTitle(const QString& value)
{
    Q_D(QtInteractiveCard);
    d->title = value;
}

QString QtInteractiveCard::subTitle() const
{
    Q_D(const QtInteractiveCard);
    return d->subTitle;
}
void QtInteractiveCard::setSubTitle(const QString& value)
{
    Q_D(QtInteractiveCard);
    d->subTitle = value;
}

int QtInteractiveCard::titlePixelSize() const
{
    Q_D(const QtInteractiveCard);
    return d->titlePixelSize;
}
void QtInteractiveCard::setTitlePixelSize(int value)
{
    Q_D(QtInteractiveCard);
    d->titlePixelSize = value;
}

int QtInteractiveCard::subTitlePixelSize() const
{
    Q_D(const QtInteractiveCard);
    return d->subTitlePixelSize;
}
void QtInteractiveCard::setSubTitlePixelSize(int value)
{
    Q_D(QtInteractiveCard);
    d->subTitlePixelSize = value;
}

int QtInteractiveCard::titleSpacing() const
{
    Q_D(const QtInteractiveCard);
    return d->titleSpacing;
}
void QtInteractiveCard::setTitleSpacing(int value)
{
    Q_D(QtInteractiveCard);
    d->titleSpacing = value;
}

QPixmap QtInteractiveCard::cardPixmap() const
{
    Q_D(const QtInteractiveCard);
    return d->cardPixmap;
}
void QtInteractiveCard::setCardPixmap(const QPixmap& value)
{
    Q_D(QtInteractiveCard);
    d->cardPixmap = value;
}

QSize QtInteractiveCard::cardPixmapSize() const
{
    Q_D(const QtInteractiveCard);
    return d->cardPixmapSize;
}
void QtInteractiveCard::setCardPixmapSize(const QSize& value)
{
    Q_D(QtInteractiveCard);
    d->cardPixmapSize = value;
}

int QtInteractiveCard::cardPixmapBorderRadius() const
{
    Q_D(const QtInteractiveCard);
    return d->cardPixmapBorderRadius;
}
void QtInteractiveCard::setCardPixmapBorderRadius(int value)
{
    Q_D(QtInteractiveCard);
    d->cardPixmapBorderRadius = value;
}

QtCardPixType::PixMode QtInteractiveCard::cardPixMode() const
{
    Q_D(const QtInteractiveCard);
    return d->cardPixMode;
}
void QtInteractiveCard::setCardPixMode(QtCardPixType::PixMode value)
{
    Q_D(QtInteractiveCard);
    d->cardPixMode = value;
}

void QtInteractiveCard::paintEvent(QPaintEvent* event)
{
    Q_D(QtInteractiveCard);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(underMouse() ? QtThemeColor(d->themeMode, BasicHoverAlpha) : Qt::transparent);
    painter.drawRoundedRect(rect(), d->borderRadius, d->borderRadius);
    if (!d->cardPixmap.isNull())
    {
        painter.save();
        QPainterPath path;
        if (d->cardPixMode == QtCardPixType::PixMode::Ellipse)
        {
            path.addEllipse(QPointF(d->cardPixmapSize.width() / 2 + 10, height() / 2), d->cardPixmapSize.width() / 2, d->cardPixmapSize.height() / 2);
            painter.setClipPath(path);
            painter.drawPixmap(QRect(10, (height() - d->cardPixmapSize.height()) / 2, d->cardPixmapSize.width(), d->cardPixmapSize.height()), d->cardPixmap); // rect为绘制区域，image为要绘制的图片
        }
        else if (d->cardPixMode == QtCardPixType::PixMode::Default)
        {
            painter.drawPixmap(10, (height() - d->cardPixmapSize.height()) / 2, d->cardPixmapSize.width(), d->cardPixmapSize.height(), d->cardPixmap);
        }
        else if (d->cardPixMode == QtCardPixType::PixMode::RoundedRect)
        {
            path.addRoundedRect(QRectF(10, (height() - d->cardPixmapSize.height()) / 2, d->cardPixmapSize.width(), d->cardPixmapSize.height()), d->cardPixmapBorderRadius, d->cardPixmapBorderRadius);
            painter.setClipPath(path);
            painter.drawPixmap(10, (height() - d->cardPixmapSize.height()) / 2, d->cardPixmapSize.width(), d->cardPixmapSize.height(), d->cardPixmap);
        }
        painter.restore();
    }
    painter.setPen(QtThemeColor(d->themeMode, BasicText));
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(d->titlePixelSize);
    painter.setFont(font);
    int textStartX = d->cardPixmapSize.width() + 26;
    int textWidth = width() - textStartX;
    painter.drawText(QRect(textStartX, rect().y(), textWidth, height() / 2 - d->titleSpacing), Qt::TextWordWrap | Qt::AlignBottom | Qt::AlignLeft, d->title);
    font.setWeight(QFont::Normal);
    font.setPixelSize(d->subTitlePixelSize);
    painter.setFont(font);
    painter.drawText(QRect(textStartX, height() / 2 + d->titleSpacing, textWidth, height() / 2 - d->titleSpacing), Qt::TextWordWrap | Qt::AlignTop | Qt::AlignLeft, d->subTitle);
    painter.restore();
}
