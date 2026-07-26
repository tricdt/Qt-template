#ifndef QTWIDGET_H
#define QTWIDGET_H

#include "qtappbar.h"
#include "qtwidgetsextra.h"
#include <QtWidgets>


class QtWidgetPrivate;
class QTWIDGETSEXTRA_EXPORT QtWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool isStayTop READ isStayTop WRITE setIsStayTop NOTIFY isStayTopChanged FINAL)
    Q_PROPERTY(
        bool isFixedSize READ isFixedSize WRITE setIsFixedSize NOTIFY isFixedSizeChanged FINAL)
    Q_PROPERTY(bool isDefaultClosed READ isDefaultClosed WRITE setIsDefaultClosed NOTIFY
            isDefaultClosedChanged FINAL)
    Q_PROPERTY(
        int appBarHeight READ appBarHeight WRITE setAppBarHeight NOTIFY appBarHeightChanged FINAL)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit QtWidget(QWidget* parent = nullptr);
    ~QtWidget();

    bool isStayTop() const;
    void setIsStayTop(bool newIsStayTop);

    bool isFixedSize() const;
    void setIsFixedSize(bool newIsFixedSize);

    bool isDefaultClosed() const;
    void setIsDefaultClosed(bool newIsDefaultClosed);

    int appBarHeight() const;
    void setAppBarHeight(int newAppBarHeight);

    void moveToCenter();
    void setWindowButtonFlag(QtAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(QtAppBarType::ButtonFlags buttonFlags);
    QtAppBarType::ButtonFlags windowButtonFlags() const;

Q_SIGNALS:
    void isStayTopChanged();
    void isFixedSizeChanged();
    void isDefaultClosedChanged();
    void appBarHeightChanged();
    void routeBackButtonClicked();
    void navigationButtonClicked();
    void themeChangeButtonClicked();
    void closeButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
#ifdef Q_OS_MACOS
    void resizeEvent(QResizeEvent* event) override;
#endif
private:
    QScopedPointer<class QtWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWidget)
};

#endif // QTWIDGET_H
