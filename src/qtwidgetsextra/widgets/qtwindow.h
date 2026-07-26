#pragma once

#include "qtappbar.h"
#include "qtwidgetsextra.h"
#include "widgets/qtappbar.h"
#include <QWidget>
#include <qmainwindow.h>

class QtWindowPrivate;
class QTWIDGETSEXTRA_EXPORT QtWindow : public QMainWindow
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
    Q_PROPERTY(bool isStayTop READ isStayTop WRITE setIsStayTop NOTIFY isStayTopChanged)
    Q_PROPERTY(bool isFixedSize READ isFixedSize WRITE setIsFixedSize NOTIFY isFixedSizeChanged)
    Q_PROPERTY(bool isDefaultClosed READ isDefaultClosed WRITE setIsDefaultClosed NOTIFY
            isDefaultClosedChanged)
    Q_PROPERTY(int appBarHeight READ appBarHeight WRITE setAppBarHeight NOTIFY appBarHeightChanged)
    Q_PROPERTY(int themeChangeTime READ themeChangeTime WRITE setThemeChangeTime NOTIFY
            themeChangeTimeChanged)
    Q_PROPERTY(bool isCentralStackedWidgetTransparent READ isCentralStackedWidgetTransparent WRITE
            setIsCentralStackedWidgetTransparent NOTIFY isCentralStackedWidgetTransparentChanged)
    Q_PROPERTY(bool isAllowPageOpenInNewWindow READ isAllowPageOpenInNewWindow WRITE
            setIsAllowPageOpenInNewWindow NOTIFY isAllowPageOpenInNewWindowChanged)
    Q_PROPERTY(bool isNavigationBarEnable READ isNavigationBarEnable WRITE setIsNavigationBarEnable
            NOTIFY isNavigationBarEnableChanged)
    Q_PROPERTY(int navigationBarWidth READ navigationBarWidth WRITE setNavigationBarWidth NOTIFY
            navigationBarWidthChanged)
    Q_PROPERTY(int currentStackIndex READ currentStackIndex WRITE setCurrentStackIndex NOTIFY
            currentStackIndexChanged)
    Q_PROPERTY(QtNavigationType::NavigationDisplayMode navigationBarDisplayMode READ
            navigationBarDisplayMode WRITE setNavigationBarDisplayMode NOTIFY
                navigationBarDisplayModeChanged)
    Q_PROPERTY(QtWindowType::StackSwitchMode stackSwitchMode READ stackSwitchMode WRITE
            setStackSwitchMode NOTIFY stackSwitchModeChanged)

    Q_PROPERTY(QtWindowType::PaintMode windowPaintMode READ windowPaintMode WRITE setWindowPaintMode
            NOTIFY windowPaintModeChanged)
public:
    explicit QtWindow(QWidget* parent = nullptr);
    ~QtWindow();

    void moveToCenter();

    void setCustomWidget(QtAppBarType::CustomArea customArea, QWidget* customWidget,
        QObject* hitTestObject = nullptr, const QString& hitTestFunctionName = "");
    QWidget* customWidget(QtAppBarType::CustomArea customArea) const;

    void setCentralCustomWidget(QWidget* customWidget);
    QWidget* centralCustomWidget() const;

    void setCustomMenu(QMenu* customMenu);
    QMenu* customMenu() const;

    bool isStayTop() const;
    void setIsStayTop(bool value);

    bool isFixedSize() const;
    void setIsFixedSize(bool value);

    bool isDefaultClosed() const;
    void setIsDefaultClosed(bool value);

    int appBarHeight() const;
    void setAppBarHeight(int value);

    int themeChangeTime() const;
    void setThemeChangeTime(int value);

    bool isCentralStackedWidgetTransparent() const;
    void setIsCentralStackedWidgetTransparent(bool value);

    bool isAllowPageOpenInNewWindow() const;
    void setIsAllowPageOpenInNewWindow(bool value);

    bool isNavigationBarEnable() const;
    void setIsNavigationBarEnable(bool value);

    int navigationBarWidth() const;
    void setNavigationBarWidth(int value);

    int currentStackIndex() const;
    void setCurrentStackIndex(int value);

    QtNavigationType::NavigationDisplayMode navigationBarDisplayMode() const;
    void setNavigationBarDisplayMode(QtNavigationType::NavigationDisplayMode value);

    QtWindowType::StackSwitchMode stackSwitchMode() const;
    void setStackSwitchMode(const QtWindowType::StackSwitchMode& value);

    QtWindowType::PaintMode windowPaintMode() const;
    void setWindowPaintMode(QtWindowType::PaintMode value);

Q_SIGNALS:
    void closeButtonClicked();
    void customWidgetChanged();
    void centralCustomWidgetChanged();
    void customMenuChanged();

    void isStayTopChanged();
    void isFixedSizeChanged();
    void isDefaultClosedChanged();
    void appBarHeightChanged();
    void themeChangeTimeChanged();
    void isCentralStackedWidgetTransparentChanged();
    void isAllowPageOpenInNewWindowChanged();
    void isNavigationBarEnableChanged();
    void navigationBarWidthChanged();
    void currentStackIndexChanged();
    void navigationBarDisplayModeChanged();
    void stackSwitchModeChanged();
    void windowPaintModeChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWindow)
};
