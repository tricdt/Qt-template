#include "qtwinshadowhelper.h"
#ifdef Q_OS_WIN
#include <QDebug>
#include <QWidget>
#include "Windows.h"
#ifndef NTSTATUS
typedef LONG NTSTATUS;
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif

class QtWinShadowHelperPrivate
{
    Q_DECLARE_PUBLIC(QtWinShadowHelper)
public:
    QtWinShadowHelper* q_ptr;
    bool isWinVersionGreater10{ true };
    bool isWinVersionGreater11{ true };
    enum _DWM_SYSTEMBACKDROP_TYPE
    {
        _DWMSBT_AUTO,
        _DWMSBT_NONE,
        _DWMSBT_MAINWINDOW,
        _DWMSBT_TRANSIENTWINDOW,
        _DWMSBT_TABBEDWINDOW
    };
    enum _DWMWINDOWATTRIBUTE
    {
        _DWMWA_USE_HOSTBACKDROPBRUSH = 17,
        _DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 = 19,
        _DWMWA_USE_IMMERSIVE_DARK_MODE = 20,
        _DWMWA_WINDOW_CORNER_PREFERENCE = 33,
        _DWMWA_VISIBLE_FRAME_BORDER_THICKNESS = 37,
        _DWMWA_SYSTEMBACKDROP_TYPE = 38,
        _DWMWA_MICA_EFFECT = 1029
    };
    enum _WINDOWCOMPOSITIONATTRIB
    {
        _WCA_UNDEFINED = 0,
        _WCA_NCRENDERING_ENABLED = 1,
        _WCA_NCRENDERING_POLICY = 2,
        _WCA_TRANSITIONS_FORCEDISABLED = 3,
        _WCA_ALLOW_NCPAINT = 4,
        _WCA_CAPTION_BUTTON_BOUNDS = 5,
        _WCA_NONCLIENT_RTL_LAYOUT = 6,
        _WCA_FORCE_ICONIC_REPRESENTATION = 7,
        _WCA_EXTENDED_FRAME_BOUNDS = 8,
        _WCA_HAS_ICONIC_BITMAP = 9,
        _WCA_THEME_ATTRIBUTES = 10,
        _WCA_NCRENDERING_EXILED = 11,
        _WCA_NCADORNMENTINFO = 12,
        _WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
        _WCA_VIDEO_OVERLAY_ACTIVE = 14,
        _WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
        _WCA_DISALLOW_PEEK = 16,
        _WCA_CLOAK = 17,
        _WCA_CLOAKED = 18,
        _WCA_ACCENT_POLICY = 19,
        _WCA_FREEZE_REPRESENTATION = 20,
        _WCA_EVER_UNCLOAKED = 21,
        _WCA_VISUAL_OWNER = 22,
        _WCA_HOLOGRAPHIC = 23,
        _WCA_EXCLUDED_FROM_DDA = 24,
        _WCA_PASSIVEUPDATEMODE = 25,
        _WCA_USEDARKMODECOLORS = 26,
        _WCA_CORNER_STYLE = 27,
        _WCA_PART_COLOR = 28,
        _WCA_DISABLE_MOVESIZE_FEEDBACK = 29,
        _WCA_LAST = 30
    };

    enum _ACCENT_STATE
    {
        _ACCENT_DISABLED = 0,
        _ACCENT_ENABLE_GRADIENT = 1,
        _ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        _ACCENT_ENABLE_BLURBEHIND = 3,
        _ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
        _ACCENT_ENABLE_HOST_BACKDROP = 5,
        _ACCENT_INVALID_STATE = 6
    };
    enum _ACCENT_FLAG
    {
        _ACCENT_NONE = 0,
        _ACCENT_ENABLE_ACRYLIC = 1,
        _ACCENT_ENABLE_ACRYLIC_WITH_LUMINOSITY = 482
    };
    struct _ACCENT_POLICY
    {
        DWORD dwAccentState;
        DWORD dwAccentFlags;
        DWORD dwGradientColor;
        DWORD dwAnimationId;
    };
    struct _WINDOWCOMPOSITIONATTRIBDATA
    {
        _WINDOWCOMPOSITIONATTRIB Attrib;
        PVOID pvData;
        SIZE_T cbData;
    };
    enum _MONITOR_DPI_TYPE
    {
        MDT_EFFECTIVE_DPI = 0,
        MDT_ANGULAR_DPI = 1,
        MDT_RAW_DPI = 2,
        MDT_DEFAULT = MDT_EFFECTIVE_DPI
    };

