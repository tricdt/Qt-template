#include "qtpagination.h"
#include "qttheme.h"
#include "widgets/qtlineedit.h"
#include <qevent.h>
#include <qpainterpath.h>
#include <qvalidator.h>
class QtPaginationPrivate
{
    Q_DECLARE_PUBLIC(QtPagination)
public:
    int currentPage;
    int totalPages;
    int buttonSize;
    int pagerCount;
    bool jumperVisible;
    QtPagination* q_ptr;
    QtThemeType::ThemeMode themeMode;
    int hoverIndex{ -1 };
    QtLineEdit* jumperEdit{ nullptr };
    QtPaginationPrivate(QtPagination* q);
    ~QtPaginationPrivate();
    QList<QPair<QRect, int>> buttonRects() const;
    QList<int> visiblePages() const;
    void updateJumperPosition();
};

QtPaginationPrivate::QtPaginationPrivate(QtPagination* q)
    : q_ptr(q)
{
}
QtPaginationPrivate::~QtPaginationPrivate()
{
}

void QtPaginationPrivate::updateJumperPosition()
{
    Q_Q(QtPagination);
    if(!jumperEdit)
    {
        return;
    }
    jumperEdit->setVisible(jumperVisible);
    if(!jumperVisible)
    {
        return;
    }
    QList<QPair<QRect, int>> buttonRects = this->buttonRects();
    int jumperX = 0;
    if(!buttonRects.isEmpty())
    {
        QRect lastRect = buttonRects.last().first;
        jumperX = lastRect.x() + lastRect.width() + 12;
    }
    int editWidth = 55;
    int editHeight = buttonSize;
    int y = (q->height() - editHeight) / 2;
    jumperEdit->setGeometry(jumperX, y, editWidth, editHeight);
}

QList<int> QtPaginationPrivate::visiblePages() const
{
    QList<int> pages;
    int total = totalPages;
    int current = currentPage;
    int pagerCount = this->pagerCount;

    if(total <= 0)
    {
        return pages;
    }

    if(total <= pagerCount)
    {
        for(int i = 1; i <= total; i++)
        {
            pages.append(i);
        }
        return pages;
    }

    int halfPager = (pagerCount - 2) / 2;
    bool showLeftEllipsis = current > halfPager + 2;
    bool showRightEllipsis = current < total - halfPager - 1;

    if(!showLeftEllipsis && showRightEllipsis)
    {
        for(int i = 1; i <= pagerCount - 2; i++)
        {
            pages.append(i);
        }
        pages.append(-1);
        pages.append(total);
    }
    else if(showLeftEllipsis && !showRightEllipsis)
    {
        pages.append(1);
        pages.append(-3);
        for(int i = total - (pagerCount - 3); i <= total; i++)
        {
            pages.append(i);
        }
    }
    else
    {
        pages.append(1);
        pages.append(-3);
        int sideCount = (pagerCount - 4) / 2;
        for(int i = current - sideCount; i <= current + sideCount; i++)
        {
            pages.append(i);
        }
        pages.append(-1);
        pages.append(total);
    }

    return pages;
}

QList<QPair<QRect, int>> QtPaginationPrivate::buttonRects() const
{
    QList<QPair<QRect, int>> rects;
    int size = buttonSize;
    int spacing = 4;
    int x = 0;
    int y = 4;

    rects.append(qMakePair(QRect(x, y, size, size), 0));
    x += size + spacing;

    QList<int> pages = visiblePages();
    for(int page : pages)
    {
        if(page == -1)
        {
            rects.append(qMakePair(QRect(x, y, size, size), -1));
        }
        else if(page == -3)
        {
            rects.append(qMakePair(QRect(x, y, size, size), -3));
        }
        else
        {
            rects.append(qMakePair(QRect(x, y, size, size), page));
        }
        x += size + spacing;
    }

    rects.append(qMakePair(QRect(x, y, size, size), -2));

    return rects;
}

