#ifndef QTAPPBAR_H
#define QTAPPBAR_H

#include "qtdef.h"
#include <QtWidgets>
#include "qtwidgetsextra.h"

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_H                                                                   \
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
#define Q_TAKEOVER_NATIVEEVENT_H                                                                   \
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#else
#define Q_TAKEOVER_NATIVEEVENT_H
#endif

#ifdef Q_OS_WIN
#define QTAPPBAR_HANDLE(QtAppBar)                                                                  \
    if(QtAppBar)                                                                                   \
    {                                                                                              \
        int ret = QtAppBar->takeOverNativeEvent(eventType, message, result);                       \
        if(ret == -1)                                                                              \
        {                                                                                          \
            return QWidget::nativeEvent(eventType, message, result);                               \
        }                                                                                          \
        return (bool) ret;                                                                         \
    }                                                                                              \
    return QWidget::nativeEvent(eventType, message, result);
#endif

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, QtAppBar)                                                \
    bool CLASS::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)           \
    {                                                                                              \
        QTAPPBAR_HANDLE(QtAppBar)                                                                  \
    }
#else
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, QtAppBar)                                                \
    bool CLASS::nativeEvent(const QByteArray& eventType, void* message, long* result)              \
    {                                                                                              \
        QTAPPBAR_HANDLE(QtAppBar)                                                                  \
    }
#endif
#else
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, QtAppBar)
#endif

class QtAppBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtAppBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isStayTop READ isStayTop WRITE setIsStayTop NOTIFY isStayTopChanged)
    Q_PROPERTY(bool isFixedSize READ isFixedSize WRITE setIsFixedSize NOTIFY isFixedSizeChanged)
    Q_PROPERTY(bool isDefaultClosed READ isDefaultClosed WRITE setIsDefaultClosed NOTIFY
            isDefaultClosedChanged)
    Q_PROPERTY(bool isOnlyAllowMinAndClose READ isOnlyAllowMinAndClose WRITE
            setIsOnlyAllowMinAndClose NOTIFY isOnlyAllowMinAndCloseChanged)
    Q_PROPERTY(int appBarHeight READ appBarHeight WRITE setAppBarHeight NOTIFY appBarHeightChanged)
public:
    explicit QtAppBar(QWidget* parent = nullptr);
    ~QtAppBar();

    bool isStayTop() const;
    void setIsStayTop(bool newIsStayTop);

    bool isFixedSize() const;
    void setIsFixedSize(bool newIsFixedSize);

    bool isDefaultClosed() const;
    void setIsDefaultClosed(bool newIsDefaultClosed);

    bool isOnlyAllowMinAndClose() const;
    void setIsOnlyAllowMinAndClose(bool newIsOnlyAllowMinAndClose);

    int appBarHeight() const;
    void setAppBarHeight(int newAppBarHeight);

    void setCustomWidget(QtAppBarType::CustomArea customArea, QWidget* customWidget,
        QObject* hitTestObject = nullptr, const QString& hitTestFunctionName = "");
    QWidget* customWidget(QtAppBarType::CustomArea customArea) const;

    void setCustomMenu(QMenu* customMenu);
    QMenu* customMenu() const;

    void setWindowButtonFlag(QtAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(QtAppBarType::ButtonFlags buttonFlags);
    QtAppBarType::ButtonFlags windowButtonFlags() const;

    void setRouteBackButtonEnable(bool isEnable);
    void setRouteForwardButtonEnable(bool isEnable);

    void closeWindow();

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#else
    int takeOverNativeEvent(const QByteArray& eventType, void* message, long* result);
#endif
#endif

Q_SIGNALS:
    void isStayTopChanged();
    void isFixedSizeChanged();
    void isDefaultClosedChanged();
    void isOnlyAllowMinAndCloseChanged();
    void appBarHeightChanged();
    void routeBackButtonClicked();
    void routeForwardButtonClicked();
    void navigationButtonClicked();
    void themeChangeButtonClicked();
    void closeButtonClicked();
    void customWidgetChanged();
    void customMenuChanged();

protected:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
#ifdef Q_OS_WIN
    virtual void paintEvent(QPaintEvent* event) override;
#endif

private:
    QScopedPointer<class QtAppBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtAppBar)
};

#endif // QTAPPBAR_H
