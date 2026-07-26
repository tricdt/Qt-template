#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>
#include <qvalidator.h>

class QtIntValidatorPrivate;
class QTWIDGETSEXTRA_EXPORT QtIntValidator : public QIntValidator
{
    Q_OBJECT

public:
    explicit QtIntValidator(QObject* parent = nullptr);
    explicit QtIntValidator(int bottom, int top, QObject* parent = nullptr);
    ~QtIntValidator();

    bool isHexMode() const;
    void setIsHexMode(bool value);

protected:
    QValidator::State validate(QString& input, int& pos) const override;
    void fixup(QString& input) const override;

private:
    QScopedPointer<class QtIntValidatorPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtIntValidator)
};
