#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtSheetPanelPrivate;
class QTWIDGETSEXTRA_EXPORT QtSheetPanel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QtSheetPanelType::Direction direction READ direction WRITE setDirection NOTIFY
            directionChanged)
    Q_PROPERTY(qreal peekRatio READ peekRatio WRITE setPeekRatio NOTIFY peekRatioChanged)
    Q_PROPERTY(qreal halfRatio READ halfRatio WRITE setHalfRatio NOTIFY halfRatioChanged)
    Q_PROPERTY(qreal fullRatio READ fullRatio WRITE setFullRatio NOTIFY fullRatioChanged)
    Q_PROPERTY(bool dragHandleVisible READ dragHandleVisible WRITE setDragHandleVisible NOTIFY
            dragHandleVisibleChanged)
    Q_PROPERTY(bool closeOnOverlayClick READ closeOnOverlayClick WRITE setCloseOnOverlayClick NOTIFY
            closeOnOverlayClickChanged)
    Q_PROPERTY(qreal overlayOpacity READ overlayOpacity WRITE setOverlayOpacity NOTIFY
            overlayOpacityChanged)
public:
    explicit QtSheetPanel(QWidget* parent = nullptr);
    ~QtSheetPanel();

    int borderRadius() const;
    void setBorderRadius(int value);

    QtSheetPanelType::Direction direction() const;
    void setDirection(QtSheetPanelType::Direction value);

    qreal peekRatio() const;
    void setPeekRatio(qreal value);

    qreal halfRatio() const;
    void setHalfRatio(qreal value);

    qreal fullRatio() const;
    void setFullRatio(qreal value);

    bool dragHandleVisible() const;
    void setDragHandleVisible(bool value);

    bool closeOnOverlayClick() const;
    void setCloseOnOverlayClick(bool value);

    qreal overlayOpacity() const;
    void setOverlayOpacity(qreal value);

Q_SIGNALS:
    void borderRadiusChanged();
    void directionChanged();
    void peekRatioChanged();
    void halfRatioChanged();
    void fullRatioChanged();
    void dragHandleVisibleChanged();
    void closeOnOverlayClickChanged();
    void overlayOpacityChanged();

private:
    QScopedPointer<class QtSheetPanelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSheetPanel)
};
