#include "qttablewidget.h"
#include "styles/qttablewidgetstyle.h"
#include "widgets/qtscrollbar.h"
#include <qevent.h>
#include <qscrollbar.h>

class QtTableWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtTableWidget)
public:
    QtTableWidget* q_ptr;
    QtTableWidgetStyle* _tableWidgetStyle{ nullptr };
    QtTableWidgetPrivate(QtTableWidget* q);
    ~QtTableWidgetPrivate();
};

QtTableWidgetPrivate::QtTableWidgetPrivate(QtTableWidget* q)
    : q_ptr(q)
{
}
QtTableWidgetPrivate::~QtTableWidgetPrivate()
{
}

QtTableWidget::QtTableWidget(QWidget* parent)
    : QTableWidget{ parent }
    , d_ptr(new QtTableWidgetPrivate(this))
{
    Q_D(QtTableWidget);
    setMouseTracking(true);
    setObjectName("QtTableWidget");
    setStyleSheet("QTableWidget{background-color:transparent;}"
                  "QHeaderView{background-color:transparent;border:0px;}");
    setShowGrid(false);
    setVerticalScrollBar(new QtScrollBar(this));
    setHorizontalScrollBar(new QtScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    d->_tableWidgetStyle = new QtTableWidgetStyle(style());
    setStyle(d->_tableWidgetStyle);
}

QtTableWidget::~QtTableWidget()
{
    Q_D(QtTableWidget);
    delete d->_tableWidgetStyle;
}

void QtTableWidget::insertRows(int row, int count)
{
    if (row < 0 || count <= 0 || row > rowCount())
    {
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        insertRow(row);
    }
}

void QtTableWidget::removeRows(int row, int count)
{
    if (row < 0 || count <= 0 || row >= rowCount())
    {
        return;
    }

    int actualCount = qMin(count, rowCount() - row);

    for (int i = row + actualCount - 1; i >= row; --i)
    {
        removeRow(i);
    }
}

void QtTableWidget::insertColumns(int column, int count)
{
    if (column < 0 || count <= 0 || column > columnCount())
    {
        return;
    }

    for (int i = 0; i < count; ++i)
    {
        insertColumn(column);
    }
}

void QtTableWidget::removeColumns(int column, int count)
{
    if (column < 0 || count <= 0 || column >= columnCount())
    {
        return;
    }

    int actualCount = qMin(count, columnCount() - column);

    for (int i = column + actualCount - 1; i >= column; --i)
    {
        removeColumn(i);
    }
}

void QtTableWidget::setItemText(int row, int column, const QString& text)
{
    if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount())
    {
        return;
    }

    QTableWidgetItem* currentItem = item(row, column);
    if (currentItem)
    {
        currentItem->setText(text);
    }
    else
    {
        QTableWidgetItem* newItem = new QTableWidgetItem(text);
        setItem(row, column, newItem);
    }
}

QString QtTableWidget::itemText(int row, int column) const
{
    if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount())
    {
        return QString();
    }

    QTableWidgetItem* currentItem = item(row, column);
    return currentItem ? currentItem->text() : QString();
}

void QtTableWidget::setRowData(int row, const QStringList& data)
{
    if (row < 0 || row >= rowCount())
    {
        return;
    }

    int maxCol = qMin(data.size(), columnCount());
    for (int col = 0; col < maxCol; ++col)
    {
        setItemText(row, col, data[col]);
    }
}

QStringList QtTableWidget::rowData(int row) const
{
    QStringList data;
    if (row < 0 || row >= rowCount())
    {
        return data;
    }

    for (int col = 0; col < columnCount(); ++col)
    {
        data.append(itemText(row, col));
    }
    return data;
}

void QtTableWidget::setItemHeight(int itemHeight)
{
    Q_D(QtTableWidget);
    if (itemHeight > 0)
    {
        d->_tableWidgetStyle->setItemHeight(itemHeight);
        doItemsLayout();
    }
}

int QtTableWidget::itemHeight() const
{
    Q_D(const QtTableWidget);
    return d->_tableWidgetStyle->itemHeight();
}

void QtTableWidget::setHeaderMargin(int headerMargin)
{
    Q_D(QtTableWidget);
    if (headerMargin >= 0)
    {
        d->_tableWidgetStyle->setHeaderMargin(headerMargin);
        doItemsLayout();
    }
}

int QtTableWidget::headerMargin() const
{
    Q_D(const QtTableWidget);
    return d->_tableWidgetStyle->headerMargin();
}

void QtTableWidget::setIsTransparent(bool isTransparent)
{
    Q_D(QtTableWidget);
    d->_tableWidgetStyle->setIsTransparent(isTransparent);
    update();
}

bool QtTableWidget::isTransparent() const
{
    Q_D(const QtTableWidget);
    return d->_tableWidgetStyle->isTransparent();
}

void QtTableWidget::showEvent(QShowEvent* event)
{
    Q_EMIT tableWidgetShow();
    QTableWidget::showEvent(event);
}

void QtTableWidget::hideEvent(QHideEvent* event)
{
    Q_EMIT tableWidgetHide();
    QTableWidget::hideEvent(event);
}

void QtTableWidget::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtTableWidget);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        int newHoverRow = rowAt(event->pos().y());
        int oldHoverRow = d->_tableWidgetStyle->currentHoverRow();
        if (newHoverRow != oldHoverRow)
        {
            d->_tableWidgetStyle->setCurrentHoverRow(newHoverRow);
            if (oldHoverRow >= 0)
            {
                for (int col = 0; col < columnCount(); ++col)
                {
                    QModelIndex index = model()->index(oldHoverRow, col);
                    update(visualRect(index));
                }
            }
            if (newHoverRow >= 0)
            {
                for (int col = 0; col < columnCount(); ++col)
                {
                    QModelIndex index = model()->index(newHoverRow, col);
                    update(visualRect(index));
                }
            }
        }
    }
    QTableWidget::mouseMoveEvent(event);
}

void QtTableWidget::leaveEvent(QEvent* event)
{
    Q_D(QtTableWidget);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_tableWidgetStyle->setCurrentHoverRow(-1);
        update();
    }
    QTableWidget::leaveEvent(event);
}
