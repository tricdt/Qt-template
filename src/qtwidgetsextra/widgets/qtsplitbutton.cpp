#include "qtsplitbutton.h"
#include "qttheme.h"
#include <qevent.h>
#include <qpainterpath.h>
class QtSplitButtonPrivate
{
    Q_DECLARE_PUBLIC(QtSplitButton)
public:
    int borderRadius;
    QString text;
    QtIconType::IconName qtIcon;
    QtSplitButton* q_ptr;
    static const int dropdownWidth = 30;
    QtMenu* menu{ nullptr };
    bool isLeftHovered{ false };
    bool isRightHovered{ false };
    bool isLeftPressed{ false };
    bool isRightPressed{ false };
    QtThemeType::ThemeMode themeMode;
    QtSplitButtonPrivate(QtSplitButton* q);
    ~QtSplitButtonPrivate();
};

QtSplitButtonPrivate::QtSplitButtonPrivate(QtSplitButton* q)
    : q_ptr(q)
{
}
QtSplitButtonPrivate::~QtSplitButtonPrivate()
{
}

QtSplitButton::QtSplitButton(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSplitButtonPrivate(this))
{
    Q_D(QtSplitButton);
    d->borderRadius = 3;
    d->text = QString();
    d->qtIcon = QtIconType::None;
    d->themeMode = qtTheme->getThemeMode();
    setMouseTracking(true);
    setFixedHeight(35);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("QtSplitButton");
    setStyleSheet("#QtSplitButton{background-color:transparent;}");
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtSplitButton::~QtSplitButton()
{
}

void QtSplitButton::setMenu(QtMenu* menu)
{
    Q_D(QtSplitButton);
    d->menu = menu;
}
QtMenu* QtSplitButton::menu() const
{
    Q_D(const QtSplitButton);
    return d->menu;
}

void QtSplitButton::mousePressEvent(QMouseEvent *event)
{
	Q_D(QtSplitButton);
	if (event->button() == Qt::LeftButton)
	{
		int splitX = width() - d->dropdownWidth;
		if (event->pos().x() < splitX)
		{
			d->isLeftPressed = true;
		}
		else
		{
			d->isRightPressed = true;
		}
		update();
	}
	QWidget::mousePressEvent(event);
}

void QtSplitButton::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(QtSplitButton);
	if (event->button() == Qt::LeftButton)
	{
		int splitX = width() - d->dropdownWidth;
		if (d->isLeftPressed && event->pos().x() < splitX)
		{
			Q_EMIT clicked();
		}
		else if (d->isRightPressed && event->pos().x() >= splitX)
		{
			if (d->menu)
			{
				QPoint pos = mapToGlobal(QPoint(0, height()));
				d->menu->popup(pos);
			}
		}
		d->isLeftPressed = false;
		d->isRightPressed = false;
		update();
	}
	QWidget::mouseReleaseEvent(event);
}

void QtSplitButton::mouseMoveEvent(QMouseEvent *event)
{
	Q_D(QtSplitButton);
	int splitX = width() - d->dropdownWidth;
	bool leftHovered = event->pos().x() < splitX;
	bool rightHovered = !leftHovered;
	if (d->isLeftHovered != leftHovered || d->isRightHovered != rightHovered)
	{
		d->isLeftHovered = leftHovered;
		d->isRightHovered = rightHovered;
		update();
	}
	QWidget::mouseMoveEvent(event);
}

void QtSplitButton::leaveEvent(QEvent *event)
{
	Q_D(QtSplitButton);
	d->isLeftHovered = false;
	d->isRightHovered = false;
	d->isLeftPressed = false;
	d->isRightPressed = false;
	update();
	QWidget::leaveEvent(event);
}

