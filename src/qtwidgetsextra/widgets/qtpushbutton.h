#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpushbutton.h>
#include <qtmetamacros.h>

class QtPushButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QColor lightDefaultColor READ lightDefaultColor WRITE setLightDefaultColor NOTIFY
            lightDefaultColorChanged)
    Q_PROPERTY(QColor darkDefaultColor READ darkDefaultColor WRITE setDarkDefaultColor NOTIFY
            darkDefaultColorChanged)
    Q_PROPERTY(QColor lightHoverColor READ lightHoverColor WRITE setLightHoverColor NOTIFY
            lightHoverColorChanged)
    Q_PROPERTY(QColor darkHoverColor READ darkHoverColor WRITE setDarkHoverColor NOTIFY
            darkHoverColorChanged)
    Q_PROPERTY(QColor lightPressColor READ lightPressColor WRITE setLightPressColor NOTIFY
            lightPressColorChanged)
    Q_PROPERTY(QColor darkPressColor READ darkPressColor WRITE setDarkPressColor NOTIFY
            darkPressColorChanged)
public:
    explicit QtPushButton(QWidget* parent = nullptr);
    explicit QtPushButton(QString text, QWidget* parent = nullptr);
    ~QtPushButton();

    int borderRadius() const;
    void setBorderRadius(int value);

    QColor lightDefaultColor() const;
    void setLightDefaultColor(QColor value);

    QColor darkDefaultColor() const;
    void setDarkDefaultColor(QColor value);

    QColor lightHoverColor() const;
    void setLightHoverColor(QColor value);

    QColor darkHoverColor() const;
    void setDarkHoverColor(QColor value);

    QColor lightPressColor() const;
    void setLightPressColor(QColor value);

    QColor darkPressColor() const;
    void setDarkPressColor(QColor value);

    void setLightTextColor(QColor color);
    QColor lightTextColor() const;

    void setDarkTextColor(QColor color);
    QColor darkTextColor() const;

Q_SIGNALS:
    void borderRadiusChanged();
    void lightDefaultColorChanged();
    void darkDefaultColorChanged();
    void lightHoverColorChanged();
    void darkHoverColorChanged();
    void lightPressColorChanged();
    void darkPressColorChanged();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtPushButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPushButton)
};
