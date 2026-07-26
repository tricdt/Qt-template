#pragma once

#include "qtwidgetsextra.h"
#include "styles/qtpivotstyle.h"
#include <QWidget>
#include <qlistview.h>
#include <qtmetamacros.h>

class QtPivotViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtPivotView : public QListView
{
    Q_OBJECT
    Q_PROPERTY(int markX READ markX WRITE setMarkX NOTIFY markXChanged);
    Q_PROPERTY(int markAnimationWidth READ markAnimationWidth WRITE setMarkAnimationWidth NOTIFY
            markAnimationWidthChanged);

public:
    explicit QtPivotView(QWidget* parent = nullptr);
    ~QtPivotView();

    int markX() const;
    void setMarkX(int value);

    int markWidth() const;
    void setMarkWidth(int value);

    int markAnimationWidth() const;
    void setMarkAnimationWidth(int value);

    QtPivotStyle* pivotStyle() const;
    void setPivotStyle(QtPivotStyle* value);

    QModelIndex currentIndex() const;
    void setCurrentIndex(QModelIndex value);

    QRect currentIndexRect() const;
    void setCurrentIndexRect(QRect value);

    bool isAnimationFinished() const;
    void setIsAnimationFinished(bool value);

    void doCurrentIndexChangedAnimation(const QModelIndex& index);

Q_SIGNALS:
    void markXChanged();
    void markAnimationWidthChanged();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtPivotViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPivotView)
};
