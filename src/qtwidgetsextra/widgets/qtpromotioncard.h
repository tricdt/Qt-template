#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtPromotionCardPrivate;
class QTWIDGETSEXTRA_EXPORT QtPromotionCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QPixmap cardPixmap READ cardPixmap WRITE setCardPixmap NOTIFY cardPixmapChanged)
    Q_PROPERTY(QString cardTitle READ cardTitle WRITE setCardTitle NOTIFY cardTitleChanged)
    Q_PROPERTY(QString promotionTitle READ promotionTitle WRITE setPromotionTitle NOTIFY
            promotionTitleChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged)
    Q_PROPERTY(QColor cardTitleColor READ cardTitleColor WRITE setCardTitleColor NOTIFY
            cardTitleColorChanged)
    Q_PROPERTY(QColor promotionTitleColor READ promotionTitleColor WRITE setPromotionTitleColor
            NOTIFY promotionTitleColorChanged)
    Q_PROPERTY(QColor promotionTitleBaseColor READ promotionTitleBaseColor WRITE
            setPromotionTitleBaseColor NOTIFY promotionTitleBaseColorChanged)
    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor NOTIFY titleColorChanged)
    Q_PROPERTY(
        QColor subTitleColor READ subTitleColor WRITE setSubTitleColor NOTIFY subTitleColorChanged)
    Q_PROPERTY(int cardTitlePixelSize READ cardTitlePixelSize WRITE setCardTitlePixelSize NOTIFY
            cardTitlePixelSizeChanged)
    Q_PROPERTY(int promotionTitlePixelSize READ promotionTitlePixelSize WRITE
            setPromotionTitlePixelSize NOTIFY promotionTitlePixelSizeChanged)
    Q_PROPERTY(
        int titlePixelSize READ titlePixelSize WRITE setTitlePixelSize NOTIFY titlePixelSizeChanged)
    Q_PROPERTY(int subTitlePixelSize READ subTitlePixelSize WRITE setSubTitlePixelSize NOTIFY
            subTitlePixelSizeChanged)
    Q_PROPERTY(qreal horizontalCardPixmapRatio READ horizontalCardPixmapRatio WRITE
            setHorizontalCardPixmapRatio NOTIFY horizontalCardPixmapRatioChanged)
    Q_PROPERTY(qreal verticalCardPixmapRatio READ verticalCardPixmapRatio WRITE
            setVerticalCardPixmapRatio NOTIFY verticalCardPixmapRatioChanged)
public:
    explicit QtPromotionCard(QWidget* parent = nullptr);
    ~QtPromotionCard();

    int borderRadius() const;
    void setBorderRadius(int value);

    QPixmap cardPixmap() const;
    void setCardPixmap(QPixmap value);

    QString cardTitle() const;
    void setCardTitle(QString value);

    QString promotionTitle() const;
    void setPromotionTitle(QString value);

    QString title() const;
    void setTitle(QString value);

    QString subTitle() const;
    void setSubTitle(QString value);

    QColor cardTitleColor() const;
    void setCardTitleColor(QColor value);

    QColor promotionTitleColor() const;
    void setPromotionTitleColor(QColor value);

    QColor promotionTitleBaseColor() const;
    void setPromotionTitleBaseColor(QColor value);

    QColor titleColor() const;
    void setTitleColor(QColor value);

    QColor subTitleColor() const;
    void setSubTitleColor(QColor value);

    int cardTitlePixelSize() const;
    void setCardTitlePixelSize(int value);

    int promotionTitlePixelSize() const;
    void setPromotionTitlePixelSize(int value);

    int titlePixelSize() const;
    void setTitlePixelSize(int value);

    int subTitlePixelSize() const;
    void setSubTitlePixelSize(int value);

    qreal horizontalCardPixmapRatio() const;
    void setHorizontalCardPixmapRatio(qreal value);

    qreal verticalCardPixmapRatio() const;
    void setVerticalCardPixmapRatio(qreal value);

Q_SIGNALS:
    void borderRadiusChanged();
    void cardPixmapChanged();
    void cardTitleChanged();
    void promotionTitleChanged();
    void titleChanged();
    void subTitleChanged();
    void cardTitleColorChanged();
    void promotionTitleColorChanged();
    void promotionTitleBaseColorChanged();
    void titleColorChanged();
    void subTitleColorChanged();
    void cardTitlePixelSizeChanged();
    void promotionTitlePixelSizeChanged();
    void titlePixelSizeChanged();
    void subTitlePixelSizeChanged();
    void horizontalCardPixmapRatioChanged();
    void verticalCardPixmapRatioChanged();

    void promotionCardClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool event(QEvent* event) override;
private:
    QScopedPointer<class QtPromotionCardPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPromotionCard)
};
