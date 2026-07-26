#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpushbutton.h>
#include <qtmetamacros.h>

class QtPushButtonPrivate;
class QTWIDGETSEXTRA_EXPORT QtPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setborderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QColor lightDefaultColor READ lightDefaultColor WRITE setLightDefaultColor NOTIFY
            lightDefaultColorChanged)
    Q_PROPERTY(QColor darkDefaultColor READ darkDefaultColor WRITE setdarkDefaultColor NOTIFY
            darkDefaultColorChanged)
    Q_PROPERTY(QColor lightHoverColor READ lightHoverColor WRITE setlightHoverColor NOTIFY
            lightHoverColorChanged)
    Q_PROPERTY(QColor darkHoverColor READ darkHoverColor WRITE setdarkHoverColor NOTIFY
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
    void setborderRadius(int value);

    QColor lightDefaultColor() const;
    void setLightDefaultColor(QColor value);

    QColor darkDefaultColor() const;
    void setdarkDefaultColor(QColor value);

    QColor lightHoverColor() const;
    void setlightHoverColor(QColor value);

    QColor darkHoverColor() const;
    void setdarkHoverColor(QColor value);

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
