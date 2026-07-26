#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qlabel.h>

class QtKeyBinderPrivate;
class QTWIDGETSEXTRA_EXPORT QtKeyBinder : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(
        QString binderKeyText READ binderKeyText WRITE setBinderKeyText NOTIFY binderKeyTextChanged)
    Q_PROPERTY(quint32 nativeVirtualBinderKey READ nativeVirtualBinderKey WRITE
            setNativeVirtualBinderKey NOTIFY nativeVirtualBinderKeyChanged)

public:
    explicit QtKeyBinder(QWidget* parent = nullptr);
    ~QtKeyBinder();

    int borderRadius() const;
    void setBorderRadius(int value);

    QString binderKeyText() const;
    void setBinderKeyText(QString value);

    quint32 nativeVirtualBinderKey() const;
    void setNativeVirtualBinderKey(quint32 value);

Q_SIGNALS:
    void borderRadiusChanged();
    void binderKeyTextChanged(QString binderKeyText);
    void nativeVirtualBinderKeyChanged(quint32 binderKey);

protected:
    virtual bool event(QEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtKeyBinderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtKeyBinder)
};
