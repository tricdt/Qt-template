#include "qttag.h"
#include "qttheme.h"
#include <qevent.h>
#include <qpainterpath.h>
class QtTagPrivate
{
    Q_DECLARE_PUBLIC(QtTag)
public:
    int borderRadius;
    QString tagText;
    bool isClosable;
    bool isCheckable;
    bool isChecked;
    QtTag* q_ptr;
    QtTag::TagColor tagColor{ QtTag::Default };
    QtThemeType::ThemeMode themeMode;
    int closeButtonWidth{ 16 };
    int checkIconWidth{ 16 };
    int padding{ 8 };
    QtTagPrivate(QtTag* q);
    ~QtTagPrivate();
    QColor backgroundColor() const;
    QColor foregroundColor() const;
};

QtTagPrivate::QtTagPrivate(QtTag* q)
    : q_ptr(q)
{
}
QtTagPrivate::~QtTagPrivate()
{
}

QColor QtTagPrivate::backgroundColor() const
{
    Q_Q(const QtTag);
    switch(tagColor)
    {
        case QtTag::Default:
            return QtThemeColor(themeMode, BasicBase);
        case QtTag::Primary: {
            QColor c = QtThemeColor(themeMode, PrimaryNormal);
            c.setAlpha(30);
            return c;
        }
        case QtTag::Success: {
            QColor c(0x0F, 0x7B, 0x0F);
            c.setAlpha(30);
            return c;
        }
        case QtTag::Warning: {
            QColor c(0xF7, 0x93, 0x0E);
            c.setAlpha(30);
            return c;
        }
        case QtTag::Danger: {
            QColor c = QtThemeColor(themeMode, StatusDanger);
            c.setAlpha(30);
            return c;
        }
    }
    return QtThemeColor(themeMode, BasicBase);
}
QColor QtTagPrivate::foregroundColor() const
{
    Q_Q(const QtTag);
    switch(tagColor)
    {
        case QtTag::Default:
            return QtThemeColor(themeMode, BasicText);
        case QtTag::Primary:
            return QtThemeColor(themeMode, PrimaryNormal);
        case QtTag::Success:
            return QColor(0x0F, 0x7B, 0x0F);
        case QtTag::Warning:
            return QColor(0xF7, 0x93, 0x0E);
        case QtTag::Danger:
            return QtThemeColor(themeMode, StatusDanger);
    }
    return QtThemeColor(themeMode, BasicText);
}

QtTag::QtTag(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtTagPrivate(this))
{
    Q_D(QtTag);
    d->borderRadius = 4;
    d->tagText = "";
    d->isClosable = false;
    d->isCheckable = false;
    d->isChecked = false;
    setObjectName("QtTag");
    setFixedHeight(28);
    setMouseTracking(true);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    connect(this, &QtTag::tagTextChanged, this, [this]() {
        updateGeometry();
        adjustSize();
        update();
    });
    connect(this, &QtTag::isClosableChanged, this, [this]() {
        updateGeometry();
        adjustSize();
        update();
    });
    connect(this, &QtTag::isCheckableChanged, this, [this]() {
        updateGeometry();
        adjustSize();
        update();
    });
    connect(this, &QtTag::isCheckedChanged, this, [this]() {
        updateGeometry();
        adjustSize();
        update();
    });
}

QtTag::~QtTag()
{
}

void QtTag::mousePressEvent(QMouseEvent *event)
{
	Q_D(QtTag);
	if (event->button() == Qt::LeftButton)
	{
		if (d->isClosable)
		{
			// Check if click is in the close button area (right side)
			int closeAreaX = width() - d->closeButtonWidth - d->padding / 2;
			if (event->pos().x() >= closeAreaX)
			{
				Q_EMIT closed();
				return;
			}
		}
		if (d->isCheckable)
		{
			setIsChecked(!d->isChecked);
			Q_EMIT checkedChanged(d->isChecked);
		}
		Q_EMIT clicked();
	}
	QWidget::mousePressEvent(event);
}

