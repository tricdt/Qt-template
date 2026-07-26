#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtSpotlightPrivate;
class QTWIDGETSEXTRA_EXPORT QtSpotlight : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int padding READ padding WRITE setPadding NOTIFY paddingChanged)
    Q_PROPERTY(int overlayAlpha READ overlayAlpha WRITE setOverlayAlpha NOTIFY overlayAlphaChanged)
    Q_PROPERTY(bool isCircle READ isCircle WRITE setIsCircle NOTIFY isCircleChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
public:
    explicit QtSpotlight(QWidget* parent = nullptr);
    ~QtSpotlight();

    int borderRadius() const;
    void setBorderRadius(int value);

    int padding() const;
    void setPadding(int value);

    int overlayAlpha() const;
    void setOverlayAlpha(int value);

    bool isCircle() const;
    void setIsCircle(bool value);

    const QString& title() const;
    void setTitle(const QString& value);

    const QString& content() const;
    void setContent(const QString& value);

Q_SIGNALS:
    void borderRadiusChanged();
    void paddingChanged();
    void overlayAlphaChanged();
    void isCircleChanged();
    void titleChanged();
    void contentChanged();

private:
    QScopedPointer<class QtSpotlightPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSpotlight)
};
