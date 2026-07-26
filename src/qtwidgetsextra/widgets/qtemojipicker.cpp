#include "qtemojipicker.h"

class QtEmojiPickerPrivate
{
    Q_DECLARE_PUBLIC(QtEmojiPicker)
public:
    int emojiSize;
    int columns;
    QtEmojiPicker* q_ptr;
    QtEmojiPickerPrivate(QtEmojiPicker* q);
    ~QtEmojiPickerPrivate();
};

QtEmojiPickerPrivate::QtEmojiPickerPrivate(QtEmojiPicker* q)
    : q_ptr(q)
{
}
QtEmojiPickerPrivate::~QtEmojiPickerPrivate()
{
}

QtEmojiPicker::QtEmojiPicker(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtEmojiPickerPrivate(this))
{
}

QtEmojiPicker::~QtEmojiPicker()
{
}

int QtEmojiPicker::emojiSize() const
{
    Q_D(const QtEmojiPicker);
    return d->emojiSize;
}

void QtEmojiPicker::setEmojiSize(int value)
{
    Q_D(QtEmojiPicker);
    if (d->emojiSize != value) {
        d->emojiSize = value;
        Q_EMIT emojiSizeChanged();
    }
}

int QtEmojiPicker::columns() const
{
    Q_D(const QtEmojiPicker);
    return d->columns;
}

void QtEmojiPicker::setColumns(int value)
{
    Q_D(QtEmojiPicker);
    if (d->columns != value) {
        d->columns = value;
        Q_EMIT columnsChanged();
    }
}