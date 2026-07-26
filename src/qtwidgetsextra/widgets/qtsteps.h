#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtStepsPrivate;
class QTWIDGETSEXTRA_EXPORT QtSteps : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentStep READ currentStep WRITE setCurrentStep NOTIFY currentStepChanged)
    Q_PROPERTY(int stepCount READ stepCount WRITE setStepCount NOTIFY stepCountChanged)
public:
    explicit QtSteps(QWidget* parent = nullptr);
    ~QtSteps();

    void setStepTitles(const QStringList& titles);
    QStringList stepTitles() const;

    void next();
    void previous();

    // 布局方向：Qt::Horizontal（默认，横向）/ Qt::Vertical（纵向，标题自动换行）
    void setStepOrientation(Qt::Orientation orientation);
    Qt::Orientation stepOrientation() const;

    int currentStep() const;
    void setCurrentStep(int value);

    int stepCount() const;
    void setStepCount(int value);

Q_SIGNALS:
    void currentStepChanged(int step);
    void currentStepChanged();
    void stepCountChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtStepsPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSteps)
};
