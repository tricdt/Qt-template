#pragma once

#include <QObject>
#include "qtdef.h"
#include "qtsingleton.h"
#include "qtwidgetsextra.h"

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windowsx.h>

#define Win7_Origin "6.1.0"
#define Win8_Origin "6.2.0"
#define Win10_Origin "10.0.0"
#define Win10_1809 "10.0.17763"
#define Win10_1903 "10.0.18362"
#define Win10_20H1 "10.0.19041"
#define Win11_Origin "10.0.22000"
#define Win11_22H2 "10.0.22621"
#define qtWinHelper QtWinShadowHelper::getInstance()
class QTWIDGETSEXTRA_EXPORT QtWinShadowHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isWinVersionGreater10 READ isWinVersionGreater10 WRITE setIsWinVersionGreater10
            NOTIFY isWinVersionGreater10Changed)
    Q_PROPERTY(bool isWinVersionGreater11 READ isWinVersionGreater11 WRITE setIsWinVersionGreater11
            NOTIFY isWinVersionGreater11Changed)
    Q_SINGLETON_CREATE(QtWinShadowHelper)
public:
    explicit QtWinShadowHelper(QObject* parent = nullptr);
    ~QtWinShadowHelper();

    bool isWinVersionGreater10() const;
    void setIsWinVersionGreater10(bool value);

    bool isWinVersionGreater11() const;
    void setIsWinVersionGreater11(bool value);

    bool initWinAPI();

    void setWindowShadow(quint64 hwnd);
    void setWindowThemeMode(quint64 hwnd, bool isLightMode);
    void setWindowDisplayMode(QWidget* widget, QtApplicationType::WindowDisplayMode displayMode,
        QtApplicationType::WindowDisplayMode lastDisplayMode);
    bool isCompositionEnabled() const;
    bool isFullScreen(const HWND& hwnd);
    MONITORINFOEXW monitorForWindow(const HWND& hwnd);
    quint32 resizeBorderThickness(const HWND& hwnd);
    quint32 dpiForWindow(const HWND& hwnd);
    int systemMetricsForDpi(const HWND& hwnd, const int index);
    bool compareWindowsVersion(const QString& windowsVersion) const;

Q_SIGNALS:
    void isWinVersionGreater10Changed();
    void isWinVersionGreater11Changed();

private:
    QScopedPointer<class QtWinShadowHelperPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWinShadowHelper)
};
#endif
