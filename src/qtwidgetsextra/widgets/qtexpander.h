#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtExpanderPrivate;
class QTWIDGETSEXTRA_EXPORT QtExpander : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged)
    Q_PROPERTY(QtIconType::IconName headerIcon READ headerIcon WRITE setHeaderIcon NOTIFY
            headerIconChanged)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration NOTIFY
            animationDurationChanged)
public:
    enum ExpandDirection
    {
        Down = 0,
        Up
    };
    Q_ENUM(ExpandDirection)

    explicit QtExpander(QWidget* parent = nullptr);
    explicit QtExpander(const QString& title, QWidget* parent = nullptr);
    ~QtExpander();

    void setExpandDirection(ExpandDirection direction);
    ExpandDirection expandDirection() const;

    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

    void setHeaderWidget(QWidget* widget);

    void setIsExpanded(bool expanded);
    bool isExpanded() const;

    int borderRadius() const;
    void setBorderRadius(int value);
    QString title() const;
    void setTitle(QString value);
    QString subTitle() const;
    void setSubTitle(QString value);
    QtIconType::IconName headerIcon() const;
    void setHeaderIcon(QtIconType::IconName value);
    int animationDuration() const;
    void setAnimationDuration(int value);

Q_SIGNALS:
    Q_SIGNAL void expandStateChanged(bool expanded);
    void borderRadiusChanged();
    void titleChanged();
    void subTitleChanged();
    void headerIconChanged();
    void animationDurationChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool event(QEvent* event) override;

private:
    QScopedPointer<class QtExpanderPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtExpander)
};
