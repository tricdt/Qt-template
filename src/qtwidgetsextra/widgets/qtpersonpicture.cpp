#include "qtpersonpicture.h"
#include "qttheme.h"
#include <qpainterpath.h>

class QtPersonPicturePrivate
{
    Q_DECLARE_PUBLIC(QtPersonPicture)
public:
    int pictureSize;
    QString displayName;
    QPixmap picture;
    QtPersonPicture* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtPersonPicturePrivate(QtPersonPicture* q);
    ~QtPersonPicturePrivate();
};

QtPersonPicturePrivate::QtPersonPicturePrivate(QtPersonPicture* q)
    : q_ptr(q)
{
}
QtPersonPicturePrivate::~QtPersonPicturePrivate()
{
}

QtPersonPicture::QtPersonPicture(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtPersonPicturePrivate(this))
{
    Q_D(QtPersonPicture);
    d->pictureSize = 40;
    setFixedSize(d->pictureSize, d->pictureSize);
    setObjectName("QtPersonPicture");
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtPersonPicture::~QtPersonPicture()
{
}

QSize QtPersonPicture::sizeHint() const
{
	int size = d_ptr->pictureSize;
	return QSize(size, size);
}

void QtPersonPicture::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(QtPersonPicture);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	int size = d->pictureSize;
	qreal borderWidth = 2.0;
	QRectF circleRect(borderWidth / 2, borderWidth / 2, size - borderWidth, size - borderWidth);
	QPointF center = circleRect.center();
	qreal radius = circleRect.width() / 2;

	if (!d->picture.isNull())
	{
		// Draw circular-clipped pixmap
		painter.save();
		QPainterPath clipPath;
		clipPath.addEllipse(center, radius, radius);
		painter.setClipPath(clipPath);
		painter.drawPixmap(circleRect.toRect(), d->picture.scaled(circleRect.size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
		painter.restore();
	}
	else if (!d->displayName.isEmpty())
	{
		// Draw circle with PrimaryNormal background and white initials
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
		painter.drawEllipse(center, radius, radius);
		painter.restore();

		painter.save();
		QString initials = d->displayName.left(2);
		QFont font = this->font();
		font.setPixelSize(size / 3);
		font.setWeight(QFont::Bold);
		painter.setFont(font);
		painter.setPen(Qt::white);
		painter.drawText(circleRect.toRect(), Qt::AlignCenter, initials);
		painter.restore();
	}
	else
	{
		// Draw circle with BasicBase background and user icon
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(QtThemeColor(d->themeMode, BasicBase));
		painter.drawEllipse(center, radius, radius);
		painter.restore();

		painter.save();
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(size / 3);
		painter.setFont(iconFont);
		painter.setPen(QtThemeColor(d->themeMode, BasicText));
		painter.drawText(circleRect.toRect(), Qt::AlignCenter, QChar((unsigned short) QtIconType::User));
		painter.restore();
	}

	// Draw 2px circular border
	painter.save();
	painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), borderWidth));
	painter.setBrush(Qt::NoBrush);
	painter.drawEllipse(center, radius, radius);
	painter.restore();
}

int QtPersonPicture::pictureSize() const
{
    Q_D(const QtPersonPicture);
    return d->pictureSize;
}
void QtPersonPicture::setPictureSize(int value)
{
    Q_D(QtPersonPicture);
    d->pictureSize = value;
    setFixedSize(value, value);
    Q_EMIT pictureSizeChanged();
}
QString QtPersonPicture::displayName() const
{
    Q_D(const QtPersonPicture);
    return d->displayName;
}
void QtPersonPicture::setDisplayName(QString value)
{
    Q_D(QtPersonPicture);
    d->displayName = value;
    Q_EMIT displayNameChanged();
}
const QPixmap& QtPersonPicture::picture() const
{
    Q_D(const QtPersonPicture);
    return d->picture;
}
void QtPersonPicture::setPicture(const QPixmap& value)
{
    Q_D(QtPersonPicture);
    d->picture = value;
    Q_EMIT pictureChanged();
}