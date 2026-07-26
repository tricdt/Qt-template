#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtEmojiPickerPrivate;
class QTWIDGETSEXTRA_EXPORT QtEmojiPicker : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int emojiSize READ emojiSize WRITE setEmojiSize NOTIFY emojiSizeChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)
public:
    explicit QtEmojiPicker(QWidget* parent = nullptr);
    ~QtEmojiPicker();

    int emojiSize() const;
    void setEmojiSize(int value);

    int columns() const;
    void setColumns(int value);

Q_SIGNALS:
    void emojiSizeChanged();
    void columnsChanged();

private:
    QScopedPointer<class QtEmojiPickerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtEmojiPicker)
};
