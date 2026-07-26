#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtCountDownPrivate;
class QTWIDGETSEXTRA_EXPORT QtCountDown : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(bool isShowDays READ isShowDays WRITE setIsShowDays NOTIFY isShowDaysChanged)
    Q_PROPERTY(bool isShowHours READ isShowHours WRITE setIsShowHours NOTIFY isShowHoursChanged)
    Q_PROPERTY(
        bool isShowMinutes READ isShowMinutes WRITE setIsShowMinutes NOTIFY isShowMinutesChanged)
    Q_PROPERTY(
        bool isShowSeconds READ isShowSeconds WRITE setIsShowSeconds NOTIFY isShowSecondsChanged)
    Q_PROPERTY(int digitWidth READ digitWidth WRITE setDigitWidth NOTIFY digitWidthChanged)
    Q_PROPERTY(int digitHeight READ digitHeight WRITE setDigitHeight NOTIFY digitHeightChanged)
    Q_PROPERTY(int digitSpacing READ digitSpacing WRITE setDigitSpacing NOTIFY digitSpacingChanged)
    Q_PROPERTY(
        int fontPixelSize READ fontPixelSize WRITE setFontPixelSize NOTIFY fontPixelSizeChanged)
public:
    explicit QtCountDown(QWidget* parent = nullptr);
    ~QtCountDown();

    int borderRadius() const;
    void setBorderRadius(int value);

    bool isShowDays() const;
    void setIsShowDays(bool value);

    bool isShowHours() const;
    void setIsShowHours(bool value);

    bool isShowMinutes() const;
    void setIsShowMinutes(bool value);

    bool isShowSeconds() const;
    void setIsShowSeconds(bool value);

    int digitWidth() const;
    void setDigitWidth(int value);

    int digitHeight() const;
    void setDigitHeight(int value);

    int digitSpacing() const;
    void setDigitSpacing(int value);

    int fontPixelSize() const;
    void setFontPixelSize(int value);

Q_SIGNALS:
    void borderRadiusChanged();
    void isShowDaysChanged();
    void isShowHoursChanged();
    void isShowMinutesChanged();
    void isShowSecondsChanged();
    void digitWidthChanged();
    void digitHeightChanged();
    void digitSpacingChanged();
    void fontPixelSizeChanged();

private:
    QScopedPointer<class QtCountDownPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtCountDown)
};
