#include "qtexpander.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qboxlayout.h>
#include <qevent.h>
#include <qpainter.h>
#include <qvariantanimation.h>
class QtExpanderPrivate
{
    Q_DECLARE_PUBLIC(QtExpander)

public:
    int borderRadius;
    QString title;
    QString subTitle;
    QtIconType::IconName headerIcon;
    int animationDuration;
    QtExpander* q_ptr;
    bool isExpanded{ false };
    bool isPressed{ false };
    QtExpander::ExpandDirection expandDirection{ QtExpander::Down };
    QWidget* contentWidget{ nullptr };
    QWidget* headerCustomWidget{ nullptr };
    QWidget* contentContainer{ nullptr };
    QVBoxLayout* contentLayout{ nullptr };
    QVBoxLayout* mainLayout{ nullptr };
    QWidget* headerWidget{ nullptr };
    QVariantAnimation* expandAnimation{ nullptr };
    int headerHeight{ 48 };
    int contentHeight{ 0 };
    QtThemeType::ThemeMode themeMode;

    QtExpanderPrivate(QtExpander* q);
    ~QtExpanderPrivate();

    void toggleExpand();
    void startExpandAnimation(bool expand);
};

QtExpanderPrivate::QtExpanderPrivate(QtExpander* q)
    : q_ptr(q)
{
}
QtExpanderPrivate::~QtExpanderPrivate()
{
}

void QtExpanderPrivate::toggleExpand()
{
    isExpanded = !isExpanded;
    startExpandAnimation(isExpanded);
    Q_Q(QtExpander);
    Q_EMIT q->expandStateChanged(isExpanded);
}

void QtExpanderPrivate::startExpandAnimation(bool expand)
{
    Q_Q(QtExpander);
    if(!contentWidget)
    {
        return;
    }
    if(!expandAnimation)
    {
        expandAnimation = new QVariantAnimation(q);
        expandAnimation->setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(expandAnimation, &QVariantAnimation::valueChanged, q,
            [this](const QVariant& value) { q_ptr->setFixedHeight(value.toInt()); });
    }
    expandAnimation->stop();
    expandAnimation->setDuration(animationDuration);

    // Ensure content widget is laid out so sizeHint is valid
    contentWidget->adjustSize();
    contentHeight = contentWidget->sizeHint().height() + contentLayout->contentsMargins().top() +
                    contentLayout->contentsMargins().bottom();
    if(contentHeight < 40)
    {
        contentHeight = 40;
    }
    int collapsedHeight = headerHeight;
    int expandedHeight = headerHeight + contentHeight;

    if(expand)
    {
        contentContainer->show();
        expandAnimation->setStartValue(collapsedHeight);
        expandAnimation->setEndValue(expandedHeight);
    }
    else
    {
        contentContainer->hide();
        expandAnimation->setStartValue(q->height());
        expandAnimation->setEndValue(collapsedHeight);
    }
    expandAnimation->start();
}

QtExpander::QtExpander(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtExpanderPrivate(this))
{
    Q_D(QtExpander);
	d->borderRadius = 6;
	d->title = "";
	d->subTitle = "";
	d->headerIcon = QtIconType::None;
	d->animationDuration = 250;
	setObjectName("QtExpander");
	setMouseTracking(true);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	d->mainLayout = new QVBoxLayout(this);
	d->mainLayout->setContentsMargins(0, 0, 0, 0);
	d->mainLayout->setSpacing(0);

	// Header area (painted via paintEvent, not a separate widget)
	d->headerWidget = new QWidget(this);
	d->headerWidget->setFixedHeight(d->headerHeight);
	d->headerWidget->setAttribute(Qt::WA_TransparentForMouseEvents);

	// Content container
	d->contentContainer = new QWidget(this);
	d->contentLayout = new QVBoxLayout(d->contentContainer);
	d->contentLayout->setContentsMargins(15, 5, 15, 5);
	d->contentContainer->hide();

	d->mainLayout->addWidget(d->headerWidget);
	d->mainLayout->addWidget(d->contentContainer);

	setFixedHeight(d->headerHeight);

	d->themeMode = qtTheme->getThemeMode();
	connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode)
	{
		d_ptr->themeMode = themeMode;
		update();
	});
}

