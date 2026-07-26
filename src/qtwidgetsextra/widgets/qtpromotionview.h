#pragma once

#include "qtpromotioncard.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtPromotionViewPrivate;
class QTWIDGETSEXTRA_EXPORT QtPromotionView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int cardExpandWidth READ cardExpandWidth WRITE setCardExpandWidth NOTIFY
            cardExpandWidthChanged)
    Q_PROPERTY(int cardCollapseWidth READ cardCollapseWidth WRITE setCardCollapseWidth NOTIFY
            cardCollapseWidthChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(bool isAutoScroll READ isAutoScroll WRITE setIsAutoScroll NOTIFY isAutoScrollChanged)
    Q_PROPERTY(int autoScrollInterval READ autoScrollInterval WRITE setAutoScrollInterval NOTIFY
            autoScrollIntervalChanged)
public:
    explicit QtPromotionView(QWidget* parent = nullptr);
    ~QtPromotionView();

    void appendPromotionCard(QtPromotionCard* card);

    int cardExpandWidth() const;
    void setCardExpandWidth(int value);

    int cardCollapseWidth() const;
    void setCardCollapseWidth(int value);

    int currentIndex() const;
    void setCurrentIndex(int value);

    bool isAutoScroll() const;
    void setIsAutoScroll(bool value);

    int autoScrollInterval() const;
    void setAutoScrollInterval(int value);

Q_SIGNALS:
    void cardExpandWidthChanged();
    void cardCollapseWidthChanged();
    void currentIndexChanged();
    void isAutoScrollChanged();
    void autoScrollIntervalChanged();

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtPromotionViewPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPromotionView)
};