QtPagination::QtPagination(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtPaginationPrivate(this))
{
    Q_D(QtPagination);
    d->currentPage = 1;
    d->totalPages = 1;
    d->buttonSize = 28;
    d->pagerCount = 11;
    d->jumperVisible = false;
    setObjectName("QtPagination");
    setMouseTracking(true);
    setFixedHeight(d->buttonSize + 8);
    d->themeMode = qtTheme->getThemeMode();

    d->jumperEdit = new QtLineEdit(this);
    d->jumperEdit->setFixedHeight(d->buttonSize);
    d->jumperEdit->setPlaceholderText("页码");
    d->jumperEdit->setAlignment(Qt::AlignCenter);
    d->jumperEdit->setStyleSheet(
        "#QtLineEdit{background-color:transparent;padding-left:0px;padding-right:0px;}");
    d->jumperEdit->setValidator(new QIntValidator(1, 9999, d->jumperEdit));
    d->jumperEdit->setVisible(false);
    connect(d->jumperEdit, &QtLineEdit::returnPressed, this, [=]() {
        QString text = d->jumperEdit->text().trimmed();
        bool ok = false;
        int page = text.toInt(&ok);
        if(ok && page >= 1 && page <= d->totalPages && page != d->currentPage)
        {
            d->currentPage = page;
            Q_EMIT currentPageChanged();
            Q_EMIT currentPageChanged(page);
            update();
        }
        d->jumperEdit->clear();
    });

    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtPagination::~QtPagination()
{
}

void QtPagination::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    Q_D(QtPagination);
    d->updateJumperPosition();
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QList<QPair<QRect, int>> buttonRects = d->buttonRects();
    QFont textFont = this->font();
    textFont.setPixelSize(14);

    QFont iconFont("ElaAwesome");
    iconFont.setPixelSize(16);

    for(int i = 0; i < buttonRects.size(); i++)
    {
        QRect rect = buttonRects[i].first;
        int pageValue = buttonRects[i].second;

        bool isHovered = (i == d->hoverIndex);
        bool isCurrentPage = (pageValue > 0 && pageValue == d->currentPage);
        bool isDisabled = false;

        if(pageValue == 0 && d->currentPage <= 1)
        {
            isDisabled = true;
        }
        if(pageValue == -2 && d->currentPage >= d->totalPages)
        {
            isDisabled = true;
        }

        painter.setPen(Qt::NoPen);
        if(isCurrentPage)
        {
            painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
        }
        else if(isDisabled)
        {
            painter.setBrush(QtThemeColor(d->themeMode, BasicDisable));
        }
        else if(isHovered)
        {
            painter.setBrush(QtThemeColor(d->themeMode, BasicHover));
        }
        else
        {
            painter.setBrush(QtThemeColor(d->themeMode, BasicBase));
        }

        QPainterPath path;
        path.addRoundedRect(rect, 6, 6);
        painter.drawPath(path);

        if(isCurrentPage)
        {
            painter.setPen(Qt::white);
        }
        else if(isDisabled)
        {
            painter.setPen(QtThemeColor(d->themeMode, BasicTextDisable));
        }
        else
        {
            painter.setPen(QtThemeColor(d->themeMode, BasicText));
        }

        if(pageValue == 0)
        {
            painter.setFont(iconFont);
            painter.drawText(rect, Qt::AlignCenter, QChar((unsigned short) 0xea84));
        }
        else if(pageValue == -2)
        {
            painter.setFont(iconFont);
            painter.drawText(rect, Qt::AlignCenter, QChar((unsigned short) 0xea85));
        }
        else if(pageValue == -1 || pageValue == -3)
        {
            painter.setFont(iconFont);
            painter.drawText(rect, Qt::AlignCenter, QChar((unsigned short) 0xec4d));
        }
        else
        {
            painter.setFont(textFont);
            painter.drawText(rect, Qt::AlignCenter, QString::number(pageValue));
        }
    }
}

