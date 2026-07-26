#pragma once

#include <QtWidgets>
#include "qtwidgetsextra.h"
#include "qtsingleton.h"

class QtExponentialBlurPrivate;
class QTWIDGETSEXTRA_EXPORT QtExponentialBlur : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE(QtExponentialBlur);

public:
    explicit QtExponentialBlur(QWidget* parent = nullptr);
    ~QtExponentialBlur();
    static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);
private:
    QScopedPointer<class QtExponentialBlurPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtExponentialBlur)
};
