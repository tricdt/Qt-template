#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtWizardPrivate;
class QTWIDGETSEXTRA_EXPORT QtWizard : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(int currentStep READ currentStep WRITE setCurrentStep NOTIFY currentStepChanged)
public:
    explicit QtWizard(QWidget* parent = nullptr);
    ~QtWizard();

    int borderRadius() const;
    void setBorderRadius(int value);

    int currentStep() const;
    void setCurrentStep(int value);

Q_SIGNALS:
    void borderRadiusChanged();
    void currentStepChanged();

private:
    QScopedPointer<class QtWizardPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtWizard)
};
