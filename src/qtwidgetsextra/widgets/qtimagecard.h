#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtImageCardPrivate;
class QTWIDGETSEXTRA_EXPORT QtImageCard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QImage cardImage READ cardImage WRITE setCardImage NOTIFY cardImageChanged)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(bool isPreserveAspectCrop READ isPreserveAspectCrop WRITE setIsPreserveAspectCrop
            NOTIFY isPreserveAspectCropChanged)
public:
    explicit QtImageCard(QWidget* parent = nullptr);
    ~QtImageCard();

    QImage cardImage() const;
    void setCardImage(const QImage& image);

    int borderRadius() const;
    void setBorderRadius(int radius);

    bool isPreserveAspectCrop() const;
    void setIsPreserveAspectCrop(bool isPreserve);

Q_SIGNALS:
    void cardImageChanged();
    void borderRadiusChanged();
    void isPreserveAspectCropChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtImageCardPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtImageCard)
};
