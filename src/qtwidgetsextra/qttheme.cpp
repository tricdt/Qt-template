#include "qttheme.h"
#include <QColor>
#include <QPainterPath>
Q_SINGLETON_CREATE_CPP(QtTheme)
class QtThemePrivate
{
public:
    QtTheme* q_ptr;
    QColor lightThemeColorList[43];
    QColor darkThemeColorList[43];
    explicit QtThemePrivate(QtTheme* q);
    ~QtThemePrivate();
    QtThemeType::ThemeMode themeMode{ QtThemeType::Dark };

    void initThemeColor();
};

QtThemePrivate::QtThemePrivate(QtTheme* q)
    : q_ptr(q)
{
}
QtThemePrivate::~QtThemePrivate()
{
}

void QtThemePrivate::initThemeColor()
{
    // QtScrollBar
    lightThemeColorList[QtThemeType::ScrollBarHandle] = QColor(0xA0, 0xA0, 0xA0);
    darkThemeColorList[QtThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

    // QtToggleSwitch
    lightThemeColorList[QtThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5A, 0x5A, 0x5A);
    darkThemeColorList[QtThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);

    // 主题颜色
    lightThemeColorList[QtThemeType::PrimaryNormal] = QColor(0x00, 0x67, 0xC0);
    darkThemeColorList[QtThemeType::PrimaryNormal] = QColor(0x4C, 0xC2, 0xFF);
    lightThemeColorList[QtThemeType::PrimaryHover] = QColor(0x19, 0x75, 0xC5);
    darkThemeColorList[QtThemeType::PrimaryHover] = QColor(0x47, 0xB1, 0xE8);
    lightThemeColorList[QtThemeType::PrimaryPress] = QColor(0x31, 0x83, 0xCA);
    darkThemeColorList[QtThemeType::PrimaryPress] = QColor(0x42, 0xA1, 0xD2);

    // 通用颜色
    // 普通窗体
    lightThemeColorList[QtThemeType::WindowBase] = QColor(0xF3, 0xF3, 0xF3);
    darkThemeColorList[QtThemeType::WindowBase] = QColor(0x20, 0x20, 0x20);
    lightThemeColorList[QtThemeType::WindowCentralStackBase] = QColor(0xFF, 0xFF, 0xFF, 80);
    darkThemeColorList[QtThemeType::WindowCentralStackBase] = QColor(0x3E, 0x3E, 0x3E, 60);

    // 浮动窗体
    lightThemeColorList[QtThemeType::PopupBorder] = QColor(0xD6, 0xD6, 0xD6);
    darkThemeColorList[QtThemeType::PopupBorder] = QColor(0x47, 0x47, 0x47);
    lightThemeColorList[QtThemeType::PopupBorderHover] = QColor(0xCC, 0xCC, 0xCC);
    darkThemeColorList[QtThemeType::PopupBorderHover] = QColor(0x54, 0x54, 0x54);
    lightThemeColorList[QtThemeType::PopupBase] = QColor(0xFA, 0xFA, 0xFA);
    darkThemeColorList[QtThemeType::PopupBase] = QColor(0x2C, 0x2C, 0x2C);
    lightThemeColorList[QtThemeType::PopupHover] = QColor(0xF0, 0xF0, 0xF0);
    darkThemeColorList[QtThemeType::PopupHover] = QColor(0x38, 0x38, 0x38);

    // Dialog窗体
    lightThemeColorList[QtThemeType::DialogBase] = Qt::white;
    darkThemeColorList[QtThemeType::DialogBase] = QColor(0x1F, 0x1F, 0x1F);
    lightThemeColorList[QtThemeType::DialogLayoutArea] = QColor(0xF3, 0xF3, 0xF3);
    darkThemeColorList[QtThemeType::DialogLayoutArea] = QColor(0x20, 0x20, 0x20);

    // 基础颜色
    lightThemeColorList[QtThemeType::BasicText] = Qt::black;
    darkThemeColorList[QtThemeType::BasicText] = Qt::white;
    lightThemeColorList[QtThemeType::BasicTextInvert] = Qt::white;
    darkThemeColorList[QtThemeType::BasicTextInvert] = Qt::black;
    lightThemeColorList[QtThemeType::BasicDetailsText] = QColor(0x87, 0x87, 0x87);
    darkThemeColorList[QtThemeType::BasicDetailsText] = QColor(0xAD, 0xAD, 0xB0);
    lightThemeColorList[QtThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    darkThemeColorList[QtThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    lightThemeColorList[QtThemeType::BasicTextDisable] = QColor(0xB6, 0xB6, 0xB6);
    darkThemeColorList[QtThemeType::BasicTextDisable] = QColor(0xA7, 0xA7, 0xA7);
    lightThemeColorList[QtThemeType::BasicTextPress] = QColor(0x5A, 0x5A, 0x5D);
    darkThemeColorList[QtThemeType::BasicTextPress] = QColor(0xBB, 0xBB, 0xBF);
    lightThemeColorList[QtThemeType::BasicTextCategory] = QColor(0x5C, 0x5C, 0x5F);
    darkThemeColorList[QtThemeType::BasicTextCategory] = QColor(0xCC, 0xCC, 0xCE);
    lightThemeColorList[QtThemeType::BasicBorder] = QColor(0xE5, 0xE5, 0xE5);
    darkThemeColorList[QtThemeType::BasicBorder] = QColor(0x4B, 0x4B, 0x4B);
    lightThemeColorList[QtThemeType::BasicBorderDeep] = QColor(0xA8, 0xA8, 0xA8);
    darkThemeColorList[QtThemeType::BasicBorderDeep] = QColor(0x5C, 0x5C, 0x5C);
    lightThemeColorList[QtThemeType::BasicBorderHover] = QColor(0xDA, 0xDA, 0xDA);
    darkThemeColorList[QtThemeType::BasicBorderHover] = QColor(0x57, 0x57, 0x57);
    lightThemeColorList[QtThemeType::BasicBase] = QColor(0xFD, 0xFD, 0xFD);
    darkThemeColorList[QtThemeType::BasicBase] = QColor(0x34, 0x34, 0x34);
    lightThemeColorList[QtThemeType::BasicBaseDeep] = QColor(0xE6, 0xE6, 0xE6);
    darkThemeColorList[QtThemeType::BasicBaseDeep] = QColor(0x61, 0x61, 0x61);
    lightThemeColorList[QtThemeType::BasicDisable] = QColor(0xF5, 0xF5, 0xF5);
    darkThemeColorList[QtThemeType::BasicDisable] = QColor(0x2A, 0x2A, 0x2A);
    lightThemeColorList[QtThemeType::BasicHover] = QColor(0xF3, 0xF3, 0xF3);
    darkThemeColorList[QtThemeType::BasicHover] = QColor(0x40, 0x40, 0x40);
    lightThemeColorList[QtThemeType::BasicPress] = QColor(0xF7, 0xF7, 0xF7);
    darkThemeColorList[QtThemeType::BasicPress] = QColor(0x3A, 0x3A, 0x3A);
    lightThemeColorList[QtThemeType::BasicSelectedHover] = QColor(0xEB, 0xEB, 0xEB);
    darkThemeColorList[QtThemeType::BasicSelectedHover] = QColor(0x38, 0x38, 0x38);
    lightThemeColorList[QtThemeType::BasicBaseLine] = QColor(0xD1, 0xD1, 0xD1);
    darkThemeColorList[QtThemeType::BasicBaseLine] = QColor(0x45, 0x45, 0x45);
    lightThemeColorList[QtThemeType::BasicHemline] = QColor(0x86, 0x86, 0x86);
    darkThemeColorList[QtThemeType::BasicHemline] = QColor(0x9A, 0x9A, 0x9A);
    lightThemeColorList[QtThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    darkThemeColorList[QtThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    lightThemeColorList[QtThemeType::BasicChute] = QColor(0xB3, 0xB3, 0xB3);
    darkThemeColorList[QtThemeType::BasicChute] = QColor(0x63, 0x63, 0x63);

    // 基础透明
    lightThemeColorList[QtThemeType::BasicAlternating] = QColor(0xEF, 0xEF, 0xEF, 160);
    darkThemeColorList[QtThemeType::BasicAlternating] = QColor(0x45, 0x45, 0x45, 125);
    lightThemeColorList[QtThemeType::BasicBaseAlpha] = QColor(0xFF, 0xFF, 0xFF, 160);
    darkThemeColorList[QtThemeType::BasicBaseAlpha] = QColor(0x2D, 0x2D, 0x2D, 95);
    lightThemeColorList[QtThemeType::BasicBaseDeepAlpha] = QColor(0xCC, 0xCC, 0xCC, 160);
    darkThemeColorList[QtThemeType::BasicBaseDeepAlpha] = QColor(0x72, 0x72, 0x72, 95);
    lightThemeColorList[QtThemeType::BasicHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 70);
    darkThemeColorList[QtThemeType::BasicHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    lightThemeColorList[QtThemeType::BasicPressAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    darkThemeColorList[QtThemeType::BasicPressAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);
    lightThemeColorList[QtThemeType::BasicSelectedAlpha] = QColor(0xCC, 0xCC, 0xCC, 70);
    darkThemeColorList[QtThemeType::BasicSelectedAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    lightThemeColorList[QtThemeType::BasicSelectedHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    darkThemeColorList[QtThemeType::BasicSelectedHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);

    // 状态颜色
    lightThemeColorList[QtThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
    darkThemeColorList[QtThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);

    // Win10顶部边框颜色
    lightThemeColorList[QtThemeType::Win10BorderActive] = QColor(0x6E, 0x6E, 0x6E);
    darkThemeColorList[QtThemeType::Win10BorderActive] = QColor(0x33, 0x33, 0x33);
    lightThemeColorList[QtThemeType::Win10BorderInactive] = QColor(0xA7, 0xA7, 0xA7);
    darkThemeColorList[QtThemeType::Win10BorderInactive] = QColor(0x3D, 0x3D, 0x3D);
}
QtTheme::QtTheme(QObject* parent)
    : QObject{ parent }
    , d_ptr(new QtThemePrivate(this))
{
    Q_D(QtTheme);
    d->initThemeColor();
}

QtTheme::~QtTheme()
{
}

void QtTheme::setThemeMode(QtThemeType::ThemeMode themeMode)
{
    Q_D(QtTheme);
    d->themeMode = themeMode;
    Q_EMIT themeModeChanged(d->themeMode);
}

QtThemeType::ThemeMode QtTheme::getThemeMode() const
{
    Q_D(const QtTheme);
    return d->themeMode;
}

void QtTheme::drawEffectShadow(
    QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
    Q_D(QtTheme);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color =
        d->themeMode == QtThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
    for(int i = 0; i < shadowBorderWidth; i++)
    {
        path.addRoundedRect(widgetRect.x() + shadowBorderWidth - i,
            widgetRect.y() + shadowBorderWidth - i,
            widgetRect.width() - (shadowBorderWidth - i) * 2,
            widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
        int alpha = 1 * (shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(color);
        painter->drawPath(path);
    }
    painter->restore();
}

void QtTheme::setThemeColor(
    QtThemeType::ThemeMode themeMode, QtThemeType::ThemeColor themeColor, QColor newColor)
{
    Q_D(QtTheme);
    if(themeMode == QtThemeType::Light)
    {
        d->lightThemeColorList[themeColor] = newColor;
    }
    else
    {
        d->darkThemeColorList[themeColor] = newColor;
    }
}

const QColor& QtTheme::getThemeColor(
    QtThemeType::ThemeMode themeMode, QtThemeType::ThemeColor themeColor)
{
    Q_D(QtTheme);
    if(themeMode == QtThemeType::Light)
    {
        return d->lightThemeColorList[themeColor];
    }
    else
    {
        return d->darkThemeColorList[themeColor];
    }
}
