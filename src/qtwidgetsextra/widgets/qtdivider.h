#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtDividerPrivate;
class QTWIDGETSEXTRA_EXPORT QtDivider : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(
        Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int contentPosition READ contentPosition WRITE setContentPosition NOTIFY
            contentPositionChanged)
public:
    enum ContentPositionType
    {
        Left = 0,
        Center = 1,
        Right = 2
    };
    Q_ENUM(ContentPositionType)
    explicit QtDivider(QWidget* parent = nullptr);
    explicit QtDivider(const QString& text, QWidget* parent = nullptr);
    ~QtDivider();

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation value);

    const QString& text() const;
    void setText(const QString& value);

    int contentPosition() const;
    void setContentPosition(int value);

Q_SIGNALS:
    void orientationChanged();
    void textChanged();
    void contentPositionChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtDividerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtDivider)
};
