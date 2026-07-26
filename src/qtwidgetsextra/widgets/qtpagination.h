#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtPaginationPrivate;
class QTWIDGETSEXTRA_EXPORT QtPagination : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(int totalPages READ totalPages WRITE setTotalPages NOTIFY totalPagesChanged)
    Q_PROPERTY(int buttonSize READ buttonSize WRITE setButtonSize NOTIFY buttonSizeChanged)
    Q_PROPERTY(int pagerCount READ pagerCount WRITE setPagerCount NOTIFY pagerCountChanged)
    Q_PROPERTY(
        bool jumperVisible READ jumperVisible WRITE setJumperVisible NOTIFY jumperVisibleChanged)
public:
    explicit QtPagination(QWidget* parent = nullptr);
    ~QtPagination();

    int currentPage() const;
    void setCurrentPage(int value);

    int totalPages() const;
    void setTotalPages(int value);

    int buttonSize() const;
    void setButtonSize(int value);

    int pagerCount() const;
    void setPagerCount(int value);

    bool jumperVisible() const;
    void setJumperVisible(bool value);

Q_SIGNALS:
    void currentPageChanged(int page);

    void currentPageChanged();
    void totalPagesChanged();
    void buttonSizeChanged();
    void pagerCountChanged();
    void jumperVisibleChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtPaginationPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPagination)
};