void QtPagination::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtPagination);
    QList<QPair<QRect, int>> buttonRects = d->buttonRects();
    int oldHover = d->hoverIndex;
    d->hoverIndex = -1;
    for(int i = 0; i < buttonRects.size(); i++)
    {
        if(buttonRects[i].first.contains(event->pos()))
        {
            d->hoverIndex = i;
            break;
        }
    }
    if(oldHover != d->hoverIndex)
    {
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void QtPagination::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtPagination);
    if(event->button() != Qt::LeftButton)
    {
        QWidget::mousePressEvent(event);
        return;
    }

    QList<QPair<QRect, int>> buttonRects = d->buttonRects();
    for(int i = 0; i < buttonRects.size(); i++)
    {
        if(buttonRects[i].first.contains(event->pos()))
        {
            int pageValue = buttonRects[i].second;
            int newPage = d->currentPage;

            if(pageValue == 0)
            {
                if(d->currentPage > 1)
                {
                    newPage = d->currentPage - 1;
                }
            }
            else if(pageValue == -2)
            {
                if(d->currentPage < d->totalPages)
                {
                    newPage = d->currentPage + 1;
                }
            }
            else if(pageValue == -1)
            {
                newPage = qMin(d->currentPage + d->pagerCount - 2, d->totalPages);
            }
            else if(pageValue == -3)
            {
                newPage = qMax(d->currentPage - (d->pagerCount - 2), 1);
            }
            else if(pageValue > 0)
            {
                newPage = pageValue;
            }

            if(newPage != d->currentPage)
            {
                d->currentPage = newPage;
                Q_EMIT currentPageChanged();
                Q_EMIT currentPageChanged(newPage);
                update();
            }
            break;
        }
    }
    QWidget::mousePressEvent(event);
}

QSize QtPagination::sizeHint() const
{
    Q_D(const QtPagination);
    QList<QPair<QRect, int>> buttonRects = d->buttonRects();
    int totalWidth = 0;
    if(!buttonRects.isEmpty())
    {
        QRect lastRect = buttonRects.last().first;
        totalWidth = lastRect.x() + lastRect.width();
    }
    if(d->jumperVisible)
    {
        totalWidth += 12 + 55;
    }
    return QSize(totalWidth, d->buttonSize + 8);
}

int QtPagination::currentPage() const
{
    Q_D(const QtPagination);
    return d->currentPage;
}

void QtPagination::setCurrentPage(int value)
{
    Q_D(QtPagination);
    if(d->currentPage != value)
    {
        d->currentPage = value;
        Q_EMIT currentPageChanged(value);
    }
}

int QtPagination::totalPages() const
{
    Q_D(const QtPagination);
    return d->totalPages;
}

void QtPagination::setTotalPages(int value)
{
    Q_D(QtPagination);
    if(d->totalPages != value)
    {
        d->totalPages = value;
        Q_EMIT totalPagesChanged();
    }
}

int QtPagination::buttonSize() const
{
    Q_D(const QtPagination);
    return d->buttonSize;
}

void QtPagination::setButtonSize(int value)
{
    Q_D(QtPagination);
    if(d->buttonSize != value)
    {
        d->buttonSize = value;
        Q_EMIT buttonSizeChanged();
    }
}

int QtPagination::pagerCount() const
{
    Q_D(const QtPagination);
    return d->pagerCount;
}

void QtPagination::setPagerCount(int value)
{
    Q_D(QtPagination);
    if(d->pagerCount != value)
    {
        d->pagerCount = value;
        Q_EMIT pagerCountChanged();
    }
}

bool QtPagination::jumperVisible() const
{
    Q_D(const QtPagination);
    return d->jumperVisible;
}

void QtPagination::setJumperVisible(bool value)
{
    Q_D(QtPagination);
    if(d->jumperVisible != value)
    {
        d->jumperVisible = value;
        Q_EMIT jumperVisibleChanged();
    }
}