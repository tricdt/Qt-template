#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtMaskWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtMaskWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int maskAlpha READ maskAlpha WRITE setMaskAlpha NOTIFY maskAlphaChanged)
public:
    explicit QtMaskWidget(QWidget* parent = nullptr);
    ~QtMaskWidget();
    void doMaskAnimation(int endValue);

    int maskAlpha() const;
    void setMaskAlpha(int value);

Q_SIGNALS:
    void maskAlphaChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtMaskWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMaskWidget)
};
