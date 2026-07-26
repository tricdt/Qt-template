#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtKeyBinderContainerPrivate;
class QTWIDGETSEXTRA_EXPORT QtKeyBinderContainer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        QString binderKeyText READ binderKeyText WRITE setBinderKeyText NOTIFY binderKeyTextChanged)
    Q_PROPERTY(quint32 nativeVirtualBinderKey READ nativeVirtualBinderKey WRITE
            setNativeVirtualBinderKey NOTIFY nativeVirtualBinderKeyChanged)
public:
    explicit QtKeyBinderContainer(QWidget* parent = nullptr);
    ~QtKeyBinderContainer();

    void logOrResetHistoryData(bool isLog);
    void saveBinderChanged();

    QString binderKeyText() const;
    void setBinderKeyText(QString value);

    quint32 nativeVirtualBinderKey() const;
    void setNativeVirtualBinderKey(quint32 value);

Q_SIGNALS:
    void binderKeyTextChanged(QString binderKeyText);
    void nativeVirtualBinderKeyChanged(quint32 binderKey);

protected:
    bool event(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtKeyBinderContainerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtKeyBinderContainer)
};
