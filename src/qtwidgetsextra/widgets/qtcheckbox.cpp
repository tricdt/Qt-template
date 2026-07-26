#include "qtcheckbox.h"
#include "styles/qtcheckboxstyle.h"
#include <qcheckbox.h>

class QtCheckBoxPrivate
{
    Q_DECLARE_PUBLIC(QtCheckBox)
public:
    int borderRadius;
    QtCheckBox* q_ptr;
    QtCheckBoxPrivate(QtCheckBox* q);
    ~QtCheckBoxPrivate();
};

QtCheckBoxPrivate::QtCheckBoxPrivate(QtCheckBox* q)
    : borderRadius(3)
    , q_ptr(q)
{
}
QtCheckBoxPrivate::~QtCheckBoxPrivate()
{
}

QtCheckBox::QtCheckBox(QWidget* parent)
    : QCheckBox{ parent }
    , d_ptr(new QtCheckBoxPrivate(this))
{
    setMouseTracking(true);
    setObjectName("QtCheckBox");
    setStyle(new QtCheckBoxStyle(style()));
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
}

QtCheckBox::QtCheckBox(const QString& text, QWidget* parent)
    : QtCheckBox(parent)
{
    setText(text);
}

QtCheckBox::~QtCheckBox()
{
    delete this->style();
}

int QtCheckBox::borderRadius() const
{
    Q_D(const QtCheckBox);
    return d->borderRadius;
}
void QtCheckBox::setBorderRadius(int value)
{
    Q_D(QtCheckBox);
    d->borderRadius = value;
}