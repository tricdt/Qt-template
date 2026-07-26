#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtTimelinePrivate;
class QTWIDGETSEXTRA_EXPORT QtTimeline : public QWidget
{
    Q_OBJECT

public:
    struct TimelineItem
    {
        QString title;
        QString content;
        QString timestamp;
        QtIconType::IconName icon = QtIconType::None;
    };
    explicit QtTimeline(QWidget* parent = nullptr);
    ~QtTimeline();

    void addItem(const TimelineItem& item);
    void clearItems();
    int getItemCount() const;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    QScopedPointer<class QtTimelinePrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtTimeline)
};
