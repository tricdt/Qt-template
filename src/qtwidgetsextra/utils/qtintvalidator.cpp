#include "qtintvalidator.h"
#include <qvalidator.h>

class QtIntValidatorPrivate
{
    Q_DECLARE_PUBLIC(QtIntValidator)
public:
    bool isHexMode;
    QtIntValidator* q_ptr;
    QtIntValidatorPrivate(QtIntValidator* q);
    ~QtIntValidatorPrivate();
    QString completeInput(QString input, int length) const;
};

QtIntValidatorPrivate::QtIntValidatorPrivate(QtIntValidator* q)
    : q_ptr(q)
{
}
QtIntValidatorPrivate::~QtIntValidatorPrivate()
{
}

QString QtIntValidatorPrivate::completeInput(QString input, int length) const
{
    while(input.length() < length)
    {
        input.prepend("0");
    }
    return input;
}

QtIntValidator::QtIntValidator(QObject* parent)
    : QIntValidator{ parent }
    , d_ptr(new QtIntValidatorPrivate(this))
{
    Q_D(QtIntValidator);
    d->isHexMode = false;
}

QtIntValidator::QtIntValidator(int bottom, int top, QObject* parent)
    : QIntValidator(bottom, top, parent)
    , d_ptr(new QtIntValidatorPrivate(this))
{
    Q_D(QtIntValidator);
    d->isHexMode = false;
}

QtIntValidator::~QtIntValidator()
{
}

QValidator::State QtIntValidator::validate(QString& input, int& pos) const
{
    Q_D(const QtIntValidator);
    QString inputCopy = input;
    if(d->isHexMode)
    {
        inputCopy.remove("#");
        if(!inputCopy.isEmpty())
        {
            bool isInt = false;
            int value = inputCopy.toInt(&isInt, 16);
            if(!isInt)
            {
                return QValidator::Invalid;
            }
            if(value < bottom() || value > top())
            {
                return QValidator::Invalid;
            }
            int topLength = QString::number(top(), 16).length();
            if(inputCopy.length() > topLength)
            {
                return QValidator::Invalid;
            }
        }
        inputCopy.prepend("#");
    }
    else
    {
        if(input.isEmpty())
        {
            return QValidator::Intermediate;
        }
        bool isInt = false;
        int value = inputCopy.toInt(&isInt);
        if(!isInt)
        {
            return QValidator::Invalid;
        }
        if(value < bottom() || value > top())
        {
            return QValidator::Invalid;
        }
    }
    input = inputCopy;
    return QValidator::Acceptable;
}

void QtIntValidator::fixup(QString& input) const
{
    Q_D(const QtIntValidator);
    if(d->isHexMode)
    {
        QString inputComplete = d->completeInput(input, QString::number(top(), 16).length());
        input = QString("#") + inputComplete;
    }
    else
    {
        input = QString::number(bottom());
    }
}

bool QtIntValidator::isHexMode() const
{
    Q_D(const QtIntValidator);
    return d->isHexMode;
}
void QtIntValidator::setIsHexMode(bool value)
{
    Q_D(QtIntValidator);
    d->isHexMode = value;
}