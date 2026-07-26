#ifndef QTTOOLBUTTONSTYLE_H
#define QTTOOLBUTTONSTYLE_H
#include <QtWidgets>
#include "qtwidgetsextra.h"

class QtToolButtonStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtToolButtonStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(bool isTransparent READ isTransparent WRITE setIsTransparent NOTIFY
            isTransparentChanged FINAL)
    Q_PROPERTY(
        int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged FINAL)
    Q_PROPERTY(qreal expandIconRotate READ expandIconRotate WRITE setExpandIconRotate NOTIFY
            expandIconRotateChanged FINAL)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged FINAL)

public:
    explicit QtToolButtonStyle(QStyle* style = nullptr);
    ~QtToolButtonStyle();

    bool isTransparent() const;
    void setIsTransparent(bool newIsTransparent);

    int borderRadius() const;
    void setBorderRadius(int newBorderRadius);

    qreal expandIconRotate() const;
    void setExpandIconRotate(qreal newExpandIconRotate);

    bool isSelected() const;
    void setIsSelected(bool newIsSelected);

Q_SIGNALS:
    void isTransparentChanged();
    void borderRadiusChanged();
    void expandIconRotateChanged();
    void isSelectedChanged();

protected:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
        QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;

private:
    QScopedPointer<class QtToolButtonStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtToolButtonStyle)
};

#endif // QTTOOLBUTTONSTYLE_H
