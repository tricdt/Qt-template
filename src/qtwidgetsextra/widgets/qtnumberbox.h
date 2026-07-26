#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtNumberBoxPrivate;
class QTWIDGETSEXTRA_EXPORT QtNumberBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(double step READ step WRITE setStep NOTIFY stepChanged)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals NOTIFY decimalsChanged)
    Q_PROPERTY(bool isWrapping READ isWrapping WRITE setIsWrapping NOTIFY isWrappingChanged)
public:
    explicit QtNumberBox(QWidget* parent = nullptr);
    ~QtNumberBox();

    void stepUp();
    void stepDown();

    int borderRadius() const;
    void setBorderRadius(int value);

    double value() const;
    void setValue(double value);

    double minimum() const;
    void setMinimum(double value);

    double maximum() const;
    void setMaximum(double value);

    double step() const;
    void setStep(double value);

    int decimals() const;
    void setDecimals(int value);

    bool isWrapping() const;
    void setIsWrapping(bool value);

Q_SIGNALS:
    Q_SIGNAL void valueChanged(double value);
    void borderRadiusChanged();
    void valueChanged();
    void minimumChanged();
    void maximumChanged();
    void stepChanged();
    void decimalsChanged();
    void isWrappingChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QScopedPointer<class QtNumberBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtNumberBox)
};
