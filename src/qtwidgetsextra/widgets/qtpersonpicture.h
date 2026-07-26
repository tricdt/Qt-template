#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtPersonPicturePrivate;
class QTWIDGETSEXTRA_EXPORT QtPersonPicture : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int pictureSize READ pictureSize WRITE setPictureSize NOTIFY pictureSizeChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QPixmap picture READ picture WRITE setPicture NOTIFY pictureChanged)
public:
    explicit QtPersonPicture(QWidget* parent = nullptr);
    ~QtPersonPicture();

    int pictureSize() const;
    void setPictureSize(int value);

    QString displayName() const;
    void setDisplayName(QString value);

    const QPixmap& picture() const;
    void setPicture(const QPixmap& value);

    QSize sizeHint() const override;

Q_SIGNALS:
    void pictureSizeChanged();
    void displayNameChanged();
    void pictureChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtPersonPicturePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtPersonPicture)
};

// int
// QString
// QPixmap

// pictureSize
// displayName
// picture