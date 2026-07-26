#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtInfoBadgePrivate;
class QTWIDGETSEXTRA_EXPORT QtInfoBadge : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QtIconType::IconName qtIcon READ qtIcon WRITE setQtIcon NOTIFY qtIconChanged)
public:
    enum BadgeMode
    {
        Dot = 0,
        Value_,
        Icon
    };
    Q_ENUM(BadgeMode)

    enum Severity
    {
        Attention = 0,
        Informational,
        Success,
        Caution,
        Critical
    };
    Q_ENUM(Severity)

    explicit QtInfoBadge(QWidget* parent = nullptr);
    explicit QtInfoBadge(int value, QWidget* parent = nullptr);
    explicit QtInfoBadge(QtIconType::IconName icon, QWidget* parent = nullptr);
    ~QtInfoBadge();

    int value() const;
    void setValue(int value);

    QtIconType::IconName qtIcon() const;
    void setQtIcon(QtIconType::IconName value);

    void setBadgeMode(BadgeMode mode);
    BadgeMode badgeMode() const;

    void setMaxValue(int maxValue);
    int maxValue() const;

    void setSeverity(Severity severity);
    Severity severity() const;

    void attachTo(QWidget* target);

Q_SIGNALS:
    void valueChanged();
    void qtIconChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtInfoBadgePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtInfoBadge)
};
