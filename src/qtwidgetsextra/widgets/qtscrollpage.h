#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qwidget.h>

class QtScrollPagePrivate;
class QTWIDGETSEXTRA_EXPORT QtScrollPage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        QWidget* customWidget READ customWidget WRITE setCustomWidget NOTIFY customWidgetChanged)
public:
    explicit QtScrollPage(QWidget* parent = nullptr);
    ~QtScrollPage();

    void addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable = true,
        bool isVerticalGrabGesture = true, qreal mousePressEventDelay = 0.5);

    void navigation(int widgetIndex, bool isLogRoute = true);

    void setPageTitleSpacing(int spacing);
    int pageTitleSpacing() const;
    void setTitleVisible(bool isVisible);

    QWidget* customWidget() const;
    void setCustomWidget(QWidget* value);

Q_SIGNALS:
    void customWidgetChanged(QWidget* widget);

private:
    QScopedPointer<class QtScrollPagePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollPage)
};
