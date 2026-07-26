#pragma once

#include "qtsingleton.h"
#include "qttabbar.h"
#include "qttabwidget.h"
#include <QWidget>
#include <qtabwidget.h>
#include <qtmetamacros.h>

class QtDragMonitor : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE(QtDragMonitor)
public:
    explicit QtDragMonitor(QObject* parent = nullptr);
    ~QtDragMonitor() override;

    bool isInDrag() const;
    void setIsInDrag(bool value);

private:
    bool _isInDrag;
};

class QtTabWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtTabWidget)
public:
    QtTabWidget* q_ptr;
    bool isTabTransparent;
    bool isContainerAcceptDrops;
    QSize tabSize;
    QtTabBar* tabBar{ nullptr };
    QtTabBar* customTabBar{ nullptr };
    QList<QWidget*> allTabWidgetList;
    QtTabWidgetPrivate(QtTabWidget* q);
    ~QtTabWidgetPrivate();
    void onTabDragCreate(QMimeData* mimeData);
    void onTabDragEnter(QMimeData* mimeData);
    void onTabDragLeave(QMimeData* mimeData);
    void onTabDragDrop(QMimeData* mimeData);
    void onTabCloseRequested(int index);
    void clearAllTabWidgetList();
};