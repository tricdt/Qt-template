#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtPlainTextEditStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtPlainTextEditStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(qreal expandMarkWidth READ expandMarkWidth WRITE setExpandMarkWidth NOTIFY
            expandMarkWidthChanged);

public:
    explicit QtPlainTextEditStyle(QStyle* style = nullptr);
    ~QtPlainTextEditStyle();

    qreal expandMarkWidth() const;
    void setExpandMarkWidth(qreal expandMarkWidth);

Q_SIGNALS:
    void expandMarkWidthChanged(qreal expandMarkWidth);

protected:
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;

private:
    QScopedPointer<class QtPlainTextEditStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPlainTextEditStyle)
};
