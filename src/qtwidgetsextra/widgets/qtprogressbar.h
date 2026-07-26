#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qprogressbar.h>

class QtProgressBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtProgressBar : public QProgressBar
{
    Q_OBJECT

public:
    explicit QtProgressBar(QWidget* parent = nullptr);
    ~QtProgressBar();

    void setMinimum(int minimum);
    void setMaximum(int maximum);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QScopedPointer<class QtProgressBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtProgressBar)
};
