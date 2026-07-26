#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qstackedwidget.h>

class QtCentralStackedWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtCentralStackedWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int popupAnimationYOffset READ popupAnimationYOffset WRITE setPopupAnimationYOffset
            NOTIFY popupAnimationYOffsetChanged)
    Q_PROPERTY(qreal scaleAnimationRatio READ scaleAnimationRatio WRITE setScaleAnimationRatio
            NOTIFY scaleAnimationRatioChanged)
    Q_PROPERTY(qreal scaleAnimationPixOpacity READ scaleAnimationPixOpacity WRITE
            setScaleAnimationPixOpacity NOTIFY scaleAnimationPixOpacityChanged)
    Q_PROPERTY(qreal flipAnimationRatio READ flipAnimationRatio WRITE setFlipAnimationRatio NOTIFY
            flipAnimationRatioChanged)
    Q_PROPERTY(int blurAnimationRadius READ blurAnimationRadius WRITE setBlurAnimationRadius NOTIFY
            blurAnimationRadiusChanged)
    Q_PROPERTY(int lastTargetIndex READ lastTargetIndex WRITE setLastTargetIndex NOTIFY
            lastTargetIndexChanged)
public:
    explicit QtCentralStackedWidget(QWidget* parent = nullptr);
    ~QtCentralStackedWidget();

    QStackedWidget* containerStackedWidget() const;

    void setCustomWidget(QWidget* widget);
    QWidget* customWidget() const;

    Q_SLOT void onThemeModeChanged(QtThemeType::ThemeMode themeMode);

    void setIsTransparent(bool isTransparent);
    bool isTransparent() const;

    void setIsHasRadius(bool isHasRadius);

    void doWindowStackSwitch(
        QtWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack);

    int popupAnimationYOffset() const;
    void setPopupAnimationYOffset(int value);

    qreal scaleAnimationRatio() const;
    void setScaleAnimationRatio(qreal value);

    qreal scaleAnimationPixOpacity() const;
    void setScaleAnimationPixOpacity(qreal value);

    qreal flipAnimationRatio() const;
    void setFlipAnimationRatio(qreal value);

    int blurAnimationRadius() const;
    void setBlurAnimationRadius(int value);

    int lastTargetIndex() const;
    void setLastTargetIndex(int value);

Q_SIGNALS:
    void popupAnimationYOffsetChanged(int value);
    void scaleAnimationRatioChanged(qreal value);
    void scaleAnimationPixOpacityChanged(qreal value);
    void flipAnimationRatioChanged(qreal value);
    void blurAnimationRadiusChanged(int value);
    void lastTargetIndexChanged(int value);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtCentralStackedWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCentralStackedWidget)
};
