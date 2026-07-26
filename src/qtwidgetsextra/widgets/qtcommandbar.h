#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtCommandBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtCommandBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int buttonSize READ buttonSize WRITE setButtonSize NOTIFY buttonSizeChanged)
public:
    struct CommandItem
    {
        QtIconType::IconName icon = QtIconType::None;
        QString text;
        bool isSeparator = false;
    };
    explicit QtCommandBar(QWidget* parent = nullptr);
    ~QtCommandBar();

    void addItem(const CommandItem& item);
    void addSeparator();
    void clearItems();

    int buttonSize() const;
    void setButtonSize(int value);

Q_SIGNALS:
    Q_SIGNAL void itemClicked(int index);
    void buttonSizeChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtCommandBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCommandBar)
};
