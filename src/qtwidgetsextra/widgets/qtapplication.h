#pragma once

#include "qtdef.h"
#include "qtsingleton.h"
#include <QtWidgets>
#include "qtwidgetsextra.h"
#define qtApp QtApplication::getInstance()
class QtApplicationPrivate;
class QTWIDGETSEXTRA_EXPORT QtApplication : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QtApplicationType::WindowDisplayMode windowDisplayMode READ windowDisplayMode WRITE
            setWindowDisplayMode NOTIFY windowDisplayModeChanged FINAL)
    Q_PROPERTY(QString qtMicaImagePath READ qtMicaImagePath WRITE setQtMicaImagePath NOTIFY
            qtMicaImagePathChanged FINAL)
    Q_SINGLETON_CREATE_H(QtApplication)
public:
    explicit QtApplication(QWidget* parent = nullptr);
    ~QtApplication();

    QtApplicationType::WindowDisplayMode windowDisplayMode() const;
    void setWindowDisplayMode(QtApplicationType::WindowDisplayMode newWindowDisplayMode);

    QString qtMicaImagePath() const;
    void setQtMicaImagePath(const QString& newQtMicaImagePath);

    void init();
    void syncWindowDisplayMode(QWidget* widget, bool isSync = true);
    static bool containsCursorToItem(QWidget* item);

Q_SIGNALS:
    void windowDisplayModeChanged();
    void qtMicaImagePathChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QScopedPointer<class QtApplicationPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtApplication)
};
