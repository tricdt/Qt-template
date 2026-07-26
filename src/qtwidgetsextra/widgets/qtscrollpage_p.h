#include "qtbreadcrumbbar.h"
#include "qtscrollpage.h"
#include "qtscrollpage.h"
#include <qboxlayout.h>
#include <qstackedwidget.h>
class QtScrollPagePrivate
{
    Q_DECLARE_PUBLIC(QtScrollPage)
public:
    QWidget* customWidget;
    QtScrollPage* q_ptr;
    QHBoxLayout* pageTitleLayout{ nullptr };
    QVBoxLayout* mainLayout{ nullptr };
    QStackedWidget* centralStackedWidget{ nullptr };
    QtBreadcrumbBar* breadcrumbBar{ nullptr };
    QMap<QString, int> centralWidgetMap;
    int navigationTargetIndex{ 0 };
    int pageTitleSpacing{ 0 };
    bool isGrabGesture{ false };
    QtScrollPagePrivate(QtScrollPage* q);
    ~QtScrollPagePrivate();
    void switchCentralStackIndex(int targetIndex, int lastIndex);
};