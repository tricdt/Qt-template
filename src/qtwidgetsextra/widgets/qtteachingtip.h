#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qpixmap.h>
#include <qtmetamacros.h>

class QtTeachingTipPrivate;
class QTWIDGETSEXTRA_EXPORT QtTeachingTip : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(QtIconType::IconName tipIcon READ tipIcon WRITE setTipIcon NOTIFY tipIconChanged)
    Q_PROPERTY(QPixmap heroImage READ heroImage WRITE setHeroImage NOTIFY heroImageChanged)
    Q_PROPERTY(bool isLightDismiss READ isLightDismiss WRITE setIsLightDismiss NOTIFY
            isLightDismissChanged)
public:
    enum TailPosition
    {
        Auto = 0,
        Top,
        Bottom,
        Left,
        Right
    };
    Q_ENUM(TailPosition)
    explicit QtTeachingTip(QWidget* parent = nullptr);
    ~QtTeachingTip();

    void setTailPosition(TailPosition position);
    TailPosition tailPosition() const;

    void setTarget(QWidget* target);
    QWidget* target() const;

    void setCloseButtonVisible(bool visible);

    void addAction(const QString& text, const std::function<void()>& callback);
    void clearActions();

    void showTip();
    void closeTip();

    int borderRadius() const;
    void setBorderRadius(int value);

    QString title() const;
    void setTitle(QString value);

    QString subTitle() const;
    void setSubTitle(QString value);

    QString content() const;
    void setContent(QString value);

    QtIconType::IconName tipIcon() const;
    void setTipIcon(QtIconType::IconName value);

    QPixmap heroImage() const;
    void setHeroImage(QPixmap value);

    bool isLightDismiss() const;
    void setIsLightDismiss(bool value);

Q_SIGNALS:

    Q_SIGNAL void closed();
    Q_SIGNAL void closeButtonClicked();

    void borderRadiusChanged();
    void titleChanged();
    void subTitleChanged();
    void contentChanged();
    void tipIconChanged();
    void heroImageChanged();
    void isLightDismissChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    QScopedPointer<class QtTeachingTipPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTeachingTip)
};
