#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtThemeAnimationWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtThemeAnimationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal endRadius READ endRadius WRITE setEndRadius NOTIFY endRadiusChanged)
    Q_PROPERTY(QPoint center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(QImage oldWindowBackground READ oldWindowBackground WRITE setOldWindowBackground
            NOTIFY oldWindowBackgroundChanged)
public:
    explicit QtThemeAnimationWidget(QWidget* parent = nullptr);
    ~QtThemeAnimationWidget();

    void startAnimation(int msec);

    qreal radius() const;
    void setRadius(qreal value);

    qreal endRadius() const;
    void setEndRadius(qreal value);

    QPoint center() const;
    void setCenter(QPoint value);

    QImage oldWindowBackground() const;
    void setOldWindowBackground(QImage value);

Q_SIGNALS:
    void radiusChanged(qreal value);
    void endRadiusChanged(qreal value);
    void centerChanged(QPoint value);
    void oldWindowBackgroundChanged(QImage value);
    void animationFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtThemeAnimationWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtThemeAnimationWidget)
};
