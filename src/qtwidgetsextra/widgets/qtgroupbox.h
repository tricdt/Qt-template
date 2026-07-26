#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qgroupbox.h>
#include <qtmetamacros.h>

class QtGroupBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtGroupBox : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtGroupBox(QWidget* parent = nullptr);
    explicit QtGroupBox(const QString& title, QWidget* parent = nullptr);
    ~QtGroupBox();

    int borderRadius() const;
    void setBorderRadius(int value);

Q_SIGNALS:
    void borderRadiusChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtGroupBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtGroupBox)
};
