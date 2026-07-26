#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtScrollPageAreaPrivate;
class QTWIDGETSEXTRA_EXPORT QtScrollPageArea : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    int borderRadius() const;
    void setBorderRadius(int value);

    explicit QtScrollPageArea(QWidget* parent = nullptr);
    ~QtScrollPageArea();

Q_SIGNALS:
    void borderRadiusChanged(int value);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtScrollPageAreaPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtScrollPageArea)
};