void QtSplitButton::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	Q_D(QtSplitButton);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.save();

	int borderRadius = d->borderRadius;
	int splitX = width() - d->dropdownWidth;
	QRect leftRect(0, 0, splitX, height());
	QRect rightRect(splitX, 0, d->dropdownWidth, height());
	QRect fullRect(0, 0, width(), height());

	QColor baseColor = QtThemeColor(d->themeMode, BasicBase);
	QColor hoverColor = QtThemeColor(d->themeMode, BasicHover);
	QColor pressColor = QtThemeColor(d->themeMode, BasicPress);
	QColor textColor = QtThemeColor(d->themeMode, BasicText);
	QColor borderColor = QtThemeColor(d->themeMode, BasicBorder);

	// Determine left and right fill colors
	QColor leftColor = baseColor;
	if (d->isLeftPressed)
	{
		leftColor = pressColor;
	}
	else if (d->isLeftHovered)
	{
		leftColor = hoverColor;
	}

	QColor rightColor = baseColor;
	if (d->isRightPressed)
	{
		rightColor = pressColor;
	}
	else if (d->isRightHovered)
	{
		rightColor = hoverColor;
	}

	// Draw full rounded rect background with base color
	QPainterPath fullPath;
	fullPath.addRoundedRect(fullRect, borderRadius, borderRadius);
	painter.setClipPath(fullPath);

	// Draw left area
	painter.setPen(Qt::NoPen);
	painter.setBrush(leftColor);
	painter.drawRect(leftRect);

	// Draw right area
	painter.setBrush(rightColor);
	painter.drawRect(rightRect);

	// Reset clip and draw border
	painter.setClipPath(fullPath);
	if (d->themeMode == QtThemeType::Light)
	{
		painter.setPen(borderColor);
	}
	else
	{
		painter.setPen(Qt::NoPen);
	}
	painter.setBrush(Qt::NoBrush);
	painter.drawRoundedRect(fullRect, borderRadius, borderRadius);

	// Draw vertical separator line
	painter.setPen(QPen(borderColor, 1));
	painter.drawLine(splitX, 4, splitX, height() - 4);

	// Draw left area content: icon + text
	painter.setPen(textColor);
	if (d->qtIcon != QtIconType::None)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(18);
		QFontMetrics iconFm(iconFont);
		int iconWidth = iconFm.horizontalAdvance(QChar((unsigned short) d->qtIcon));

		QFont textFont = this->font();
		QFontMetrics textFm(textFont);
		int textWidth = d->text.isEmpty() ? 0 : textFm.horizontalAdvance(d->text);

		int spacing = d->text.isEmpty() ? 0 : 8;
		int totalWidth = iconWidth + spacing + textWidth;
		int startX = leftRect.x() + (leftRect.width() - totalWidth) / 2;

		painter.setFont(iconFont);
		painter.drawText(QRect(startX, leftRect.y(), iconWidth, leftRect.height()), Qt::AlignCenter, QChar((unsigned short) d->qtIcon));

		if (!d->text.isEmpty())
		{
			painter.setFont(textFont);
			painter.drawText(QRect(startX + iconWidth + spacing, leftRect.y(), textWidth, leftRect.height()), Qt::AlignVCenter | Qt::AlignLeft, d->text);
		}
	}
	else
	{
		painter.setFont(this->font());
		painter.drawText(leftRect, Qt::AlignCenter, d->text);
	}

	// Draw right area: AngleDown icon
	QFont arrowFont = QFont("ElaAwesome");
	arrowFont.setPixelSize(14);
	painter.setFont(arrowFont);
	painter.setPen(textColor);
	painter.drawText(rightRect, Qt::AlignCenter, QChar((unsigned short) QtIconType::AngleDown));

	painter.restore();
}

QSize QtSplitButton::sizeHint() const
{
	Q_D(const QtSplitButton);
	QFontMetrics fm(font());
	int textWidth = d->text.isEmpty() ? 0 : fm.horizontalAdvance(d->text);
	int iconWidth = (d->qtIcon != QtIconType::None) ? 24 : 0;
	int spacing = (!d->text.isEmpty() && d->qtIcon != QtIconType::None) ? 8 : 0;
	int leftWidth = iconWidth + spacing + textWidth + 20;
	return QSize(leftWidth + d->dropdownWidth, 35);
}


int QtSplitButton::borderRadius() const
{
    Q_D(const QtSplitButton);
    return d->borderRadius;
}
void QtSplitButton::setBorderRadius(int value)
{
    Q_D(QtSplitButton);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}
QString QtSplitButton::text() const
{
    Q_D(const QtSplitButton);
    return d->text;
}
void QtSplitButton::setText(QString value)
{
    Q_D(QtSplitButton);
    d->text = value;
    Q_EMIT textChanged();
}
QtIconType::IconName QtSplitButton::qtIcon() const
{
    Q_D(const QtSplitButton);
    return d->qtIcon;
}
void QtSplitButton::setQtIcon(QtIconType::IconName value)
{
    Q_D(QtSplitButton);
    d->qtIcon = value;
    Q_EMIT qtIconChanged();
}