void QtTag::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(QtTag);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QColor bgColor = d->backgroundColor();
	QColor fgColor = d->foregroundColor();

	// When checkable and checked, override to primary color
	if (d->isCheckable && d->isChecked)
	{
		QColor primaryColor = QtThemeColor(d->themeMode, PrimaryNormal);
		bgColor = primaryColor;
		bgColor.setAlpha(30);
		fgColor = primaryColor;
	}

	// Draw background rounded rectangle
	painter.setPen(Qt::NoPen);
	painter.setBrush(bgColor);
	QPainterPath path;
	path.addRoundedRect(QRectF(0, 0, width(), height()), d->borderRadius, d->borderRadius);
	painter.drawPath(path);

	// Calculate text draw area
	int xOffset = d->padding;

	// Draw check mark if checkable and checked
	if (d->isCheckable && d->isChecked)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(12);
		painter.setFont(iconFont);
		painter.setPen(fgColor);
		QRectF checkRect(xOffset, 0, d->checkIconWidth, height());
		painter.drawText(checkRect, Qt::AlignCenter, QChar((unsigned short) 0xea6c));
		xOffset += d->checkIconWidth;
	}

	// Draw text
	QFont textFont = this->font();
	textFont.setPixelSize(13);
	painter.setFont(textFont);
	painter.setPen(fgColor);
	int textWidth = width() - xOffset - d->padding;
	if (d->isClosable)
	{
		textWidth -= d->closeButtonWidth;
	}
	QRectF textRect(xOffset, 0, textWidth, height());
	painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, d->tagText);

	// Draw close button if closable
	if (d->isClosable)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(10);
		painter.setFont(iconFont);
		painter.setPen(fgColor);
		int closeX = width() - d->closeButtonWidth - d->padding / 2;
		QRectF closeRect(closeX, 0, d->closeButtonWidth, height());
		painter.drawText(closeRect, Qt::AlignCenter, QChar((unsigned short) 0xf4ce));
	}
}

QSize QtTag::sizeHint() const
{
	Q_D(const QtTag);
	QFont textFont = this->font();
	textFont.setPixelSize(13);
	QFontMetrics fm(textFont);
	int textWidth = fm.horizontalAdvance(d->tagText);
	int w = textWidth + d->padding * 2;
	if (d->isClosable)
	{
		w += d->closeButtonWidth;
	}
	if (d->isCheckable && d->isChecked)
	{
		w += d->checkIconWidth;
	}
	return QSize(qMax(w, 32), 28);
}

void QtTag::setTagColor(TagColor color)
{
    Q_D(QtTag);
    	d->tagColor = color;
	update();
}
QtTag::TagColor QtTag::tagColor() const
{
    Q_D(const QtTag);
    return d->tagColor;
}

int QtTag::borderRadius() const
{
    Q_D(const QtTag);
    return d->borderRadius;
}
void QtTag::setBorderRadius(int value)
{
    Q_D(QtTag);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}
const QString& QtTag::tagText() const
{
    Q_D(const QtTag);
    return d->tagText;
}
void QtTag::setTagText(const QString& value)
{
    Q_D(QtTag);
    d->tagText = value;
    Q_EMIT tagTextChanged();
}
bool QtTag::isClosable() const
{
    Q_D(const QtTag);
    return d->isClosable;
}
void QtTag::setIsClosable(bool value)
{
    Q_D(QtTag);
    d->isClosable = value;
    Q_EMIT isClosableChanged();
}
bool QtTag::isCheckable() const
{
    Q_D(const QtTag);
    return d->isCheckable;
}
void QtTag::setIsCheckable(bool value)
{
    Q_D(QtTag);
    d->isCheckable = value;
    Q_EMIT isCheckableChanged();
}
bool QtTag::isChecked() const
{
    Q_D(const QtTag);
    return d->isChecked;
}
void QtTag::setIsChecked(bool value)
{
    Q_D(QtTag);
    d->isChecked = value;
    Q_EMIT isCheckedChanged();
}

// int
// QString
// bool
// bool
// bool

// borderRadius
// tagText
// isClosable
// isCheckable
// isChecked