#include "QtTabBar.h"
#include "qtabbar"
#include "styles/qttabbarstyle.h"
#include <QMouseEvent>
#include <qapplication.h>
#include <qnamespace.h>

class QtTabBar;
class QtTabBarStyle;
class QTabBarPrivate;
class QtTabBarPrivate
{
    Q_DECLARE_PUBLIC(QtTabBar)
public:
    QtTabBar* q_ptr;
    QMimeData* mimeData{ nullptr };
    QtTabBarStyle* style{ nullptr };
    int pressedIndex{ -1 };
    QtTabBarPrivate(QtTabBar* q);
    ~QtTabBarPrivate();
};

QtTabBarPrivate::QtTabBarPrivate(QtTabBar* q)
    : q_ptr(q)
{
}
QtTabBarPrivate::~QtTabBarPrivate()
{
}

QtTabBar::QtTabBar(QWidget* parent)
    : QTabBar{ parent }
    , d_ptr(new QtTabBarPrivate(this))
{
    Q_D(QtTabBar);
    setObjectName("QtTabBar");
    setMouseTracking(true);
    setStyleSheet("#QtTabBar{background-color:transparent;}");
    setTabsClosable(true);
    setMovable(true);
    setAcceptDrops(true);
    d->style = new QtTabBarStyle(style());
    setStyle(d->style);
    setExpanding(false);
}

QtTabBar::~QtTabBar()
{
    Q_D(QtTabBar);
    delete d->style;
}

QSize QtTabBar::tabSize() const
{
    Q_D(const QtTabBar);
    return d->style->tabSize();
}
void QtTabBar::setTabSize(QSize value)
{
    Q_D(QtTabBar);
    d->style->setTabSize(value);
}

QSize QtTabBar::sizeHint() const
{
    Q_D(const QtTabBar);
    return QTabBar::sizeHint();;
}

QSize QtTabBar::tabSizeHint(int index) const
{
    Q_D(const QtTabBar); 
    if (d && d->style) {
        return d->style->tabSize(); 
    }
    return QTabBar::tabSizeHint(index);
}

void QtTabBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtTabBar);
    d->pressedIndex = tabAt(event->position().toPoint());
    QTabBar::mousePressEvent(event);
}

void QtTabBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtTabBar);
    QTabBar::mouseMoveEvent(event);
    if(d->pressedIndex >= 0 && event->buttons() & Qt::LeftButton)
    {
        QPoint currentPos = event->pos();
        if(objectName() == "QtCustomTabBar" && count() == 1)
        {
            if(!d->mimeData)
            {
                d->mimeData = new QMimeData();
                d->mimeData->setProperty("DragType", "QtTabBarDrag");
                d->mimeData->setProperty("QtTabBarObject", QVariant::fromValue(this));
                d->mimeData->setProperty("TabSize", d->style->tabSize());
                d->mimeData->setProperty("IsFloatWidget", true);
                QRect currentTabRect = tabRect(currentIndex());
                d->mimeData->setProperty("DragPos", QPoint(currentPos.x() - currentTabRect.x(),
                                                        currentPos.y() - currentTabRect.y()));
                Q_EMIT tabDragCreate(d->mimeData);
                d->mimeData = nullptr;
            }
        }
        else
        {
            QRect moveRect = rect();
            moveRect.adjust(0, -height(), 0, height());
            if(currentPos.x() < 0 || currentPos.x() > width() ||
                currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y())
            {
                if(!d->mimeData)
                {
                    d->mimeData = new QMimeData();
                    d->mimeData->setProperty("DragType", "QtTabBarDrag");
                    d->mimeData->setProperty("QtTabBarObject", QVariant::fromValue(this));
                    d->mimeData->setProperty("TabSize", d->style->tabSize());
                    Q_EMIT tabDragCreate(d->mimeData);
                    d->mimeData = nullptr;
                }
            }
        }
    }
}
void QtTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    Q_D(QtTabBar);
    if(event->mimeData()->property("DragType").toString() == "QtTabBarDrag")
    {
        event->acceptProposedAction();
        auto mimeData = const_cast<QMimeData*>(event->mimeData());
        d->mimeData = mimeData;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        mimeData->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        mimeData->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragEnter(mimeData);
        qApp->processEvents();
        QMouseEvent pressEvent(QEvent::MouseButtonPress,
            QPointF(tabRect(currentIndex()).x() + d->style->tabSize().width() / 2, 0),
            QPointF(tabRect(currentIndex()).x() + d->style->tabSize().width() / 2, 0),
            Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &pressEvent);
        QMouseEvent moveEvent(QEvent::MouseMove, QPointF(event->position().x(), 0),
            QPointF(event->position().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &moveEvent);
    }
    QTabBar::dragEnterEvent(event);
}
void QtTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    Q_D(QtTabBar);
    if(event->mimeData()->property("DragType").toString() == "QtTabBarDrag")
    {
        QMouseEvent moveEvent(QEvent::MouseMove, QPointF(event->position().x(), 0),
            QPointF(event->position().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(this, &moveEvent);
    }
    QWidget::dragMoveEvent(event);
}
void QtTabBar::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_D(QtTabBar);
    if(d->mimeData)
    {
        Q_EMIT tabDragLeave(d->mimeData);
        d->mimeData = nullptr;
    }
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1),
        Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    QTabBar::dragLeaveEvent(event);
}
void QtTabBar::dropEvent(QDropEvent* event)
{
    Q_D(QtTabBar);
    d->mimeData = nullptr;
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1),
        Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    if(objectName() != "QtCustomTabBar")
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragDrop(data);
    }
    QTabBar::dropEvent(event);
}
void QtTabBar::wheelEvent(QWheelEvent* event)
{
    Q_D(QtTabBar);
    QTabBar::wheelEvent(event);
    event->accept();
}
void QtTabBar::paintEvent(QPaintEvent* event)
{
    Q_D(QtTabBar);
    QTabBar::paintEvent(event);
}