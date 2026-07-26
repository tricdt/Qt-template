#pragma once

#include "qproxystyle.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtComboBoxStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtComboBoxStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(qreal expandIconRotate READ expandIconRotate WRITE setExpandIconRotate NOTIFY
            expandIconRotateChanged)
    Q_PROPERTY(qreal expandMarkWidth READ expandMarkWidth WRITE setExpandMarkWidth NOTIFY
            expandMarkWidthChanged)
public:
    explicit QtComboBoxStyle(QStyle* parent = nullptr);
    ~QtComboBoxStyle();

    qreal expandIconRotate() const;
    void setExpandIconRotate(qreal value);

    qreal expandMarkWidth() const;
    void setExpandMarkWidth(qreal value);

Q_SIGNALS:
    void expandIconRotateChanged(qreal value);
    void expandMarkWidthChanged(qreal value);

protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc,
        const QWidget* widget) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;

private:
    QScopedPointer<class QtComboBoxStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtComboBoxStyle)
};
