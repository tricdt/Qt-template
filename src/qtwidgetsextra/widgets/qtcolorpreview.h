#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtColorPreviewPrivate;
class QTWIDGETSEXTRA_EXPORT QtColorPreview : public QWidget
{
    Q_OBJECT

public:
    explicit QtColorPreview(QWidget* parent = nullptr);
    ~QtColorPreview();

    QColor baseColor() const;
    void setBaseColor(QColor value);
    

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtColorPreviewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorPreview)
};
