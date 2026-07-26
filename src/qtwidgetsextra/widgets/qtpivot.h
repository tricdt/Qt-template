#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtPivotPrivate;
class QTWIDGETSEXTRA_EXPORT QtPivot : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        int textPixelSize READ textPixelSize WRITE setTextPixelSize NOTIFY textPixelSizeChanged);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(int pivotSpacing READ pivotSpacing WRITE setPivotSpacing NOTIFY pivotSpacingChanged);
    Q_PROPERTY(int markWidth READ markWidth WRITE setMarkWidth NOTIFY markWidthChanged);

public:
    explicit QtPivot(QWidget* parent = nullptr);
    ~QtPivot();
    int textPixelSize() const;
    void setTextPixelSize(int value);

    int currentIndex() const;
    void setCurrentIndex(int value);

    int pivotSpacing() const;
    void setPivotSpacing(int value);

    int markWidth() const;
    void setMarkWidth(int value);

    void appendPivot(QString pivotTitle);
    void removePivot(QString pivotTitle);

Q_SIGNALS:
    void textPixelSizeChanged();
    void currentIndexChanged();
    void pivotSpacingChanged();
    void markWidthChanged();

    void pivotClicked(int index);
    void pivotDoubleClicked(int index);

private:
    QScopedPointer<class QtPivotPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPivot)
};
