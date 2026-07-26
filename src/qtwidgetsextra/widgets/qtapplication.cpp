#include "qtapplication.h"
#include "utils/qtmicabaseinitobject.h"
#include "qttheme.h"
#include "utils/qtwinshadowhelper.h"
#include <qobject.h>
#include "qtapplication_p.h"

QtApplicationPrivate::QtApplicationPrivate(QtApplication* q)
    : q_ptr(q)
{
    qtMicaImagePath = ":/include/Image/MicaBase.png";
    windowDisplayMode = QtApplicationType::Normal;
    themeMode = qtTheme->getThemeMode();
    QObject::connect(
        qtTheme, &QtTheme::themeModeChanged, [this]() { onThemeModeChanged(themeMode); });
}

void QtApplicationPrivate::onThemeModeChanged(QtThemeType::ThemeMode themeMode)
{
    this->themeMode = themeMode;
    switch(windowDisplayMode)
    {
        case QtApplicationType::Normal: {
            break;
        }
        case QtApplicationType::QtMica: {
            updateAllMicaWidget();
            break;
        }
        default: {
#ifdef Q_OS_WIN
            for(auto widget : micaWidgetList)
            {
                QtWinShadowHelper::getInstance()->setWindowThemeMode(
                    widget->winId(), themeMode == QtThemeType::Light);
            }
#endif
            break;
        }
    }
}

