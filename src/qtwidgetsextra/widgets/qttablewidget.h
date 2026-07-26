#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtablewidget.h>

class QtTableWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtTableWidget : public QTableWidget
{
    Q_OBJECT

    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int headerMargin READ headerMargin WRITE setHeaderMargin NOTIFY headerMarginChanged)
    Q_PROPERTY(
        bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY isTransparentChanged)
public:
    explicit QtTableWidget(QWidget* parent = nullptr);
    ~QtTableWidget();

    void insertRows(int row, int count);
    void removeRows(int row, int count);
    void insertColumns(int column, int count);
    void removeColumns(int column, int count);

    void setItemText(int row, int column, const QString& text);
    QString itemText(int row, int column) const;
    void setRowData(int row, const QStringList& data);
    QStringList rowData(int row) const;

        int itemHeight() const;
    void setItemHeight(int value);

    int headerMargin() const;
    void setHeaderMargin(int value);
    
    bool isTransparent() const;
    void setIsTransparent(bool value);

Q_SIGNALS:
    void itemHeightChanged();
    void headerMarginChanged();
    void isTransparentChanged();
    Q_SIGNAL void tableWidgetShow();
    Q_SIGNAL void tableWidgetHide();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    QScopedPointer<class QtTableWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTableWidget)
};
