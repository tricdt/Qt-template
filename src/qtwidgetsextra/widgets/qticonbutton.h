#ifndef QTICONBUTTON_H
#define QTICONBUTTON_H

#include "qtdef.h"
#include <QtWidgets>
#include "qtwidgetsextra.h"

class QtIconButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtIconButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(
        int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged FINAL)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(QColor lightHoverColor READ lightHoverColor WRITE setLightHoverColor NOTIFY
            lightHoverColorChanged FINAL)
    Q_PROPERTY(QColor darkHoverColor READ darkHoverColor WRITE setDarkHoverColor NOTIFY
            darkHoverColorChanged FINAL)
    Q_PROPERTY(QColor lightIconColor READ lightIconColor WRITE setLightIconColor NOTIFY
            lightIconColorChanged FINAL)
    Q_PROPERTY(QColor darkIconColor READ darkIconColor WRITE setDarkIconColor NOTIFY
            darkIconColorChanged FINAL)
    Q_PROPERTY(QColor lightHoverIconColor READ lightHoverIconColor WRITE setLightHoverIconColor
            NOTIFY lightHoverIconColorChanged FINAL)
    Q_PROPERTY(QColor darkHoverIconColor READ darkHoverIconColor WRITE setDarkHoverIconColor NOTIFY
            darkHoverIconColorChanged FINAL)
    Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY isSelectedChanged FINAL)
public:
    explicit QtIconButton(QPixmap pix, QWidget* parent = nullptr);
    QtIconButton(QtIconType::IconName awesome, QWidget* parent = nullptr);
    QtIconButton(QtIconType::IconName awesome, int pixelSize, QWidget* parent = nullptr);
    QtIconButton(QtIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight,
        QWidget* parent = nullptr);
    ~QtIconButton();

    int borderRadius() const;
    void setBorderRadius(int newBorderRadius);

    qreal opacity() const;
    void setOpacity(qreal newOpacity);

    QColor lightHoverColor() const;
    void setLightHoverColor(const QColor& newLightHoverColor);

    QColor darkHoverColor() const;
    void setDarkHoverColor(const QColor& newDarkHoverColor);

    QColor lightIconColor() const;
    void setLightIconColor(const QColor& newLightIconColor);

    QColor darkIconColor() const;
    void setDarkIconColor(const QColor& newDarkIconColor);

    QColor lightHoverIconColor() const;
    void setLightHoverIconColor(const QColor& newLightHoverIconColor);

    QColor darkHoverIconColor() const;
    void setDarkHoverIconColor(const QColor& newDarkHoverIconColor);

    bool isSelected() const;
    void setIsSelected(bool newIsSelected);

    void setAwesome(QtIconType::IconName awesome);
    QtIconType::IconName awesome() const;

    void setPixmap(QPixmap pix);

Q_SIGNALS:
    void borderRadiusChanged();
    void opacityChanged();
    void lightHoverColorChanged();
    void darkHoverColorChanged();
    void lightIconColorChanged();
    void darkIconColorChanged();
    void lightHoverIconColorChanged();
    void darkHoverIconColorChanged();
    void isSelectedChanged();
protected:
    virtual bool event(QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
private:
    QScopedPointer<class QtIconButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtIconButton)
};
#endif // QTICONBUTTON_H
