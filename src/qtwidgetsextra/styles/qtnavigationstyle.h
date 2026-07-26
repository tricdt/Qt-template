#pragma once

#include "itemviews/views/qtnavigationview.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qproxystyle.h>

class QtNavigationStylePrivate;
class QTWIDGETSEXTRA_EXPORT QtNavigationStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged);
    Q_PROPERTY(qreal rotate READ rotate WRITE setRotate NOTIFY rotateChanged);
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged);
    Q_PROPERTY(QtNavigationView* navigationView READ navigationView WRITE setNavigationView NOTIFY
            navigationViewChanged);
    Q_PROPERTY(qreal lastSelectMarkTop READ lastSelectMarkTop WRITE setLastSelectMarkTop NOTIFY
            lastSelectMarkTopChanged);
    Q_PROPERTY(qreal lastSelectMarkBottom READ lastSelectMarkBottom WRITE setLastSelectMarkBottom
            NOTIFY lastSelectMarkBottomChanged);
    Q_PROPERTY(
        qreal selectMarkTop READ selectMarkTop WRITE setSelectMarkTop NOTIFY selectMarkTopChanged);
    Q_PROPERTY(qreal selectMarkBottom READ selectMarkBottom WRITE setSelectMarkBottom NOTIFY
            selectMarkBottomChanged);
    Q_PROPERTY(QModelIndex pressIndex READ pressIndex WRITE setPressIndex NOTIFY pressIndexChanged);

public:
    explicit QtNavigationStyle(QStyle* style = nullptr);
    ~QtNavigationStyle();

    void navigationNodeStateChange(QVariantMap data);

    qreal opacity() const;
    void setOpacity(qreal value);

    qreal rotate() const;
    void setRotate(qreal value);

    int itemHeight() const;
    void setItemHeight(int value);

    QtNavigationView* navigationView() const;
    void setNavigationView(QtNavigationView* view);

    qreal lastSelectMarkTop() const;
    void setLastSelectMarkTop(qreal value);

    qreal lastSelectMarkBottom() const;
    void setLastSelectMarkBottom(qreal value);

    qreal selectMarkTop() const;
    void setSelectMarkTop(qreal value);

    qreal selectMarkBottom() const;
    void setSelectMarkBottom(qreal value);

    QModelIndex pressIndex() const;
    void setPressIndex(QModelIndex value);

Q_SIGNALS:
    void opacityChanged();
    void rotateChanged();
    void itemHeightChanged();
    void navigationViewChanged();
    void lastSelectMarkTopChanged();
    void lastSelectMarkBottomChanged();
    void selectMarkTopChanged();
    void selectMarkBottomChanged();
    void pressIndexChanged();
protected:
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget) const override;

private:
    QScopedPointer<class QtNavigationStylePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNavigationStyle)
};