QtExpander::QtExpander(const QString& title, QWidget* parent)
    : QtExpander(parent)
{
    Q_D(QtExpander);
    d->title = title;
}

QtExpander::~QtExpander()
{
}

void QtExpander::setExpandDirection(ExpandDirection direction)
{
	Q_D(QtExpander);
	d->expandDirection = direction;
	// Rebuild layout order
	d->mainLayout->removeWidget(d->headerWidget);
	d->mainLayout->removeWidget(d->contentContainer);
	if (direction == Up)
	{
		d->mainLayout->addWidget(d->contentContainer);
		d->mainLayout->addWidget(d->headerWidget);
	}
	else
	{
		d->mainLayout->addWidget(d->headerWidget);
		d->mainLayout->addWidget(d->contentContainer);
	}
	update();
}

QtExpander::ExpandDirection QtExpander::expandDirection() const
{
	return d_ptr->expandDirection;
}

void QtExpander::setContentWidget(QWidget *widget)
{
	Q_D(QtExpander);
	if (d->contentWidget)
	{
		d->contentLayout->removeWidget(d->contentWidget);
		d->contentWidget->deleteLater();
	}
	d->contentWidget = widget;
	if (widget)
	{
		d->contentLayout->addWidget(widget);
	}
}

QWidget *QtExpander::contentWidget() const
{
	return d_ptr->contentWidget;
}

void QtExpander::setHeaderWidget(QWidget *widget)
{
	Q_D(QtExpander);
	if (d->headerCustomWidget)
	{
		d->headerCustomWidget->deleteLater();
	}
	d->headerCustomWidget = widget;
	if (widget)
	{
		widget->setParent(d->headerWidget);
		widget->setAttribute(Qt::WA_TransparentForMouseEvents);
		widget->move(d->headerHeight, 0);
		widget->setFixedHeight(d->headerHeight);
	}
	update();
}

void QtExpander::setIsExpanded(bool expanded)
{
	Q_D(QtExpander);
	if (d->isExpanded != expanded)
	{
		d->toggleExpand();
	}
}

bool QtExpander::isExpanded() const
{
	return d_ptr->isExpanded;
}

void QtExpander::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(QtExpander);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	// Background
	painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), 1));
	painter.setBrush(QtThemeColor(d->themeMode, BasicBase));
	painter.drawRoundedRect(QRectF(0.5, 0.5, width() - 1, height() - 1), d->borderRadius, d->borderRadius);

	// Header separator line when expanded
	if (d->isExpanded || (d->expandAnimation && d->expandAnimation->state() == QVariantAnimation::Running))
	{
		int lineY = (d->expandDirection == Down) ? d->headerHeight : height() - d->headerHeight;
		painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), 1));
		painter.drawLine(d->borderRadius, lineY, width() - d->borderRadius, lineY);
	}

	// Header area
	int headerY = (d->expandDirection == Down) ? 0 : height() - d->headerHeight;

	// Header icon
	int textStartX = 15;
	if (d->headerIcon != QtIconType::None)
	{
		QFont iconFont = QFont("ElaAwesome");
		iconFont.setPixelSize(16);
		painter.setFont(iconFont);
		painter.setPen(QtThemeColor(d->themeMode, BasicText));
		QRect iconRect(15, headerY, 20, d->headerHeight);
		painter.drawText(iconRect, Qt::AlignVCenter | Qt::AlignLeft, QChar((unsigned short) d->headerIcon));
		textStartX = 42;
	}

	// Title
	if (!d->title.isEmpty())
	{
		QFont titleFont = font();
		titleFont.setPixelSize(14);
		titleFont.setBold(true);
		painter.setFont(titleFont);
		painter.setPen(QtThemeColor(d->themeMode, BasicText));
		QRect titleRect(textStartX, headerY, width() - textStartX - 40, d->headerHeight);
		painter.drawText(titleRect, Qt::AlignVCenter | Qt::AlignLeft, d->title);

		// SubTitle
		if (!d->subTitle.isEmpty())
		{
			QFontMetrics fm(titleFont);
			int titleWidth = fm.horizontalAdvance(d->title);
			QFont subFont = font();
			subFont.setPixelSize(12);
			painter.setFont(subFont);
			painter.setPen(QtThemeColor(d->themeMode, BasicDetailsText));
			QRect subRect(textStartX + titleWidth + 12, headerY, width() - textStartX - titleWidth - 52, d->headerHeight);
			painter.drawText(subRect, Qt::AlignVCenter | Qt::AlignLeft, d->subTitle);
		}
	}

	// Expand/collapse arrow
	QFont arrowFont = QFont("ElaAwesome");
	arrowFont.setPixelSize(14);
	painter.setFont(arrowFont);
	painter.setPen(QtThemeColor(d->themeMode, BasicText));
	QRect arrowRect(width() - 35, headerY, 20, d->headerHeight);
	QtIconType::IconName arrowIcon;
	if (d->expandDirection == QtExpander::Down)
	{
		arrowIcon = d->isExpanded ? QtIconType::AngleUp : QtIconType::AngleDown;
	}
	else
	{
		arrowIcon = d->isExpanded ? QtIconType::AngleDown : QtIconType::AngleUp;
	}
	painter.drawText(arrowRect, Qt::AlignVCenter | Qt::AlignCenter, QChar((unsigned short) arrowIcon));
}