void QtApplicationPrivate::initMicaBaseImage(QImage img)
{
    Q_Q(QtApplication);
    if(img.isNull())
        return;
    QThread* initThread = new QThread();
    QtMicaBaseInitObject* initObject = new QtMicaBaseInitObject(this, nullptr);
    initObject->moveToThread(initThread);
    QObject::connect(initThread, &QThread::started, initObject,
        [initObject, img]() { initObject->onInitMicaBase(img); });
    QObject::connect(initObject, &QtMicaBaseInitObject::initFinished, q, [this, initThread]() {
        Q_EMIT this->q_ptr->windowDisplayModeChanged();
        updateAllMicaWidget();
        initThread->quit();
        initThread->deleteLater();
    });
    QObject::connect(initThread, &QThread::finished, initObject, &QObject::deleteLater);
    initThread->start();
}
QRect QtApplicationPrivate::calculateWindowVirtualGeometry(QWidget* widget)
{
    QRect geometry = widget->geometry();
    qreal xImageRatio = 1, yImageRatio = 1;
    QRect relativeGeometry;
    if(qApp->screens().count() > 1)
    {
        QScreen* currentScreen = qApp->screenAt(geometry.topLeft());
        if(currentScreen)
        {
            QRect screenGeometry = currentScreen->availableGeometry();
            xImageRatio = (qreal) lightBaseImage.width() / screenGeometry.width();
            yImageRatio = (qreal) lightBaseImage.height() / screenGeometry.height();
            relativeGeometry = QRect((geometry.x() - screenGeometry.x()) * xImageRatio,
                (geometry.y() - screenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio,
                geometry.height() * yImageRatio);
            return relativeGeometry;
        }
    }
    QRect primaryScreenGeometry = qApp->primaryScreen()->availableGeometry();
    xImageRatio = (qreal) lightBaseImage.width() / primaryScreenGeometry.width();
    yImageRatio = (qreal) lightBaseImage.height() / primaryScreenGeometry.height();
    relativeGeometry = QRect((geometry.x() - primaryScreenGeometry.x()) * xImageRatio,
        (geometry.y() - primaryScreenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio,
        geometry.height() * yImageRatio);
    return relativeGeometry;
}
void QtApplicationPrivate::updateMica(QWidget* widget, bool isProcessEvent)
{
    if(widget->isVisible())
    {
        QPalette palette = widget->palette();
        if(themeMode == QtThemeType::Light)
        {
            palette.setBrush(QPalette::Window,
                lightBaseImage.copy(calculateWindowVirtualGeometry(widget))
                    .scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            palette.setBrush(QPalette::Window,
                darkBaseImage.copy(calculateWindowVirtualGeometry(widget))
                    .scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        widget->setPalette(palette);
        if(isProcessEvent)
        {
            QApplication::processEvents();
        }
    }
}
void QtApplicationPrivate::updateAllMicaWidget()
{
    if(windowDisplayMode == QtApplicationType::WindowDisplayMode::QtMica)
    {
        for(auto widget : micaWidgetList)
        {
            updateMica(widget, false);
        }
    }
}
void QtApplicationPrivate::resetAllMicaWidget()
{
    for(auto widget : micaWidgetList)
    {
        QPalette palette = widget->palette();
        palette.setBrush(QPalette::Window, Qt::transparent);
        widget->setPalette(palette);
    }
}
Q_SINGLETON_CREATE_CPP(QtApplication)
QtApplication::QtApplication(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new QtApplicationPrivate(this))
{
}

QtApplication::~QtApplication()
{
}

QtApplicationType::WindowDisplayMode QtApplication::windowDisplayMode() const
{
    Q_D(const QtApplication);
    return d->windowDisplayMode;
}

void QtApplication::setWindowDisplayMode(QtApplicationType::WindowDisplayMode windowDisplayType)
{
    Q_D(QtApplication);
    auto lastDisplayMode = d->windowDisplayMode;
    if(lastDisplayMode == windowDisplayType)
    {
        return;
    }
    if(lastDisplayMode == QtApplicationType::QtMica)
    {
        d->resetAllMicaWidget();
    }
    switch(windowDisplayType)
    {
        case QtApplicationType::Normal: {
            break;
        }
        case QtApplicationType::QtMica: {
            d->windowDisplayMode = windowDisplayType;
            d->initMicaBaseImage(QImage(d->qtMicaImagePath));
            break;
        }
        default: {
            break;
        }
    }
#ifdef Q_OS_WIN
    for(auto widget : d->micaWidgetList)
    {
        QtWinShadowHelper::getInstance()->setWindowDisplayMode(
            widget, windowDisplayType, lastDisplayMode);
        QtWinShadowHelper::getInstance()->setWindowThemeMode(
            widget->winId(), d->themeMode == QtThemeType::Light);
    }
#endif
    if(windowDisplayType != QtApplicationType::QtMica)
    {
        d->windowDisplayMode = windowDisplayType;
        Q_EMIT windowDisplayModeChanged();
    }
}

QString QtApplication::qtMicaImagePath() const
{
    Q_D(const QtApplication);
    return d->qtMicaImagePath;
}

void QtApplication::setQtMicaImagePath(const QString& micaImagePath)
{
    Q_D(QtApplication);
    d->qtMicaImagePath = std::move(micaImagePath);
    d->initMicaBaseImage(QImage(d->qtMicaImagePath));
    Q_EMIT qtMicaImagePathChanged();
}

void QtApplication::init()
{
    Q_INIT_RESOURCE(qtwidgetsextra);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    // QFontDatabase::addApplicationFont(":/include/Font/ElaAwesome.ttf");
    QFontDatabase::addApplicationFont(":/include/QtAwesome.ttf");
    QFont font = qApp->font();
    font.setPixelSize(10);
    font.setFamily("Microsoft YaHei");
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
#ifdef Q_OS_WIN
    qtWinHelper->initWinAPI();
#endif
}
void QtApplication::syncWindowDisplayMode(QWidget* widget, bool isSync)
{
    Q_D(QtApplication);
    if(!widget)
    {
        return;
    }
    if(isSync)
    {
        d->micaWidgetList.append(widget);
        widget->installEventFilter(this);
    }
    else
    {
        d->micaWidgetList.removeOne(widget);
        widget->removeEventFilter(this);
    }
    switch(d->windowDisplayMode)
    {
        case QtApplicationType::Normal:
        case QtApplicationType::QtMica: {
            if(isSync)
            {
                if(d->windowDisplayMode == QtApplicationType::WindowDisplayMode::QtMica)
                {
                    d->updateMica(widget, false);
                }
            }
            break;
        }
        default: {
#ifdef Q_OS_WIN
            if(isSync)
            {
                QtWinShadowHelper::getInstance()->setWindowDisplayMode(
                    widget, d->windowDisplayMode, QtApplicationType::Normal);
                QtWinShadowHelper::getInstance()->setWindowThemeMode(
                    widget->winId(), d->themeMode == QtThemeType::Light);
            }
            else
            {
                QtWinShadowHelper::getInstance()->setWindowDisplayMode(
                    widget, QtApplicationType::Normal, d->windowDisplayMode);
                QtWinShadowHelper::getInstance()->setWindowThemeMode(widget->winId(), true);
            }
#endif
            break;
        }
    }
}
bool QtApplication::containsCursorToItem(QWidget* item)
{
    if(!item || !item->isVisible())
    {
        return false;
    }
    auto itemRect = QRect(item->mapToGlobal(QPoint(0, 0)), item->size());
    if(itemRect.contains(QCursor::pos()))
    {
        return true;
    }
    return false;
}

bool QtApplication::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtApplication);
    switch(event->type())
    {
        case QEvent::Show: {
            if(d->windowDisplayMode == QtApplicationType::WindowDisplayMode::QtMica)
            {
                QWidget* widget = qobject_cast<QWidget*>(watched);
                if(widget)
                {
                    d->updateMica(widget);
                }
            }
            else if(d->windowDisplayMode != QtApplicationType::WindowDisplayMode::Normal)
            {
#ifdef Q_OS_WIN
                QWidget* widget = qobject_cast<QWidget*>(watched);
                if(widget)
                {
                    QtWinShadowHelper::getInstance()->setWindowDisplayMode(
                        widget, d->windowDisplayMode, d->windowDisplayMode);
                }
#endif
            }
            break;
        }
        case QEvent::Move:
        case QEvent::Resize: {
            if(d->windowDisplayMode == QtApplicationType::WindowDisplayMode::QtMica)
            {
                QWidget* widget = qobject_cast<QWidget*>(watched);
                if(widget)
                {
                    d->updateMica(widget);
                }
            }
            break;
        }
        case QEvent::Destroy: {
            QWidget* widget = qobject_cast<QWidget*>(watched);
            if(widget)
            {
                d->micaWidgetList.removeOne(widget);
            }
            break;
        }
        default: {
            break;
        }
    }
    return QObject::eventFilter(watched, event);
}
