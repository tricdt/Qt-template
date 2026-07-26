#pragma once

#include "qttheme.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtMessageDialogButtonPrivate;
class QtMessageDialogButton : public QWidget
{
    Q_OBJECT

public:
    enum ButtonType
    {
        Confirm,
        Cancel
    };
    explicit QtMessageDialogButton(ButtonType type, QWidget* parent = nullptr);
    ~QtMessageDialogButton();

Q_SIGNALS:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent* event) override;
#else
    void enterEvent(QEvent* event) override;
#endif
    void leaveEvent(QEvent* event) override;

private:
    QScopedPointer<class QtMessageDialogButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMessageDialogButton)
};

class QtMessageDialogPrivate;
class QTWIDGETSEXTRA_EXPORT QtMessageDialog : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString borderRadius READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
    Q_PROPERTY(
        int titlePixelSize READ titlePixelSize WRITE setTitlePixelSize NOTIFY titlePixelSizeChanged)
    Q_PROPERTY(int contentPixelSize READ contentPixelSize WRITE setContentPixelSize NOTIFY
            contentPixelSizeChanged)
public:
    explicit QtMessageDialog(QWidget* parent = nullptr);
    ~QtMessageDialog();

    int borderRadius() const;
    void setBorderRadius(int value);

    QString title() const;
    void setTitle(QString value);

    QString content() const;
    void setContent(QString value);

    int titlePixelSize() const;
    void setTitlePixelSize(int value);

    int contentPixelSize() const;
    void setContentPixelSize(int value);

Q_SIGNALS:
    void confirmed();
    void cancelled();

    void borderRadiusChanged();
    void titleChanged();
    void contentChanged();
    void titlePixelSizeChanged();
    void contentPixelSizeChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtMessageDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMessageDialog)
};
