#ifndef QTTEXT_H
#define QTTEXT_H
#include <QLabel>
#include "qtdef.h"
#include "qtwidgetsextra.h"
class QtTextPrivate;
class QTWIDGETSEXTRA_EXPORT QtText : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool isWrapAnyWhere READ isWrapAnyWhere WRITE setIsWrapAnyWhere NOTIFY
            isWrapAnyWhereChanged)
    Q_PROPERTY(
        int textPixelSize READ textPixelSize WRITE setTextPixelSize NOTIFY textPixelSizeChanged)
    Q_PROPERTY(
        int textPointSize READ textPointSize WRITE setTextPointSize NOTIFY textPointSizeChanged)
    Q_PROPERTY(
        QtTextType::TextStyle textStyle READ textStyle WRITE setTextStyle NOTIFY textStyleChanged)
    Q_PROPERTY(QtIconType::IconName qtIcon READ qtIcon WRITE setQtIcon NOTIFY qtIconChanged)

public:
    explicit QtText(QWidget* parent = nullptr);
    explicit QtText(QString text, QWidget* parent = nullptr);
    explicit QtText(QString text, int pixelSize, QWidget* parent = nullptr);
    ~QtText();

    bool isWrapAnyWhere() const;
    void setIsWrapAnyWhere(bool value);

    int textPixelSize() const;
    void setTextPixelSize(int value);

    QtTextType::TextStyle textStyle() const;
    void setTextStyle(QtTextType::TextStyle value);

    QtIconType::IconName qtIcon() const;
    void setQtIcon(QtIconType::IconName value);

    int textPointSize() const;
    void setTextPointSize(int newTextPointSize);
Q_SIGNALS:
    void isWrapAnyWhereChanged();
    void textPixelSizeChanged();
    void textStyleChanged();
    void qtIconChanged();
    void textPointSizeChanged();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtTextPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtText)
};

#endif // QTTEXT_H
