#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpushbutton.h>
class QtInteractiveCardPrivate;
class QTWIDGETSEXTRA_EXPORT QtInteractiveCard : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged)
    Q_PROPERTY(
        int titlePixelSize READ titlePixelSize WRITE setTitlePixelSize NOTIFY titlePixelSizeChanged)
    Q_PROPERTY(int subTitlePixelSize READ subTitlePixelSize WRITE setSubTitlePixelSize NOTIFY
            subTitlePixelSizeChanged)
    Q_PROPERTY(int titleSpacing READ titleSpacing WRITE setTitleSpacing NOTIFY titleSpacingChanged)
    Q_PROPERTY(QPixmap cardPixmap READ cardPixmap WRITE setCardPixmap NOTIFY cardPixmapChanged)
    Q_PROPERTY(QSize cardPixmapSize READ cardPixmapSize WRITE setCardPixmapSize NOTIFY
            cardPixmapSizeChanged)
    Q_PROPERTY(int cardPixmapBorderRadius READ cardPixmapBorderRadius WRITE
            setCardPixmapBorderRadius NOTIFY cardPixmapBorderRadiusChanged)
    Q_PROPERTY(QtCardPixType::PixMode cardPixMode READ cardPixMode WRITE setCardPixMode NOTIFY
            cardPixModeChanged)
public:
    explicit QtInteractiveCard(QWidget* parent = nullptr);
    ~QtInteractiveCard();

    void setCardPixmapSize(int width, int height);

    int borderRadius() const;
    void setBorderRadius(int value);

    QString title() const;
    void setTitle(const QString& value);

    QString subTitle() const;
    void setSubTitle(const QString& value);

    int titlePixelSize() const;
    void setTitlePixelSize(int value);

    int subTitlePixelSize() const;
    void setSubTitlePixelSize(int value);

    int titleSpacing() const;
    void setTitleSpacing(int value);

    QPixmap cardPixmap() const;
    void setCardPixmap(const QPixmap& value);

    QSize cardPixmapSize() const;
    void setCardPixmapSize(const QSize& value);

    int cardPixmapBorderRadius() const;
    void setCardPixmapBorderRadius(int value);

    QtCardPixType::PixMode cardPixMode() const;
    void setCardPixMode(QtCardPixType::PixMode value);

Q_SIGNALS:
    void borderRadiusChanged();
    void titleChanged();
    void subTitleChanged();
    void titlePixelSizeChanged();
    void subTitlePixelSizeChanged();
    void titleSpacingChanged();
    void subTitleSpacingChanged();
    void cardPixmapChanged();
    void cardPixmapSizeChanged();
    void cardPixmapBorderRadiusChanged();
    void cardPixModeChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtInteractiveCardPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtInteractiveCard)
};
