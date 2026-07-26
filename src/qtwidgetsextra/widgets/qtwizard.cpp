#include "qtwizard.h"

class QtWizardPrivate
{
    Q_DECLARE_PUBLIC(QtWizard)
public:
int borderRadius;
    int currentStep;
    QtWizard* q_ptr;
    QtWizardPrivate(QtWizard* q);
    ~QtWizardPrivate();
};

QtWizardPrivate::QtWizardPrivate(QtWizard* q) : q_ptr(q)
{
}
QtWizardPrivate::~QtWizardPrivate()
{
}

QtWizard::QtWizard(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtWizardPrivate(this))
{
}

QtWizard::~QtWizard()
{
}

int QtWizard::borderRadius() const
{
    Q_D(const QtWizard);
    return d->borderRadius;
}

void QtWizard::setBorderRadius(int value)
{
    Q_D(QtWizard);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

int QtWizard::currentStep() const
{
    Q_D(const QtWizard);
    return d->currentStep;
}

void QtWizard::setCurrentStep(int value)
{
    Q_D(QtWizard);
    if (d->currentStep != value) {
        d->currentStep = value;
        Q_EMIT currentStepChanged();
    }
}