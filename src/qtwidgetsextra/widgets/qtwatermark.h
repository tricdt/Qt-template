#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
class QtWaterMarkPrivate;
class QTWIDGETSEXTRA_EXPORT QtWaterMark : public QWidget
{
    Q_OBJECT
Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int fontPixelSize READ fontPixelSize WRITE setFontPixelSize NOTIFY fontPixelSizeChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(int gapX READ gapX WRITE setGapX NOTIFY gapXChanged)
    Q_PROPERTY(int gapY READ gapY WRITE setGapY NOTIFY gapYChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(int imageWidth READ imageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(int imageHeight READ imageHeight WRITE setImageHeight NOTIFY imageHeightChanged)
public:
    explicit QtWaterMark(QWidget* parent = nullptr);
    ~QtWaterMark();

    const QString& text() const;
    void setText(const QString &value);

    int fontPixelSize() const;
    void setFontPixelSize(int value);

    qreal opacity() const;
    void setOpacity(qreal value);

    qreal rotation() const;
    void setRotation(qreal value);

    int gapX() const;
    void setGapX(int value);

    int gapY() const;
    void setGapY(int value);

    const QColor& textColor() const;
    void setTextColor(const QColor &value);

    const QImage& image() const;
    void setImage(const QImage &value);

    int imageWidth() const;
    void setImageWidth(int value);

    int imageHeight() const;
    void setImageHeight(int value);

Q_SIGNALS:
    void textChanged();
    void fontPixelSizeChanged();
    void opacityChanged();
    void rotationChanged();
    void gapXChanged();
    void gapYChanged();
    void textColorChanged();
    void imageChanged();
    void imageWidthChanged();
    void imageHeightChanged();

private:
    QScopedPointer<class QtWaterMarkPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWaterMark)
};
