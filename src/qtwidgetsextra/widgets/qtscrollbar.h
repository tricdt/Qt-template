#ifndef QTSCROLLBAR_H
#define QTSCROLLBAR_H

#include <QScrollBar>
#include "qtwidgetsextra.h"

class QAbstractScrollArea;
class QtScrollBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtScrollBar : public QScrollBar
{
    Q_OBJECT
    Q_PROPERTY(bool isAnimation READ isAnimation WRITE setIsAnimation NOTIFY isAnimationChanged)
    Q_PROPERTY(qreal speedLimit READ speedLimit WRITE setSpeedLimit NOTIFY speedLimitChanged)
public:
    explicit QtScrollBar(QWidget* parent = nullptr);
    explicit QtScrollBar(Qt::Orientation orientation, QWidget* parent = nullptr);
    explicit QtScrollBar(QScrollBar* originScrollBar, QAbstractScrollArea* parent = nullptr);
    ~QtScrollBar() override;

    bool isAnimation() const;
    void setIsAnimation(bool newIsAnimation);

    qreal speedLimit() const;
    void setSpeedLimit(qreal newSpeedLimit);

Q_SIGNALS:
    void isAnimationChanged();
    void speedLimitChanged();
    void rangeAnimationFinished();

protected:
    virtual bool event(QEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;

private:
    QScopedPointer<class QtScrollBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollBar)
};

#endif // QTSCROLLBAR_H
