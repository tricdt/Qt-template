#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtDrawerHeaderPrivate;
class QTWIDGETSEXTRA_EXPORT QtDrawerHeader : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal expandIconRotate READ expandIconRotate WRITE setExpandIconRotate NOTIFY
            expandIconRotateChanged)
public:
    explicit QtDrawerHeader(QWidget* parent = nullptr);
    ~QtDrawerHeader();

    void setHeaderWidget(QWidget* widget);
    void doExpandOrCollapseAnimation();

    qreal expandIconRotate() const;
    void setExpandIconRotate(qreal value);

    int borderRadius() const;
    void setBorderRadius(int value);

    int isExpand() const;
    void setIsExpand(bool value);
Q_SIGNALS:
    void expandIconRotateChanged();
    Q_SIGNAL void drawerHeaderClicked(bool isExpand);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtDrawerHeaderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDrawerHeader)
};
