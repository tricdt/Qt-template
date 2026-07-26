#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtTransferPrivate;
class QTWIDGETSEXTRA_EXPORT QtTransfer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(QString sourceTitle READ sourceTitle WRITE setSourceTitle NOTIFY sourceTitleChanged)
    Q_PROPERTY(QString targetTitle READ targetTitle WRITE setTargetTitle NOTIFY targetTitleChanged)
    Q_PROPERTY(bool isSearchVisible READ isSearchVisible WRITE setIsSearchVisible NOTIFY
            isSearchVisibleChanged)
public:
    explicit QtTransfer(QWidget* parent = nullptr);
    ~QtTransfer();

    int borderRadius() const;
    void setBorderRadius(int value);

    int itemHeight() const;
    void setItemHeight(int value);

    const QString& sourceTitle() const;
    void setSourceTitle(const QString& value);

    const QString& targetTitle() const;
    void setTargetTitle(const QString& value);

    bool isSearchVisible() const;
    void setIsSearchVisible(bool value);

Q_SIGNALS:
    void borderRadiusChanged();
    void itemHeightChanged();
    void sourceTitleChanged();
    void targetTitleChanged();
    void isSearchVisibleChanged();

private:
    QScopedPointer<class QtTransferPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTransfer)
};
