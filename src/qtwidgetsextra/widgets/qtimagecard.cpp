#include "qtimagecard.h"
#include "qttheme.h"
#include <qpainter.h>
#include <qpainterpath.h>

class QtImageCardPrivate
{
    Q_DECLARE_PUBLIC(QtImageCard)
public:
    QImage cardImage;
    int borderRadius;
    bool isPreserveAspectCrop;
    QtImageCard* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtImageCardPrivate(QtImageCard* q);
    ~QtImageCardPrivate();
};

QtImageCardPrivate::QtImageCardPrivate(QtImageCard* q)
    : q_ptr(q)
{
}
QtImageCardPrivate::~QtImageCardPrivate()
{
}

QtImageCard::QtImageCard(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtImageCardPrivate(this))
{
    Q_D(QtImageCard);
    d->borderRadius = 6;
    d->isPreserveAspectCrop = true;
    setMinimumSize(350, 260);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtImageCard::~QtImageCard()
{
}

QImage QtImageCard::cardImage() const
{
    Q_D(const QtImageCard);
    return d->cardImage;
}
void QtImageCard::setCardImage(const QImage& image)
{
    Q_D(QtImageCard);
    d->cardImage = image;
}

int QtImageCard::borderRadius() const
{
    Q_D(const QtImageCard);
    return d->borderRadius;
}
void QtImageCard::setBorderRadius(int radius)
{
    Q_D(QtImageCard);
    d->borderRadius = radius;
}

bool QtImageCard::isPreserveAspectCrop() const
{
    Q_D(const QtImageCard);
    return d->isPreserveAspectCrop;
}
void QtImageCard::setIsPreserveAspectCrop(bool isPreserve)
{
    Q_D(QtImageCard);
    d->isPreserveAspectCrop = isPreserve;
}

void QtImageCard::paintEvent(QPaintEvent* event)
{
    Q_D(QtImageCard);
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), d->borderRadius, d->borderRadius);
    painter.setClipPath(path);
    if(d->isPreserveAspectCrop)
    {
        qreal windowAspectRatio = (qreal) rect().width() / rect().height();
        qreal pixAspectRatio = (qreal) d->cardImage.width() / d->cardImage.height();
        int targetPixWidth, targetPixHeight;
        if(windowAspectRatio < pixAspectRatio)
        {
            targetPixWidth = qRound(d->cardImage.width() * windowAspectRatio / pixAspectRatio);
            targetPixHeight = d->cardImage.height();
        }
        else
        {
            targetPixWidth = d->cardImage.width();
            targetPixHeight = qRound(d->cardImage.height() * pixAspectRatio / windowAspectRatio);
        }
        painter.drawImage(rect(), d->cardImage,
            QRect((d->cardImage.width() - targetPixWidth) / 2,
                (d->cardImage.height() - targetPixHeight) / 2, targetPixWidth, targetPixHeight));
    }
    else
    {
        painter.drawImage(rect(), d->cardImage);
    }
    painter.restore();
}
