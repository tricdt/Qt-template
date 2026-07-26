#include "qtevent.h"
#include <qnamespace.h>
#include <qobject.h>

class QtEventPrivate
{
    Q_DECLARE_PUBLIC(QtEvent)
public:
    QtEvent* q_ptr;
    QString eventName;
    QString functionName;
    Qt::ConnectionType connectionType;
    QtEventPrivate(QtEvent* q);
};

QtEventPrivate::QtEventPrivate(QtEvent* q)
    : q_ptr(q)
    , eventName("")
    , functionName("")
    , connectionType(Qt::AutoConnection)
{
}

QtEvent::QtEvent(QObject* parent)
    : QObject{ parent }
    , d_ptr(new QtEventPrivate(this))
{
}

QtEvent::QtEvent(QString eventName, QString functionName, QObject* parent)
    : QtEvent(parent)
{
    Q_D(QtEvent);
    d->eventName = eventName;
    d->functionName = functionName;
    d->connectionType = Qt::AutoConnection;
}

QtEvent::~QtEvent()
{
    QtEventBus::getInstance()->unRegisterEvent(this);
}

Q_SINGLETON_CREATE_CPP(QtEventBus)
QString QtEvent::eventName() const
{
    Q_D(const QtEvent);
    return d->eventName;
}

void QtEvent::setEventName(const QString& value)
{
    Q_D(QtEvent);
    if(d->eventName == value)
        return;
    d->eventName = value;
    Q_EMIT eventNameChanged();
}

QString QtEvent::functionName() const
{
    Q_D(const QtEvent);
    return d->functionName;
}

void QtEvent::setFunctionName(const QString& value)
{
    Q_D(QtEvent);
    if(d->functionName == value)
        return;
    d->functionName = value;
    Q_EMIT functionNameChanged();
}

Qt::ConnectionType QtEvent::connectionType() const
{
    Q_D(const QtEvent);
    return d->connectionType;
}

void QtEvent::setConnectionType(Qt::ConnectionType value)
{
    Q_D(QtEvent);
    if(d->connectionType == value)
        return;
    d->connectionType = value;
    Q_EMIT connectionTypeChanged();
}

QtEventBusType::EventBusReturnType QtEvent::registerAndInit()
{
    return QtEventBus::getInstance()->registerEvent(this);
}

class QtEventBusPrivate
{
    Q_DECLARE_PUBLIC(QtEventBus)
public:
    QtEventBus* q_ptr;
    QMap<QString, QList<QtEvent*>> eventMap;
    QtEventBusPrivate(QtEventBus* q);
    QtEventBusType::EventBusReturnType registerEvent(QtEvent* event);
    void unRegisterEvent(QtEvent* event);
};

QtEventBusPrivate::QtEventBusPrivate(QtEventBus* q)
    : q_ptr(q)
{
}

QtEventBusType::EventBusReturnType QtEventBusPrivate::registerEvent(QtEvent* event)
{
    if(!event)
    {
        return QtEventBusType::EventBusReturnType::EventInvalid;
    }
    if(event->eventName().isEmpty())
    {
        return QtEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if(eventMap.contains(event->eventName()))
    {
        QList<QtEvent*> eventList = eventMap.value(event->eventName());
        if(eventList.contains(event))
        {
            return QtEventBusType::EventBusReturnType::EventInvalid;
        }
        eventList.append(event);
        eventMap[event->eventName()] = eventList;
    }
    else
    {
        QList<QtEvent*> eventList;
        eventList.append(event);
        eventMap.insert(event->eventName(), eventList);
    }
    return QtEventBusType::EventBusReturnType::Success;
}
void QtEventBusPrivate::unRegisterEvent(QtEvent* event)
{
    if(!event)
    {
        return;
    }
    if(event->eventName().isEmpty())
    {
        return;
    }
    if(eventMap.contains(event->eventName()))
    {
        if(eventMap[event->eventName()].count() == 1)
        {
            eventMap.remove(event->eventName());
        }
        else
        {
            QList<QtEvent*> eventList = eventMap.value(event->eventName());
            eventList.removeOne(event);
            eventMap[event->eventName()] = eventList;
        }
    }
}

QtEventBus::QtEventBus(QObject* parent)
    : QObject(parent)
    , d_ptr(new QtEventBusPrivate(this))
{
}

QtEventBus::~QtEventBus()
{
}

QtEventBusType::EventBusReturnType QtEventBus::registerEvent(QtEvent* event)
{
    Q_D(QtEventBus);
    return d->registerEvent(event);
}
void QtEventBus::unRegisterEvent(QtEvent* event)
{
    Q_D(QtEventBus);
    d->unRegisterEvent(event);
}

QtEventBusType::EventBusReturnType QtEventBus::post(
    const QString& eventName, const QVariantMap& data)
{
    Q_D(QtEventBus);
    if(eventName.isEmpty())
    {
        return QtEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if(d->eventMap.contains(eventName))
    {
        QList<QtEvent*> eventList = d->eventMap.value(eventName);
        for(auto event : eventList)
        {
            if(event->parent())
            {
                QMetaObject::invokeMethod(event->parent(),
                    event->functionName().toLocal8Bit().constData(), event->connectionType(),
                    Q_ARG(QVariantMap, data));
            }
        }
    }
    return QtEventBusType::EventBusReturnType::Success;
}
QStringList QtEventBus::getRegisteredEventsName() const
{
    Q_D(const QtEventBus);
    if(d->eventMap.count() == 0)
    {
        return QStringList();
    }
    QStringList eventsNameList = d->eventMap.keys();
    return eventsNameList;
}