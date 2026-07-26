#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtBreadcrumbBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtBreadcrumbBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        int textPixelSize READ textPixelSize WRITE setTextPixelSize NOTIFY textPixelSizeChanged)
    Q_PROPERTY(bool isAutoRemove READ isAutoRemove WRITE setIsAutoRemove NOTIFY isAutoRemoveChanged)
public:
    explicit QtBreadcrumbBar(QWidget* parent = nullptr);
    ~QtBreadcrumbBar();
    void setBreadcrumbList(QStringList breadcrumbList);
    QStringList appendBreadcrumb(QString breadcrumb);
    QStringList removeBreadcrumb(QString breadcrumb);

    int breadcrumbListCount() const;
    QStringList breadcrumbList() const;

    int textPixelSize() const;
    void setTextPixelSize(int value);

    bool isAutoRemove() const;
    void setIsAutoRemove(bool value);

Q_SIGNALS:
    void textPixelSizeChanged();
    void isAutoRemoveChanged();
    void breadcrumbClicked(QString breadcrumb, QStringList lastBreadcrumbList);

private:
    QScopedPointer<class QtBreadcrumbBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtBreadcrumbBar)
};
