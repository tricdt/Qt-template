#pragma once

#include "qtcustomwidget.h"
#include "qttabbar.h"
#include "qttabwidget.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtCustomTabWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtCustomTabWidget : public QtCustomWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isFinished READ isFinished WRITE setIsFinished NOTIFY isFinishedChanged FINAL)
public:
    explicit QtCustomTabWidget(QWidget* parent = nullptr);
    ~QtCustomTabWidget();

    void addTab(QWidget* widget, QIcon tabIcon, const QString& tabTitle);

    bool isFinished() const;
    void setIsFinished(bool value);

    bool isAllowLeave() const;
    void setIsAllowLeave(bool value);

    QtTabBar* customTabBar() const;
    void setCustomTabBar(QtTabBar* value);

    QtTabWidget* customTabWidget() const;
    void setCustomTabWidget(QtTabWidget* value);

    Q_INVOKABLE bool processHitTest();

protected:
    void closeEvent(QCloseEvent* event) override;

Q_SIGNALS:
    void isFinishedChanged();

private:
    QScopedPointer<class QtCustomTabWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCustomTabWidget)
};
