#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtUploadAreaPrivate;
class QTWIDGETSEXTRA_EXPORT QtUploadArea : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged)
    Q_PROPERTY(QStringList acceptedSuffixes READ acceptedSuffixes WRITE setAcceptedSuffixes NOTIFY
            acceptedSuffixesChanged)
    Q_PROPERTY(int maxFileCount READ maxFileCount WRITE setMaxFileCount NOTIFY maxFileCountChanged)
    Q_PROPERTY(qint64 maxFileSize READ maxFileSize WRITE setMaxFileSize NOTIFY maxFileSizeChanged)
    Q_PROPERTY(bool isMultiple READ isMultiple WRITE setIsMultiple NOTIFY isMultipleChanged)
    Q_PROPERTY(QString dialogTitle READ dialogTitle WRITE setDialogTitle NOTIFY dialogTitleChanged)
public:
    explicit QtUploadArea(QWidget* parent = nullptr);
    ~QtUploadArea();

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& title() const;
    void setTitle(const QString& value);

    const QString& subTitle() const;
    void setSubTitle(const QString& value);

    const QStringList& acceptedSuffixes() const;
    void setAcceptedSuffixes(const QStringList& value);

    int maxFileCount() const;
    void setMaxFileCount(int value);

    qint64 maxFileSize() const;
    void setMaxFileSize(qint64 value);

    bool isMultiple() const;
    void setIsMultiple(bool value);

    const QString& dialogTitle() const;
    void setDialogTitle(const QString& value);

Q_SIGNALS:
    void borderRadiusChanged();
    void titleChanged();
    void subTitleChanged();
    void acceptedSuffixesChanged();
    void maxFileCountChanged();
    void maxFileSizeChanged();
    void isMultipleChanged();
    void dialogTitleChanged();

private:
    QScopedPointer<class QtUploadAreaPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtUploadArea)
};
