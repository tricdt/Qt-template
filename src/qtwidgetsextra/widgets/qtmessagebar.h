#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtMessageBarPrivate;
class QTWIDGETSEXTRA_EXPORT QtMessageBar : public QWidget
{
    Q_OBJECT

public:
    static void success(QtMessageBarType::PositionPolicy policy, QString title, QString text,
        int displayMsec, QWidget* parent = nullptr);
    static void warning(QtMessageBarType::PositionPolicy policy, QString title, QString text,
        int displayMsec, QWidget* parent = nullptr);
    static void information(QtMessageBarType::PositionPolicy policy, QString title, QString text,
        int displayMsec, QWidget* parent = nullptr);
    static void error(QtMessageBarType::PositionPolicy policy, QString title, QString text,
        int displayMsec, QWidget* parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    explicit QtMessageBar(QtMessageBarType::PositionPolicy policy,
        QtMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec,
        QWidget* parent = nullptr);
    ~QtMessageBar();

private:
    QScopedPointer<class QtMessageBarPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMessageBar)
};
