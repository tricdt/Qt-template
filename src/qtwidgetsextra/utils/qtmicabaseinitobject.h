#ifndef QTMICABASEINITOBJECT_H
#define QTMICABASEINITOBJECT_H
#include <QObject>
#include <QtWidgets>
#include "qtwidgetsextra.h"

class QtApplicationPrivate;
class QtMicaBaseInitObjectPrivate;
class QTWIDGETSEXTRA_EXPORT QtMicaBaseInitObject : public QObject
{
    Q_OBJECT
public:
    explicit QtMicaBaseInitObject(QtApplicationPrivate* appPrivate, QObject* parent = nullptr);
    ~QtMicaBaseInitObject();
public Q_SLOTS:
    void onInitMicaBase(QImage img);
Q_SIGNALS:
    void initFinished();

private:
    QScopedPointer<class QtMicaBaseInitObjectPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMicaBaseInitObject)
};

#endif // QTMICABASEINITOBJECT_H
