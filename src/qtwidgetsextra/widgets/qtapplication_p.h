#pragma once
#include <QObject>
#include "qtdef.h"
#include "qtapplication.h"
class QImage;
class QtApplicationPrivate
{
    Q_DECLARE_PUBLIC(QtApplication)
public:
    QtApplication* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QList<QWidget*> micaWidgetList;
    QImage lightBaseImage;
    QImage darkBaseImage;

    QtApplicationType::WindowDisplayMode windowDisplayMode;
    QString qtMicaImagePath;

    QtApplicationPrivate(QtApplication* q);
    void onThemeModeChanged(QtThemeType::ThemeMode themeMode);
    void initMicaBaseImage(QImage img);
    QRect calculateWindowVirtualGeometry(QWidget* widget);
    void updateMica(QWidget* widget, bool isProcessEvent = true);
    void updateAllMicaWidget();
    void resetAllMicaWidget();
    friend class QtMicaBaseInitObject;
};