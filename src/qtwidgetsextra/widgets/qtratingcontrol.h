#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtRatingControlPrivate;
class QTWIDGETSEXTRA_EXPORT QtRatingControl : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int maxRating READ maxRating WRITE setMaxRating NOTIFY maxRatingChanged)
    Q_PROPERTY(int rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int starSize READ starSize WRITE setStarSize NOTIFY starSizeChanged)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(bool isReadOnly READ isReadOnly WRITE setIsReadOnly NOTIFY isReadOnlyChanged)
public:
    explicit QtRatingControl(QWidget* parent = nullptr);
    ~QtRatingControl();

    int maxRating() const;
    void setMaxRating(int value);

    int rating() const;
    void setRating(int value);

    int starSize() const;
    void setStarSize(int value);

    int spacing() const;
    void setSpacing(int value);

    bool isReadOnly() const;
    void setIsReadOnly(bool value);

Q_SIGNALS:
    Q_SIGNAL void ratingChanged(int rating);
    void maxRatingChanged();
    void ratingChanged();
    void starSizeChanged();
    void spacingChanged();
    void isReadOnlyChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtRatingControlPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtRatingControl)
};
