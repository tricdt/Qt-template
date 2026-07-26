#include "qtsteps.h"
#include "qttheme.h"
class QtStepsPrivate
{
    Q_DECLARE_PUBLIC(QtSteps)
public:
    int currentStep;
    int stepCount;
    QtSteps* q_ptr;
    QStringList stepTitles;
    QtThemeType::ThemeMode themeMode;
    Qt::Orientation stepOrientation = Qt::Horizontal;
    QtStepsPrivate(QtSteps* q);
    ~QtStepsPrivate();
    void updateSize();
};

QtStepsPrivate::QtStepsPrivate(QtSteps* q)
    : q_ptr(q)
{
}
QtStepsPrivate::~QtStepsPrivate()
{
}

void QtStepsPrivate::updateSize()
{
	Q_Q(QtSteps);
	if (stepOrientation == Qt::Vertical)
		q->setFixedHeight(60 + qMax(0, stepCount - 1) * 52);
	else
		q->setFixedHeight(70);
}

QtSteps::QtSteps(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtStepsPrivate(this))
{
    Q_D(QtSteps);
    d->q_ptr = this;
    setObjectName("QtSteps");
    d->updateSize();

    d->currentStep = 0;
    d->stepCount = 3;

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtSteps::~QtSteps()
{
}

void QtSteps::setStepTitles(const QStringList &titles)
{
	Q_D(QtSteps);
	d->stepTitles = titles;
	update();
}

QStringList QtSteps::stepTitles() const
{
	Q_D(const QtSteps);
	return d->stepTitles;
}

void QtSteps::setStepOrientation(Qt::Orientation orientation)
{
	Q_D(QtSteps);
	if (d->stepOrientation == orientation)
		return;
	d->stepOrientation = orientation;
	d->updateSize();
	update();
}

Qt::Orientation QtSteps::stepOrientation() const
{
	Q_D(const QtSteps);
	return d->stepOrientation;
}

void QtSteps::next()
{
	Q_D(QtSteps);
	if (d->currentStep < d->stepCount - 1)
	{
		d->currentStep++;
		update();
		Q_EMIT currentStepChanged(d->currentStep);
	}
}

void QtSteps::previous()
{
	Q_D(QtSteps);
	if (d->currentStep > 0)
	{
		d->currentStep--;
		update();
		Q_EMIT currentStepChanged(d->currentStep);
	}
}

void QtSteps::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	Q_D(QtSteps);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

	int stepCount = d->stepCount;
	if (stepCount <= 0)
	{
		return;
	}

	int w = width();
	int circleDiameter = 28;
	int circleRadius = circleDiameter / 2;
	int circleY = 8;
	int titleY = 42;
	int lineCenterY = circleY + circleRadius;

	// Evenly distribute circles with margin for end titles
	int margin = 50;
	qreal usableWidth = w - margin * 2;
	qreal spacing = (stepCount > 1) ? usableWidth / (stepCount - 1) : 0;

	QColor primaryNormal = QtThemeColor(d->themeMode, PrimaryNormal);
	QColor basicBase = QtThemeColor(d->themeMode, BasicBase);
	QColor basicBorder = QtThemeColor(d->themeMode, BasicBorder);
	QColor basicDetailsText = QtThemeColor(d->themeMode, BasicDetailsText);
	QColor whiteColor = Qt::white;

	QFont iconFont("ElaAwesome");
	iconFont.setPixelSize(14);

	QFont numberFont = this->font();
	numberFont.setPixelSize(14);
	numberFont.setWeight(QFont::Bold);

	QFont titleFont = this->font();
	titleFont.setPixelSize(13);

	if (d->stepOrientation == Qt::Vertical)
	{
		const int circleDiameter = 28;
		const int circleRadius = circleDiameter / 2;  // 14
		const int leftX = 22;                          // 圆点中心 x
		const int firstCenter = 16 + circleRadius;     // 首个圆点中心 y
		const int rowSpacing = 52;                     // 行间距
		const int titleX = leftX + circleDiameter + 14;
		const int titleMaxW = qMax(40, width() - titleX - 16);

		for (int i = 0; i < stepCount; i++)
		{
			const qreal cy = firstCenter + i * rowSpacing;

			// 连接竖线
			if (i < stepCount - 1)
			{
				const qreal nextCy = firstCenter + (i + 1) * rowSpacing;
				if (i < d->currentStep)
					painter.setPen(QPen(primaryNormal, 2));
				else
					painter.setPen(QPen(basicBorder, 2));
				painter.drawLine(QPointF(leftX, cy + circleRadius), QPointF(leftX, nextCy - circleRadius));
			}

			// 圆点
			const QRectF circleRect(leftX - circleRadius, cy - circleRadius, circleDiameter, circleDiameter);
			painter.setPen(Qt::NoPen);
			if (i < d->currentStep)
			{
				painter.setBrush(primaryNormal);
				painter.drawEllipse(circleRect);
				painter.setPen(whiteColor);
				painter.setFont(iconFont);
				painter.drawText(circleRect, Qt::AlignCenter, QChar((unsigned short)0xea6c));
			}
			else if (i == d->currentStep)
			{
				painter.setBrush(primaryNormal);
				painter.drawEllipse(circleRect);
				painter.setPen(whiteColor);
				painter.setFont(numberFont);
				painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
			}
			else
			{
				painter.setBrush(basicBase);
				painter.drawEllipse(circleRect);
				painter.setPen(QPen(basicBorder, 1));
				painter.setBrush(Qt::NoBrush);
				painter.drawEllipse(circleRect);
				painter.setPen(basicDetailsText);
				painter.setFont(numberFont);
				painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
			}

			// 标题：序号圆点中心与文本垂直居中对齐，长文本自动换行
			if (i < d->stepTitles.size())
			{
				painter.setPen(QtThemeColor(d->themeMode, BasicText));
				painter.setFont(titleFont);
				const QRectF titleRect(titleX, cy - rowSpacing / 2.0, titleMaxW, rowSpacing);
				painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, d->stepTitles[i]);
			}
		}
		return;
	}

	for (int i = 0; i < stepCount; i++)
	{
		qreal cx = (stepCount > 1) ? (margin + i * spacing) : (w / 2.0);

		// Draw connecting line (before circle, so circle overlaps)
		if (i < stepCount - 1)
		{
			qreal lineStartX = cx + circleRadius;
			qreal nextCx = (margin + (i + 1) * spacing);
			qreal lineEndX = nextCx - circleRadius;

			if (i < d->currentStep)
			{
				painter.setPen(QPen(primaryNormal, 2));
			}
			else
			{
				painter.setPen(QPen(basicBorder, 2));
			}
			painter.drawLine(QPointF(lineStartX, lineCenterY), QPointF(lineEndX, lineCenterY));
		}

		// Draw circle
		QRectF circleRect(cx - circleRadius, circleY, circleDiameter, circleDiameter);
		painter.setPen(Qt::NoPen);

		if (i < d->currentStep)
		{
			// Completed
			painter.setBrush(primaryNormal);
			painter.drawEllipse(circleRect);

			// Draw check icon
			painter.setPen(whiteColor);
			painter.setFont(iconFont);
			painter.drawText(circleRect, Qt::AlignCenter, QChar((unsigned short) 0xea6c));
		}
		else if (i == d->currentStep)
		{
			// Active
			painter.setBrush(primaryNormal);
			painter.drawEllipse(circleRect);

			// Draw number
			painter.setPen(whiteColor);
			painter.setFont(numberFont);
			painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
		}
		else
		{
			// Pending
			painter.setBrush(basicBase);
			painter.drawEllipse(circleRect);
			painter.setPen(QPen(basicBorder, 1));
			painter.setBrush(Qt::NoBrush);
			painter.drawEllipse(circleRect);

			// Draw number
			painter.setPen(basicDetailsText);
			painter.setFont(numberFont);
			painter.drawText(circleRect, Qt::AlignCenter, QString::number(i + 1));
		}

		// Draw title
		if (i < d->stepTitles.size())
		{
			painter.setPen(QtThemeColor(d->themeMode, BasicText));
			painter.setFont(titleFont);
			int titleWidth = 120;
			QRectF titleRect(cx - titleWidth / 2.0, titleY, titleWidth, 20);
			painter.drawText(titleRect, Qt::AlignHCenter | Qt::AlignTop, d->stepTitles[i]);
		}
	}
}

int QtSteps::currentStep() const
{
    Q_D(const QtSteps);
    return d->currentStep;
}

void QtSteps::setCurrentStep(int value)
{
    Q_D(QtSteps);
    if(d->currentStep != value)
    {
        d->currentStep = value;
        Q_EMIT currentStepChanged();
    }
}

int QtSteps::stepCount() const
{
    Q_D(const QtSteps);
    return d->stepCount;
}

void QtSteps::setStepCount(int value)
{
    Q_D(QtSteps);
    if(d->stepCount != value)
    {
        d->stepCount = value;
        Q_EMIT stepCountChanged();
    }
}