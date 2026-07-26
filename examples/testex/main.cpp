#include "mainwindow.h"
#include "widget.h"
#include "widgets/qtapplication.h"
#include "widgets/qtwidget.h"
#include "widgets/qtwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    // 1. Tắt cơ chế tự động lấy cấu hình DPI từ hệ điều hành Windows
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    
    // 2. Sử dụng biến môi trường thay thế của Qt 6 để khóa cứng tỷ lệ scale tổng thể là 2.0
    qputenv("QT_SCALE_FACTOR", "1.25");
    
    // 3. (Tùy chọn bổ sung) Nếu máy có nhiều màn hình, ép toàn bộ các màn hình về mức 2.0
    //qputenv("QT_SCREEN_SCALE_FACTORS", "2.0");
    QApplication a(argc, argv);
    qtApp->init();
    // QFontDatabase::addApplicationFont(":/include/QtAwesome.ttf");
    a.setStyle("Fusion");
    // QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
    //     Qt::HighDpiScaleFactorRoundingPolicy::Round);
    
    // QtWindow w;
    // w.show();

    // QtWidget w;
    // QObject::connect(&w, &QtWidget::themeChangeButtonClicked, [=]() {
    //     if(qtTheme->getThemeMode() == QtThemeType::Dark)
    //     {
    //         qtTheme->setThemeMode(QtThemeType::Light);
    //     }
    //     else
    //     {
    //         qtTheme->setThemeMode(QtThemeType::Dark);
    //     }
    // });
    // w.show();
    // QtWindow w;
    // w.show();
    // MainWindow w;
    // w.show();
    Widget w;
    w.show();
    return a.exec();
}
