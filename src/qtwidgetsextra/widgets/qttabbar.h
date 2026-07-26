#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qmimedata.h>
#include <qtabbar.h>

class QtTabBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtTabBar : public QTabBar
{
    Q_OBJECT
    Q_PROPERTY(QSize tabSize READ tabSize WRITE setTabSize NOTIFY tabSizeChanged)
public:
    explicit QtTabBar(QWidget* parent = nullptr);
    ~QtTabBar();

    QSize tabSize() const;
    void setTabSize(QSize value);

Q_SIGNALS:
    void tabDragCreate(QMimeData* mimeData);
    void tabDragEnter(QMimeData* mimeData);
    void tabDragLeave(QMimeData* mimeData);
    void tabDragDrop(QMimeData* mimeData);
    void tabSizeChanged();

protected:
    QSize sizeHint() const override;
    QSize tabSizeHint(int index) const override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtTabBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTabBar)
};
