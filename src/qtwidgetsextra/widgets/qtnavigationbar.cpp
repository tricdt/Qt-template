#include "qtnavigationbar.h"
#include "qtdef.h"
#include "qtsuggestbox.h"
#include "utils/qtnavigationnode.h"
#include "widgets/qtmenu.h"
#include <qboxlayout.h>

class QtNavigationBarPrivate
{
    Q_DECLARE_PUBLIC(QtNavigationBar)
public:
    bool isTransparent;
    bool isAllowPageOpenInNewWindow;
    int navigationBarWidth;
    QtNavigationBar* q_ptr;
    int navigationViewWidth;
    int userButtonSpacing;
    QtThemeType::ThemeMode themeMode;
    QList<QtSuggestBox::SuggestData> suggestDataList;
    QMap<QString, const QMetaObject*> pageMetaMap;
    QMap<QString, int> pageNewWindowCountMap;
    QMap<QtNavigationNode*, QtMenu*> compactMenuMap;
    QVBoxLayout* userCardLayout{ nullptr };
    QVBoxLayout* userButtonLayout{ nullptr };
    QtNavigationBarPrivate(QtNavigationBar* q);
    ~QtNavigationBarPrivate();
};

QtNavigationBarPrivate::QtNavigationBarPrivate(QtNavigationBar* q)
    : q_ptr(q)
{
}
QtNavigationBarPrivate::~QtNavigationBarPrivate()
{
}

QtNavigationBar::QtNavigationBar(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtNavigationBarPrivate(this))
{
}

QtNavigationBar::~QtNavigationBar()
{
}

bool QtNavigationBar::isTransparent() const
{
    Q_D(const QtNavigationBar);
    return d->isTransparent;
}
void QtNavigationBar::setIsTransparent(bool value)
{
    Q_D(QtNavigationBar);
    d->isTransparent = value;
}

bool QtNavigationBar::isAllowPageOpenInNewWindow() const
{
    Q_D(const QtNavigationBar);
    return d->isAllowPageOpenInNewWindow;
}
void QtNavigationBar::setIsAllowPageOpenInNewWindow(bool value)
{
    Q_D(QtNavigationBar);
    d->isAllowPageOpenInNewWindow = value;
}

int QtNavigationBar::navigationBarWidth() const
{
    Q_D(const QtNavigationBar);
    return d->navigationBarWidth;
}
void QtNavigationBar::setNavigationBarWidth(int value)
{
    Q_D(QtNavigationBar);
    d->navigationBarWidth = value;
}