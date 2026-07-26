#pragma once

#include <QWidget>
#include "qtwidgetsextra.h"
class QtCaptchaPrivate;
class QTWIDGETSEXTRA_EXPORT QtCaptcha : public QWidget
{
    Q_OBJECT
Q_PROPERTY(int codeLength READ codeLength WRITE setCodeLength NOTIFY codeLengthChanged)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int boxSize READ boxSize WRITE setBoxSize NOTIFY boxSizeChanged)
    Q_PROPERTY(int boxSpacing READ boxSpacing WRITE setBoxSpacing NOTIFY boxSpacingChanged)
public:
    explicit QtCaptcha(QWidget* parent = nullptr);
    ~QtCaptcha();

    int codeLength() const;
    void setCodeLength(int value);

    int borderRadius() const;
    void setBorderRadius(int value);

    int boxSize() const;
    void setBoxSize(int value);

    int boxSpacing() const;
    void setBoxSpacing(int value);

Q_SIGNALS:
    void codeLengthChanged();
    void borderRadiusChanged();
    void boxSizeChanged();
    void boxSpacingChanged();

private:
    QScopedPointer<class QtCaptchaPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCaptcha)
};
