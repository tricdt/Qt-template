#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtRollerPrivate;
class QTWIDGETSEXTRA_EXPORT QtRoller : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QStringList itemList READ itemList WRITE setItemList NOTIFY itemListChanged)
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int maxVisibleItems READ maxVisibleItems WRITE setMaxVisibleItems NOTIFY
            maxVisibleItemsChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(bool isContainer READ isContainer WRITE setIsContainer NOTIFY isContainerChanged)
    Q_PROPERTY(bool isEnableLoop READ isEnableLoop WRITE setIsEnableLoop NOTIFY isEnableLoopChanged)
public:
    explicit QtRoller(QWidget* parent = nullptr);
    ~QtRoller();

    void setCurrentData(const QString& data);
    QString currentData() const;

    int borderRadius() const;
    void setBorderRadius(int value);

    QStringList itemList() const;
    void setItemList(QStringList value);

    int itemHeight() const;
    void setItemHeight(int value);

    int maxVisibleItems() const;
    void setMaxVisibleItems(int value);

    int currentIndex() const;
    void setCurrentIndex(int value);

    bool isContainer() const;
    void setIsContainer(bool value);

    bool isEnableLoop() const;
    void setIsEnableLoop(bool value);

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event) override;

Q_SIGNALS:
    void borderRadiusChanged();
    void itemListChanged();
    void itemHeightChanged();
    void maxVisibleItemsChanged();
    void currentIndexChanged();
    void isContainerChanged();
    void isEnableLoopChanged();

    Q_SIGNAL void currentDataChanged(const QString& data);

private:
    QScopedPointer<class QtRollerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRoller)
};
