#pragma once

#include <QObject>
#include "qtdef.h"
#include "qtsingleton.h"
#include "qtwidgetsextra.h"
#include <QPainter>

#define qtTheme QtTheme::getInstance()
#define QtThemeColor(themeMode, themeColor)                                                        \
    qtTheme->getThemeColor(themeMode, QtThemeType::themeColor)
class QtThemePrivate;
class QTWIDGETSEXTRA_EXPORT QtTheme : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(QtTheme)
public:
    void setThemeMode(QtThemeType::ThemeMode themeMode);
    QtThemeType::ThemeMode getThemeMode() const;

    void drawEffectShadow(
        QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    void setThemeColor(
        QtThemeType::ThemeMode themeMode, QtThemeType::ThemeColor themeColor, QColor newColor);
    const QColor& getThemeColor(
        QtThemeType::ThemeMode themeMode, QtThemeType::ThemeColor themeColor);
Q_SIGNALS:
    Q_SIGNAL void themeModeChanged(QtThemeType::ThemeMode themeMode);

private:
    explicit QtTheme(QObject* parent = nullptr);
    ~QtTheme();
    QScopedPointer<class QtThemePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTheme)
};