void QtExpander::mousePressEvent(QMouseEvent *event)
{
	Q_D(QtExpander);
	int headerY = (d->expandDirection == Down) ? 0 : height() - d->headerHeight;
	QRect headerRect(0, headerY, width(), d->headerHeight);
	if (headerRect.contains(event->pos()) && event->button() == Qt::LeftButton)
	{
		d->isPressed = true;
	}
	QWidget::mousePressEvent(event);
}

void QtExpander::mouseReleaseEvent(QMouseEvent *event)
{
	Q_D(QtExpander);
	if (d->isPressed && event->button() == Qt::LeftButton)
	{
		int headerY = (d->expandDirection == Down) ? 0 : height() - d->headerHeight;
		QRect headerRect(0, headerY, width(), d->headerHeight);
		if (headerRect.contains(event->pos()))
		{
			d->toggleExpand();
		}
		d->isPressed = false;
	}
	QWidget::mouseReleaseEvent(event);
}

bool QtExpander::event(QEvent *event)
{
	switch (event->type())
	{
		case QEvent::Enter:
		case QEvent::Leave:
			update();
			break;
		default:
			break;
	}
	return QWidget::event(event);
}

int QtExpander::borderRadius() const
{
    Q_D(const QtExpander);
    return d->borderRadius;
}
void QtExpander::setBorderRadius(int value)
{
    Q_D(QtExpander);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}
QString QtExpander::title() const
{
    Q_D(const QtExpander);
    return d->title;
}
void QtExpander::setTitle(QString value)
{
    Q_D(QtExpander);
    d->title = value;
    Q_EMIT titleChanged();
}
QString QtExpander::subTitle() const
{
    Q_D(const QtExpander);
    return d->subTitle;
}
void QtExpander::setSubTitle(QString value)
{
    Q_D(QtExpander);
    d->subTitle = value;
    Q_EMIT subTitleChanged();
}
QtIconType::IconName QtExpander::headerIcon() const
{
    Q_D(const QtExpander);
    return d->headerIcon;
}
void QtExpander::setHeaderIcon(QtIconType::IconName value)
{
    Q_D(QtExpander);
    d->headerIcon = value;
    Q_EMIT headerIconChanged();
}
int QtExpander::animationDuration() const
{
    Q_D(const QtExpander);
    return d->animationDuration;
}
void QtExpander::setAnimationDuration(int value)
{
    Q_D(QtExpander);
    d->animationDuration = value;
    Q_EMIT animationDurationChanged();
}