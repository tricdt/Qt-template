#pragma once

#include "qttabbar.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtabwidget.h>
#include <qtmetamacros.h>

class QtTabWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isTabTransparent READ isTabTransparent WRITE setIsTabTransparent NOTIFY
            isTabTransparentChanged)
    Q_PROPERTY(bool isContainerAcceptDrops READ isContainerAcceptDrops WRITE
            setIsContainerAcceptDrops NOTIFY isContainerAcceptDropsChanged)
    Q_PROPERTY(QSize tabSize READ tabSize WRITE setTabSize NOTIFY tabSizeChanged)
public:
    explicit QtTabWidget(QWidget* parent = nullptr);
    ~QtTabWidget();

    bool isTabTransparent() const;
    void setIsTabTransparent(bool value);

    bool isContainerAcceptDrops() const;
    void setIsContainerAcceptDrops(bool value);

    QSize tabSize() const;
    void setTabSize(QSize value);

    QtTabBar* customTabBar() const;
    void setCustomTabBar(QtTabBar* value);

    Q_SLOT void onTabDragCreate(QMimeData* mimeData);
    Q_SLOT void onTabDragEnter(QMimeData* mimeData);
    Q_SLOT void onTabDragLeave(QMimeData* mimeData);
    Q_SLOT void onTabDragDrop(QMimeData* mimeData);
    Q_SLOT void onTabCloseRequested(int index);

Q_SIGNALS:
    void isTabTransparentChanged();
    void isContainerAcceptDropsChanged();
    void tabSizeChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
    virtual void tabInserted(int index) override;

private:
    QScopedPointer<class QtTabWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTabWidget)
};
