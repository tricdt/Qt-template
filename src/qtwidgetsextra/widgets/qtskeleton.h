#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtSkeletonPrivate;
class QTWIDGETSEXTRA_EXPORT QtSkeleton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(bool isAnimated READ isAnimated WRITE setIsAnimated NOTIFY isAnimatedChanged)
public:
    enum SkeletonType
    {
        Text = 0,
        Circle,
        Rectangle
    };
    Q_ENUM(SkeletonType)
    explicit QtSkeleton(QWidget* parent = nullptr);
    ~QtSkeleton();
    
    void setSkeletonType(SkeletonType type);
    SkeletonType skeletonType() const;

    int borderRadius() const;
    void setBorderRadius(int value);

    bool isAnimated() const;
    void setIsAnimated(bool value);

Q_SIGNALS:
    void borderRadiusChanged();
    void isAnimatedChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtSkeletonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSkeleton)
};
