#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qtmetamacros.h>

class QtTagPrivate;
class QTWIDGETSEXTRA_EXPORT QtTag : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(QString tagText READ tagText WRITE setTagText NOTIFY tagTextChanged)
    Q_PROPERTY(bool isClosable READ isClosable WRITE setIsClosable NOTIFY isClosableChanged)
    Q_PROPERTY(bool isCheckable READ isCheckable WRITE setIsCheckable NOTIFY isCheckableChanged)
    Q_PROPERTY(bool isChecked READ isChecked WRITE setIsChecked NOTIFY isCheckedChanged)

public:
    enum TagColor
    {
        Default = 0,
        Primary,
        Success,
        Warning,
        Danger
    };
    Q_ENUM(TagColor)
    explicit QtTag(QWidget* parent = nullptr);
    explicit QtTag(const QString& text, QWidget* parent = nullptr);
    ~QtTag();

    void setTagColor(TagColor color);
    TagColor tagColor() const;

    int borderRadius() const;
    void setBorderRadius(int value);

    const QString& tagText() const;
    void setTagText(const QString& value);

    bool isClosable() const;
    void setIsClosable(bool value);

    bool isCheckable() const;
    void setIsCheckable(bool value);

    bool isChecked() const;
    void setIsChecked(bool value);

Q_SIGNALS:
    void closed();
    void clicked();
    void checkedChanged(bool checked);

    void borderRadiusChanged();
    void tagTextChanged();
    void isClosableChanged();
    void isCheckableChanged();
    void isCheckedChanged();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtTagPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTag)
};
