#pragma once

#include <QObject>
#include <QVariantMap>
#include "qtdef.h"
#include "qtsingleton.h"
#include "qtwidgetsextra.h"

class QtEventPrivate;
class QTWIDGETSEXTRA_EXPORT QtEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString eventName READ eventName WRITE setEventName NOTIFY eventNameChanged)
    Q_PROPERTY(
        QString functionName READ functionName WRITE setFunctionName NOTIFY functionNameChanged)
    Q_PROPERTY(Qt::ConnectionType connectionType READ connectionType WRITE setConnectionType NOTIFY
            connectionTypeChanged)
public:
    explicit QtEvent(QObject* parent = nullptr);
    explicit QtEvent(QString eventName, QString functionName, QObject* parent = nullptr);
    ~QtEvent();

    QString eventName() const;
    void setEventName(const QString& newEventName);

    QString functionName() const;
    void setFunctionName(const QString& newFunctionName);

    Qt::ConnectionType connectionType() const;
    void setConnectionType(Qt::ConnectionType newConnectionType);

    QtEventBusType::EventBusReturnType registerAndInit();

Q_SIGNALS:
    void eventNameChanged();
    void functionNameChanged();
    void connectionTypeChanged();

private:
    QScopedPointer<class QtEventPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtEvent)
};

class QtEventBusPrivate;
class QTWIDGETSEXTRA_EXPORT QtEventBus : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(QtEventBus);

private:
    explicit QtEventBus(QObject* parent = nullptr);
    ~QtEventBus();
    QtEventBusType::EventBusReturnType registerEvent(QtEvent* event);
    void unRegisterEvent(QtEvent* event);

public:
    QtEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
    QStringList getRegisteredEventsName() const;

private:
    QScopedPointer<class QtEventBusPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtEventBus)
    friend class QtEvent;
};
