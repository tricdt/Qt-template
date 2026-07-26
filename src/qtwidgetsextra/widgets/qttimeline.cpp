#include "qttimeline.h"
#include "qttheme.h"

class QtTimelinePrivate
{
    Q_DECLARE_PUBLIC(QtTimeline)
public:
    QtTimeline* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QList<QtTimeline::TimelineItem> items;
    QtTimelinePrivate(QtTimeline* q);
    ~QtTimelinePrivate();
};

QtTimelinePrivate::QtTimelinePrivate(QtTimeline* q)
    : q_ptr(q)
{
}
QtTimelinePrivate::~QtTimelinePrivate()
{
}

QtTimeline::QtTimeline(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtTimelinePrivate(this))
{
    Q_D(QtTimeline);
    d->q_ptr = this;
    setObjectName("QtTimeline");
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtTimeline::~QtTimeline()
{
}

void QtTimeline::addItem(const TimelineItem& item)
{
    Q_D(QtTimeline);
    d->items.append(item);
    updateGeometry();
    update();
}

void QtTimeline::clearItems()
{
    Q_D(QtTimeline);
    d->items.clear();
    updateGeometry();
    update();
}

int QtTimeline::getItemCount() const
{
    return d_ptr->items.count();
}

void QtTimeline::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtTimeline);
    if(d->items.isEmpty())
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    const int leftMargin = 80;
    const int circleDiameter = 12;
    const int circleRadius = circleDiameter / 2;
    const int iconCircleDiameter = 24;
    const int iconCircleRadius = iconCircleDiameter / 2;
    const int lineX = leftMargin + circleRadius; // x=86
    const int contentX = 100;
    const int contentWidth = width() - 120;
    const int itemGap = 8;

    QColor primaryColor = QtThemeColor(d->themeMode, PrimaryNormal);
    QColor borderColor = QtThemeColor(d->themeMode, BasicBorder);
    QColor textColor = QtThemeColor(d->themeMode, BasicText);
    QColor detailsTextColor = QtThemeColor(d->themeMode, BasicDetailsText);

    // Prepare fonts
    QFont timestampFont = font();
    timestampFont.setPixelSize(12);

    QFont titleFont = font();
    titleFont.setPixelSize(14);
    titleFont.setBold(true);

    QFont contentFont = font();
    contentFont.setPixelSize(13);

    QFont iconFont("ElaAwesome");

    int currentY = 0;

    for(int i = 0; i < d->items.count(); ++i)
    {
        const TimelineItem& item = d->items[i];
        bool hasIcon = (item.icon != QtIconType::None);
        int currentCircleDiameter = hasIcon ? iconCircleDiameter : circleDiameter;
        int currentCircleRadius = currentCircleDiameter / 2;

        // Calculate content height
        painter.setFont(titleFont);
        QFontMetrics titleFm(titleFont);
        int titleHeight = titleFm.height();

        painter.setFont(contentFont);
        QFontMetrics contentFm(contentFont);
        int contentTextHeight = 0;
        if(!item.content.isEmpty())
        {
            QRect contentBound = contentFm.boundingRect(
                QRect(0, 0, contentWidth, 10000), Qt::TextWordWrap, item.content);
            contentTextHeight = contentBound.height();
        }

        int itemContentHeight = titleHeight + (contentTextHeight > 0 ? 4 + contentTextHeight : 0);
        int itemHeight = qMax(60, itemContentHeight + 10);

        // Circle center Y
        int circleCenterY = currentY + currentCircleRadius;

        // Draw timestamp (left side, right-aligned)
        painter.setFont(timestampFont);
        painter.setPen(detailsTextColor);
        QFontMetrics timestampFm(timestampFont);
        int timestampHeight = timestampFm.height();
        QRect timestampRect(
            0, circleCenterY - timestampHeight / 2, leftMargin - 10, timestampHeight);
        painter.drawText(timestampRect, Qt::AlignRight | Qt::AlignVCenter, item.timestamp);

        // Draw circle
        painter.setPen(Qt::NoPen);
        if(i == 0)
        {
            painter.setBrush(primaryColor);
        }
        else
        {
            painter.setBrush(borderColor);
        }

        if(hasIcon)
        {
            // Draw larger circle with icon
            painter.drawEllipse(lineX - iconCircleRadius, circleCenterY - iconCircleRadius,
                iconCircleDiameter, iconCircleDiameter);

            // Draw icon inside circle
            iconFont.setPixelSize(12);
            painter.setFont(iconFont);
            painter.setPen(Qt::white);
            QRect iconRect(lineX - iconCircleRadius, circleCenterY - iconCircleRadius,
                iconCircleDiameter, iconCircleDiameter);
            painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) item.icon));
        }
        else
        {
            painter.drawEllipse(
                leftMargin, circleCenterY - circleRadius, circleDiameter, circleDiameter);
        }

        // Draw vertical line to next item
        if(i < d->items.count() - 1)
        {
            QPen linePen(borderColor, 1);
            painter.setPen(linePen);
            int lineStartY = circleCenterY + currentCircleRadius;
            int lineEndY = currentY + itemHeight + itemGap;
            painter.drawLine(lineX, lineStartY, lineX, lineEndY);
        }

        // Draw title
        painter.setFont(titleFont);
        painter.setPen(textColor);
        QRect titleRect(contentX, currentY, contentWidth, titleHeight);
        painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, item.title);

        // Draw content
        if(!item.content.isEmpty())
        {
            painter.setFont(contentFont);
            painter.setPen(detailsTextColor);
            QRect contentRect(
                contentX, currentY + titleHeight + 4, contentWidth, contentTextHeight);
            painter.drawText(
                contentRect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop, item.content);
        }

        currentY += itemHeight + itemGap;
    }
}

QSize QtTimeline::sizeHint() const
{
    Q_D(const QtTimeline);
    if(d->items.isEmpty())
    {
        return QSize(400, 0);
    }

    QFont titleFont = font();
    titleFont.setPixelSize(14);
    titleFont.setBold(true);

    QFont contentFont = font();
    contentFont.setPixelSize(13);

    int contentWidth = 400 - 120;
    int totalHeight = 0;
    const int itemGap = 8;

    for(int i = 0; i < d->items.count(); ++i)
    {
        QFontMetrics titleFm(titleFont);
        int titleHeight = titleFm.height();

        int contentTextHeight = 0;
        if(!d->items[i].content.isEmpty())
        {
            QFontMetrics contentFm(contentFont);
            QRect contentBound = contentFm.boundingRect(
                QRect(0, 0, contentWidth, 10000), Qt::TextWordWrap, d->items[i].content);
            contentTextHeight = contentBound.height();
        }

        int itemContentHeight = titleHeight + (contentTextHeight > 0 ? 4 + contentTextHeight : 0);
        int itemHeight = qMax(60, itemContentHeight + 10);
        totalHeight += itemHeight;
        if(i < d->items.count() - 1)
        {
            totalHeight += itemGap;
        }
    }

    return QSize(400, totalHeight);
}