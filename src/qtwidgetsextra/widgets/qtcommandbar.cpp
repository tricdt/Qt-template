#include "qtcommandbar.h"
#include "qttheme.h"
#include "widgets/qtmenu.h"
#include <qevent.h>

class QtCommandBarPrivate
{
    Q_DECLARE_PUBLIC(QtCommandBar)
public:
    int buttonSize;
    QtCommandBar* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QList<QtCommandBar::CommandItem> items;
    int hoveredIndex{ -1 };
    int pressedIndex{ -1 };
    int visibleCount{ 0 };
    QtMenu* overflowMenu{ nullptr };
    QtCommandBarPrivate(QtCommandBar* q);
    ~QtCommandBarPrivate();
    QList<QRect> visibleItemRects() const;
    int itemWidth(const QtCommandBar::CommandItem& item) const;
};

QtCommandBarPrivate::QtCommandBarPrivate(QtCommandBar* q)
    : q_ptr(q)
{
}
QtCommandBarPrivate::~QtCommandBarPrivate()
{
}

int QtCommandBarPrivate::itemWidth(const QtCommandBar::CommandItem& item) const
{
    if(item.isSeparator)
    {
        return 8;
    }
    if(item.icon != QtIconType::None && item.text.isEmpty())
    {
        return buttonSize;
    }
    int w = 12; // padding
    if(item.icon != QtIconType::None)
    {
        w += 20; // icon space
    }
    QFont textFont = q_ptr->font();
    textFont.setPixelSize(12);
    QFontMetrics fm(textFont);
    w += fm.horizontalAdvance(item.text);
    if(w < buttonSize)
    {
        w = buttonSize;
    }
    return w;
}

QList<QRect> QtCommandBarPrivate::visibleItemRects() const
{
    QList<QRect> rects;
    int availableWidth = q_ptr->width();
    int overflowWidth = buttonSize; // width for the "..." button
    int yOffset = (q_ptr->height() - buttonSize) / 2;
    int x = 0;

    // First pass: check if all items fit
    int totalWidth = 0;
    for(const auto& item : items)
    {
        totalWidth += itemWidth(item);
    }

    bool needsOverflow = (totalWidth > availableWidth);
    int maxWidth = needsOverflow ? (availableWidth - overflowWidth) : availableWidth;

    int visibleCount = 0;
    for(int i = 0; i < items.count(); ++i)
    {
        int itemWidth = this->itemWidth(items[i]);
        if(x + itemWidth > maxWidth)
        {
            break;
        }
        int itemHeight = items[i].isSeparator ? q_ptr->height() - 16 : buttonSize;
        int itemY = items[i].isSeparator ? 8 : yOffset;
        rects.append(QRect(x, itemY, itemWidth, itemHeight));
        x += itemWidth;
        visibleCount++;
    }

    const_cast<QtCommandBarPrivate*>(this)->visibleCount = visibleCount;

    if(needsOverflow)
    {
        rects.append(QRect(x, yOffset, overflowWidth, buttonSize));
    }

    return rects;
}

QtCommandBar::QtCommandBar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtCommandBarPrivate(this))
{
    Q_D(QtCommandBar);
    d->q_ptr = this;
    d->buttonSize = 36;
    setFixedHeight(40);
    setObjectName("QtCommandBar");
    setMouseTracking(true);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtCommandBar::~QtCommandBar()
{
}

void QtCommandBar::addItem(const CommandItem& item)
{
    Q_D(QtCommandBar);
    d->items.append(item);
    update();
}

void QtCommandBar::addSeparator()
{
    CommandItem sep;
    sep.isSeparator = true;
    Q_D(QtCommandBar);
    d->items.append(sep);
    update();
}

void QtCommandBar::clearItems()
{
    Q_D(QtCommandBar);
    d->items.clear();
    d->hoveredIndex = -1;
    d->pressedIndex = -1;
    update();
}

void QtCommandBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtCommandBar);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QList<QRect> rects = d->visibleItemRects();
    bool hasOverflow = (d->visibleCount < d->items.count());
    int totalVisible = rects.count();
    if(hasOverflow)
    {
        totalVisible -= 1; // last rect is the overflow button
    }

    for(int i = 0; i < totalVisible; ++i)
    {
        const QtCommandBar::CommandItem& item = d->items[i];
        QRect itemRect = rects[i];

        if(item.isSeparator)
        {
            painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), 1));
            int x = itemRect.center().x();
            int y1 = itemRect.top() + 8;
            int y2 = itemRect.bottom() - 8;
            painter.drawLine(x, y1, x, y2);
        }
        else
        {
            // Hover / Press background
            if(d->pressedIndex == i)
            {
                painter.setPen(Qt::NoPen);
                painter.setBrush(QtThemeColor(d->themeMode, BasicPress));
                painter.drawRoundedRect(itemRect, 4, 4);
            }
            else if(d->hoveredIndex == i)
            {
                painter.setPen(Qt::NoPen);
                painter.setBrush(QtThemeColor(d->themeMode, BasicHover));
                painter.drawRoundedRect(itemRect, 4, 4);
            }

            int xOffset = itemRect.x();

            // Draw icon
            if(item.icon != QtIconType::None)
            {
                QFont iconFont = QFont("ElaAwesome");
                iconFont.setPixelSize(16);
                painter.setFont(iconFont);
                painter.setPen(QtThemeColor(d->themeMode, BasicText));
                if(item.text.isEmpty())
                {
                    painter.drawText(itemRect, Qt::AlignCenter, QChar((unsigned short) item.icon));
                }
                else
                {
                    QRect iconRect(xOffset + 6, itemRect.y(), 20, itemRect.height());
                    painter.drawText(iconRect, Qt::AlignCenter, QChar((unsigned short) item.icon));
                    xOffset += 26;
                }
            }

            // Draw text
            if(!item.text.isEmpty())
            {
                QFont textFont = this->font();
                textFont.setPixelSize(12);
                painter.setFont(textFont);
                painter.setPen(QtThemeColor(d->themeMode, BasicText));
                int textX = (item.icon != QtIconType::None) ? xOffset : itemRect.x() + 6;
                QRect textRect(
                    textX, itemRect.y(), itemRect.right() - textX - 6, itemRect.height());
                painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, item.text);
            }
        }
    }

    // Draw overflow button
    if(hasOverflow && !rects.isEmpty())
    {
        QRect overflowRect = rects.last();
        int overflowIndex = -2; // special index for overflow
        if(d->pressedIndex == overflowIndex)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QtThemeColor(d->themeMode, BasicPress));
            painter.drawRoundedRect(overflowRect, 4, 4);
        }
        else if(d->hoveredIndex == overflowIndex)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QtThemeColor(d->themeMode, BasicHover));
            painter.drawRoundedRect(overflowRect, 4, 4);
        }
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(16);
        painter.setFont(iconFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        painter.drawText(
            overflowRect, Qt::AlignCenter, QChar((unsigned short) QtIconType::Ellipsis));
    }

    painter.restore();
}

void QtCommandBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtCommandBar);
    QList<QRect> rects = d->visibleItemRects();
    bool hasOverflow = (d->visibleCount < d->items.count());
    int totalVisible = rects.count();

    int oldHovered = d->hoveredIndex;
    d->hoveredIndex = -1;

    if(hasOverflow && !rects.isEmpty())
    {
        QRect overflowRect = rects.last();
        if(overflowRect.contains(event->pos()))
        {
            d->hoveredIndex = -2;
        }
        totalVisible -= 1;
    }

    if(d->hoveredIndex == -1)
    {
        for(int i = 0; i < totalVisible; ++i)
        {
            if(!d->items[i].isSeparator && rects[i].contains(event->pos()))
            {
                d->hoveredIndex = i;
                break;
            }
        }
    }

    if(oldHovered != d->hoveredIndex)
    {
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void QtCommandBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtCommandBar);
    if(event->button() == Qt::LeftButton)
    {
        d->pressedIndex = d->hoveredIndex;
        update();
    }
    QWidget::mousePressEvent(event);
}

void QtCommandBar::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtCommandBar);
    if(event->button() == Qt::LeftButton)
    {
        if(d->pressedIndex != -1 && d->pressedIndex == d->hoveredIndex)
        {
            if(d->pressedIndex == -2)
            {
                // Overflow menu
                if(!d->overflowMenu)
                {
                    d->overflowMenu = new QtMenu(this);
                }
                d->overflowMenu->clear();
                for(int i = d->visibleCount; i < d->items.count(); ++i)
                {
                    const CommandItem& item = d->items[i];
                    if(item.isSeparator)
                    {
                        d->overflowMenu->addSeparator();
                    }
                    else
                    {
                        QAction* action = nullptr;
                        if(item.icon != QtIconType::None)
                        {
                            action = d->overflowMenu->addQtIconAction(item.icon, item.text);
                        }
                        else
                        {
                            action = d->overflowMenu->addAction(item.text);
                        }
                        int index = i;
                        connect(action, &QAction::triggered, this,
                            [=]() { Q_EMIT itemClicked(index); });
                    }
                }
                QList<QRect> rects = d->visibleItemRects();
                if(!rects.isEmpty())
                {
                    QRect overflowRect = rects.last();
                    d->overflowMenu->popup(mapToGlobal(overflowRect.bottomLeft()));
                }
            }
            else
            {
                Q_EMIT itemClicked(d->pressedIndex);
            }
        }
        d->pressedIndex = -1;
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

void QtCommandBar::leaveEvent(QEvent* event)
{
    Q_D(QtCommandBar);
    d->hoveredIndex = -1;
    update();
    QWidget::leaveEvent(event);
}

QSize QtCommandBar::sizeHint() const
{
    Q_D(const QtCommandBar);
    int totalWidth = 0;
    for(const auto& item : d->items)
    {
        totalWidth += d->itemWidth(item);
    }
    return QSize(qMax(totalWidth, 50), 40);
}

int QtCommandBar::buttonSize() const
{
    Q_D(const QtCommandBar);
    return d->buttonSize;
}

void QtCommandBar::setButtonSize(int value)
{
    Q_D(QtCommandBar);
    if(d->buttonSize != value)
    {
        d->buttonSize = value;
        Q_EMIT buttonSizeChanged();
    }
}