    using DwmExtendFrameIntoClientAreaFunc = HRESULT(WINAPI*)(HWND hWnd, const MARGINS* pMarInset);
    using DwmSetWindowAttributeFunc = HRESULT(WINAPI*)(
        HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
    using DwmIsCompositionEnabledFunc = HRESULT(WINAPI*)(BOOL* pfEnabled);
    using RtlGetVersionFunc = NTSTATUS(WINAPI*)(PRTL_OSVERSIONINFOW);
    using DwmEnableBlurBehindWindowFunc = HRESULT(WINAPI*)(
        HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
    using SetWindowCompositionAttributeFunc = BOOL(WINAPI*)(
        HWND hwnd, const _WINDOWCOMPOSITIONATTRIBDATA*);
    using GetDpiForWindowFunc = UINT(WINAPI*)(HWND hwnd);
    using GetDpiForMonitorFunc = HRESULT(WINAPI*)(
        HMONITOR hmonitor, _MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY);
    using GetSystemMetricsForDpiFunc = int(WINAPI*)(int index, UINT dpi);

    DwmExtendFrameIntoClientAreaFunc _dwmExtendFrameIntoClientArea{ nullptr };
    DwmSetWindowAttributeFunc _dwmSetWindowAttribute{ nullptr };
    DwmIsCompositionEnabledFunc _dwmIsCompositionEnabled{ nullptr };
    DwmEnableBlurBehindWindowFunc _dwmEnableBlurBehindWindow{ nullptr };
    SetWindowCompositionAttributeFunc _setWindowCompositionAttribute{ nullptr };
    GetDpiForWindowFunc _getDpiForWindow{ nullptr };
    GetDpiForMonitorFunc _getDpiForMonitor{ nullptr };
    GetSystemMetricsForDpiFunc _getSystemMetricsForDpi{ nullptr };
    RTL_OSVERSIONINFOW _windowsVersion{};
    void _externWindowMargins(HWND hwnd);
    QtWinShadowHelperPrivate(QtWinShadowHelper* q);

    bool initWinAPI();
    void setWindowShadow(quint64 hwnd);
    void setWindowThemeMode(quint64 hwnd, bool isLightMode);
    void setWindowDisplayMode(QWidget* widget, QtApplicationType::WindowDisplayMode displayMode,
        QtApplicationType::WindowDisplayMode lastDisplayMode);
    bool isCompositionEnabled() const;
    bool isFullScreen(const HWND& hwnd);
    MONITORINFOEXW monitorForWindow(const HWND& hwnd);
    quint32 resizeBorderThickness(const HWND& hwnd);
    int systemMetricsForDpi(const HWND& hwnd, const int index);
    quint32 dpiForWindow(const HWND& hwnd);
    bool compareWindowsVersion(const QString& windowsVersion) const;
};

QtWinShadowHelperPrivate::QtWinShadowHelperPrivate(QtWinShadowHelper* q)
    : q_ptr(q)
{
    isWinVersionGreater10 = true;
    isWinVersionGreater11 = true;
    HMODULE module = LoadLibraryW(L"ntdll.dll");
    if(module)
    {
        auto pRtlGetVersion =
            reinterpret_cast<RtlGetVersionFunc>(::GetProcAddress(module, "RtlGetVersion"));
        Q_ASSERT(pRtlGetVersion);
        _windowsVersion.dwOSVersionInfoSize = sizeof(_windowsVersion);
        pRtlGetVersion(&_windowsVersion);
        isWinVersionGreater10 = compareWindowsVersion(Win10_Origin);
        isWinVersionGreater11 = compareWindowsVersion(Win11_Origin);
    }
}

void QtWinShadowHelperPrivate::_externWindowMargins(HWND hwnd)
{
    static const MARGINS margins = { 65536, 0, 0, 0 };
    if(_dwmExtendFrameIntoClientArea)
    {
        _dwmExtendFrameIntoClientArea(hwnd, &margins);
    }
}

bool QtWinShadowHelperPrivate::initWinAPI()
{
    HMODULE dwmModule = LoadLibraryW(L"dwmapi.dll");
    if(dwmModule)
    {
        if(!_dwmExtendFrameIntoClientArea)
        {
            _dwmExtendFrameIntoClientArea = reinterpret_cast<DwmExtendFrameIntoClientAreaFunc>(
                GetProcAddress(dwmModule, "DwmExtendFrameIntoClientArea"));
        }
        if(!_dwmSetWindowAttribute)
        {
            _dwmSetWindowAttribute = reinterpret_cast<DwmSetWindowAttributeFunc>(
                GetProcAddress(dwmModule, "DwmSetWindowAttribute"));
        }
        if(!_dwmIsCompositionEnabled)
        {
            _dwmIsCompositionEnabled = reinterpret_cast<DwmIsCompositionEnabledFunc>(
                GetProcAddress(dwmModule, "DwmIsCompositionEnabled"));
        }
        if(!_dwmEnableBlurBehindWindow)
        {
            _dwmEnableBlurBehindWindow = reinterpret_cast<DwmEnableBlurBehindWindowFunc>(
                GetProcAddress(dwmModule, "DwmEnableBlurBehindWindow"));
        }
        if(!(_dwmExtendFrameIntoClientArea && _dwmSetWindowAttribute && _dwmIsCompositionEnabled &&
               _dwmEnableBlurBehindWindow))
        {
            qCritical() << "Dwm Func Init Incomplete!";
            return false;
        }
    }
    else
    {
        qCritical() << "dwmapi.dll Load Fail!";
        return false;
    }
    HMODULE user32Module = LoadLibraryW(L"user32.dll");
    if(user32Module)
    {
        if(!_setWindowCompositionAttribute)
        {
            _setWindowCompositionAttribute = reinterpret_cast<SetWindowCompositionAttributeFunc>(
                GetProcAddress(user32Module, "SetWindowCompositionAttribute"));
        }
        if(!_getDpiForWindow)
        {
            _getDpiForWindow = reinterpret_cast<GetDpiForWindowFunc>(
                GetProcAddress(user32Module, "GetDpiForWindow"));
        }
        if(!_getSystemMetricsForDpi)
        {
            _getSystemMetricsForDpi = reinterpret_cast<GetSystemMetricsForDpiFunc>(
                GetProcAddress(user32Module, "GetSystemMetricsForDpi"));
        }
        if(!(_setWindowCompositionAttribute && _getDpiForWindow && _getSystemMetricsForDpi))
        {
            qCritical() << "User32 Func Init Incomplete!";
            return false;
        }
    }
    else
    {
        qCritical() << "user32.dll Load Fail!";
        return false;
    }

    HMODULE shCoreModule = LoadLibraryW(L"SHCore.dll");
    if(shCoreModule)
    {
        if(!_getDpiForMonitor)
        {
            _getDpiForMonitor = reinterpret_cast<GetDpiForMonitorFunc>(
                GetProcAddress(shCoreModule, "GetDpiForMonitor"));
        }
        if(!(_getDpiForMonitor))
        {
            qCritical() << "SHCore Func Init Incomplete!";
            return false;
        }
    }
    else
    {
        qCritical() << "SHCore.dll Load Fail!";
        return false;
    }
    return true;
}

void QtWinShadowHelperPrivate::setWindowShadow(quint64 hwnd)
{
    static const MARGINS shadow = { 1, 0, 0, 0 };
    _dwmExtendFrameIntoClientArea((HWND) hwnd, &shadow);
}

void QtWinShadowHelperPrivate::setWindowThemeMode(quint64 hwnd, bool isLightMode)
{
    if(!compareWindowsVersion(Win10_1809) || !_dwmSetWindowAttribute)
    {
        return;
    }
    BOOL bIsLightMode = !isLightMode;
    _DWMWINDOWATTRIBUTE dwAttritube = compareWindowsVersion(Win10_20H1)
                                          ? _DWMWA_USE_IMMERSIVE_DARK_MODE
                                          : _DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1;
    _dwmSetWindowAttribute((HWND) hwnd, dwAttritube, &bIsLightMode, sizeof(bIsLightMode));
}

void QtWinShadowHelperPrivate::setWindowDisplayMode(QWidget* widget,
    QtApplicationType::WindowDisplayMode displayMode,
    QtApplicationType::WindowDisplayMode lastDisplayMode)
{
    HWND winHwnd = (HWND) widget->winId();
    switch(lastDisplayMode)
    {
        case QtApplicationType::Mica: {
            if(!compareWindowsVersion(Win11_Origin))
            {
                break;
            }
            if(compareWindowsVersion(Win11_22H2))
            {
                const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_AUTO;
                _dwmSetWindowAttribute(
                    winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            }
            else
            {
                const BOOL isEnable = FALSE;
                _dwmSetWindowAttribute(winHwnd, _DWMWA_MICA_EFFECT, &isEnable, sizeof(isEnable));
            }
            break;
        }
        case QtApplicationType::MicaAlt: {
            if(!compareWindowsVersion(Win11_22H2))
            {
                break;
            }
            const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_AUTO;
            _dwmSetWindowAttribute(
                winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            break;
        }
        case QtApplicationType::Acrylic: {
            if(!compareWindowsVersion(Win11_Origin))
            {
                break;
            }
            const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_AUTO;
            _dwmSetWindowAttribute(
                winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            break;
        }
        case QtApplicationType::DWMBlur: {
            if(compareWindowsVersion(Win7_Origin))
            {
                _ACCENT_POLICY policy{};
                policy.dwAccentState = _ACCENT_DISABLED;
                policy.dwAccentFlags = _ACCENT_NONE;
                _WINDOWCOMPOSITIONATTRIBDATA wcad{};
                wcad.Attrib = _WCA_ACCENT_POLICY;
                wcad.pvData = &policy;
                wcad.cbData = sizeof(policy);
                _setWindowCompositionAttribute(winHwnd, &wcad);
            }
            else
            {
                DWM_BLURBEHIND bb{};
                bb.fEnable = FALSE;
                bb.dwFlags = DWM_BB_ENABLE;
                _dwmEnableBlurBehindWindow(winHwnd, &bb);
            }
            break;
        }
        default: {
            break;
        }
    }

    switch(displayMode)
    {
        case QtApplicationType::Mica: {
            if(!compareWindowsVersion(Win11_Origin))
            {
                break;
            }
            _externWindowMargins(winHwnd);
            if(compareWindowsVersion(Win11_22H2))
            {
                const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_MAINWINDOW;
                _dwmSetWindowAttribute(
                    winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            }
            else
            {
                const BOOL enable = TRUE;
                _dwmSetWindowAttribute(winHwnd, _DWMWA_MICA_EFFECT, &enable, sizeof(enable));
            }
            break;
        }
        case QtApplicationType::MicaAlt: {
            if(!compareWindowsVersion(Win11_22H2))
            {
                break;
            }
            _externWindowMargins(winHwnd);
            const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_TABBEDWINDOW;
            _dwmSetWindowAttribute(
                winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            break;
        }
        case QtApplicationType::Acrylic: {
            if(!compareWindowsVersion(Win11_Origin))
            {
                break;
            }
            _externWindowMargins(winHwnd);
            const _DWM_SYSTEMBACKDROP_TYPE backdropType = _DWMSBT_TRANSIENTWINDOW;
            _dwmSetWindowAttribute(
                winHwnd, _DWMWA_SYSTEMBACKDROP_TYPE, &backdropType, sizeof(backdropType));
            break;
        }
        case QtApplicationType::DWMBlur: {
            MARGINS windowMargins = { 0, 1, 0, 0 };
            _dwmExtendFrameIntoClientArea(winHwnd, &windowMargins);
            if(compareWindowsVersion(Win7_Origin))
            {
                _ACCENT_POLICY policy{};
                policy.dwAccentState = _ACCENT_ENABLE_BLURBEHIND;
                policy.dwAccentFlags = _ACCENT_NONE;
                _WINDOWCOMPOSITIONATTRIBDATA wcad{};
                wcad.Attrib = _WCA_ACCENT_POLICY;
                wcad.pvData = &policy;
                wcad.cbData = sizeof(policy);
                _setWindowCompositionAttribute(winHwnd, &wcad);
            }
            else
            {
                DWM_BLURBEHIND bb{};
                bb.fEnable = TRUE;
                bb.dwFlags = DWM_BB_ENABLE;
                _dwmEnableBlurBehindWindow(winHwnd, &bb);
            }
            break;
        }
        default: {
            break;
        }
    }
}

bool QtWinShadowHelperPrivate::isCompositionEnabled() const
{
    BOOL isCompositionEnabled = false;
    if(_dwmIsCompositionEnabled)
    {
        _dwmIsCompositionEnabled(&isCompositionEnabled);
    }
    return isCompositionEnabled;
}

bool QtWinShadowHelperPrivate::isFullScreen(const HWND& hwnd)
{
    RECT windowRect{};
    ::GetWindowRect(hwnd, &windowRect);
    RECT rcMonitor = monitorForWindow(hwnd).rcMonitor;
    return windowRect.top == rcMonitor.top && windowRect.left == rcMonitor.left &&
           windowRect.right == rcMonitor.right && windowRect.bottom == rcMonitor.bottom;
}

MONITORINFOEXW QtWinShadowHelperPrivate::monitorForWindow(const HWND& hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEXW monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    ::GetMonitorInfoW(monitor, &monitorInfo);
    return monitorInfo;
}

quint32 QtWinShadowHelperPrivate::resizeBorderThickness(const HWND& hwnd)
{
    return systemMetricsForDpi(hwnd, SM_CXSIZEFRAME) + systemMetricsForDpi(hwnd, SM_CXPADDEDBORDER);
}

int QtWinShadowHelperPrivate::systemMetricsForDpi(const HWND& hwnd, const int index)
{
    const quint32 dpi = dpiForWindow(hwnd);
    if(_getSystemMetricsForDpi)
    {
        return _getSystemMetricsForDpi(index, dpi);
    }
    const int result = ::GetSystemMetrics(index);
    if(dpi != USER_DEFAULT_SCREEN_DPI)
    {
        return result;
    }
    const qreal dpr = qreal(dpi) / qreal(USER_DEFAULT_SCREEN_DPI);
    return qRound(qreal(result) / dpr);
}

quint32 QtWinShadowHelperPrivate::dpiForWindow(const HWND& hwnd)
{
    if(_getDpiForWindow)
    {
        return _getDpiForWindow(hwnd);
    }
    else if(_getDpiForMonitor)
    {
        HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
        UINT dpiX{ 0 };
        UINT dpiY{ 0 };
        _getDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
        return dpiX;
    }
    else
    {
        HDC hdc = ::GetDC(nullptr);
        const int dpiX = ::GetDeviceCaps(hdc, LOGPIXELSX);
        ::ReleaseDC(nullptr, hdc);
        return quint32(dpiX);
    }
}

bool QtWinShadowHelperPrivate::compareWindowsVersion(const QString& windowsVersion) const
{
    QStringList versionList = windowsVersion.split(".");
    if(versionList.count() != 3)
    {
        return false;
    }
    return (_windowsVersion.dwMajorVersion > versionList[0].toUInt()) ||
           (_windowsVersion.dwMajorVersion == versionList[0].toUInt() &&
               (_windowsVersion.dwMinorVersion > versionList[1].toUInt() ||
                   _windowsVersion.dwBuildNumber >= versionList[2].toUInt()));
}

QtWinShadowHelper::QtWinShadowHelper(QObject* parent)
    : QObject{ parent }
    , d_ptr(new QtWinShadowHelperPrivate(this))
{
}

QtWinShadowHelper::~QtWinShadowHelper()
{
}

bool QtWinShadowHelper::isWinVersionGreater10() const
{
    Q_D(const QtWinShadowHelper);
    return d->isWinVersionGreater10;
}
void QtWinShadowHelper::setIsWinVersionGreater10(bool value)
{
    Q_D(QtWinShadowHelper);
    if(d->isWinVersionGreater10 == value)
        return;
    d->isWinVersionGreater10 = value;
    Q_EMIT isWinVersionGreater10Changed();
}

bool QtWinShadowHelper::isWinVersionGreater11() const
{
    Q_D(const QtWinShadowHelper);
    return d->isWinVersionGreater11;
}
void QtWinShadowHelper::setIsWinVersionGreater11(bool value)
{
    Q_D(QtWinShadowHelper);
    if(d->isWinVersionGreater11 == value)
        return;
    d->isWinVersionGreater11 = value;
    Q_EMIT isWinVersionGreater11Changed();
}

bool QtWinShadowHelper::initWinAPI()
{
    Q_D(QtWinShadowHelper);
    return d->initWinAPI();
}

void QtWinShadowHelper::setWindowShadow(quint64 hwnd)
{
    Q_D(QtWinShadowHelper);
    d->setWindowShadow(hwnd);
}
void QtWinShadowHelper::setWindowThemeMode(quint64 hwnd, bool isLightMode)
{
    Q_D(QtWinShadowHelper);
    d->setWindowThemeMode(hwnd, isLightMode);
}
void QtWinShadowHelper::setWindowDisplayMode(QWidget* widget,
    QtApplicationType::WindowDisplayMode displayMode,
    QtApplicationType::WindowDisplayMode lastDisplayMode)
{
    Q_D(QtWinShadowHelper);
    d->setWindowDisplayMode(widget, displayMode, lastDisplayMode);
}
bool QtWinShadowHelper::isCompositionEnabled() const
{
    Q_D(const QtWinShadowHelper);
    return d->isCompositionEnabled();
}
bool QtWinShadowHelper::isFullScreen(const HWND& hwnd)
{
    Q_D(QtWinShadowHelper);
    return d->isFullScreen(hwnd);
}
MONITORINFOEXW QtWinShadowHelper::monitorForWindow(const HWND& hwnd)
{
    Q_D(QtWinShadowHelper);
    return d->monitorForWindow(hwnd);
}
quint32 QtWinShadowHelper::resizeBorderThickness(const HWND& hwnd)
{
    Q_D(QtWinShadowHelper);
    return d->resizeBorderThickness(hwnd);
}
quint32 QtWinShadowHelper::dpiForWindow(const HWND& hwnd)
{
    Q_D(QtWinShadowHelper);
    return d->dpiForWindow(hwnd);
}
int QtWinShadowHelper::systemMetricsForDpi(const HWND& hwnd, const int index)
{
    Q_D(QtWinShadowHelper);
    return d->systemMetricsForDpi(hwnd, index);
}
bool QtWinShadowHelper::compareWindowsVersion(const QString& windowsVersion) const
{
    Q_D(const QtWinShadowHelper);
    return d->compareWindowsVersion(windowsVersion);
}

#endif