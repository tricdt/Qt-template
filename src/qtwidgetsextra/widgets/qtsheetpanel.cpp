#include "qtsheetpanel.h"

class QtSheetPanelPrivate
{
    Q_DECLARE_PUBLIC(QtSheetPanel)
public:
    int borderRadius;
    QtSheetPanelType::Direction direction;
    qreal peekRatio;
    qreal halfRatio;
    qreal fullRatio;
    bool dragHandleVisible;
    bool closeOnOverlayClick;
    qreal overlayOpacity;
    QtSheetPanel* q_ptr;
    QtSheetPanelPrivate(QtSheetPanel* q);
    ~QtSheetPanelPrivate();
};

QtSheetPanelPrivate::QtSheetPanelPrivate(QtSheetPanel* q)
    : q_ptr(q)
{
}
QtSheetPanelPrivate::~QtSheetPanelPrivate()
{
}

QtSheetPanel::QtSheetPanel(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSheetPanelPrivate(this))
{
}

QtSheetPanel::~QtSheetPanel()
{
}

int QtSheetPanel::borderRadius() const
{
    Q_D(const QtSheetPanel);
    return d->borderRadius;
}

void QtSheetPanel::setBorderRadius(int value)
{
    Q_D(QtSheetPanel);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

QtSheetPanelType::Direction QtSheetPanel::direction() const
{
    Q_D(const QtSheetPanel);
    return d->direction;
}

void QtSheetPanel::setDirection(QtSheetPanelType::Direction value)
{
    Q_D(QtSheetPanel);
    if(d->direction != value)
    {
        d->direction = value;
        Q_EMIT directionChanged();
    }
}

qreal QtSheetPanel::peekRatio() const
{
    Q_D(const QtSheetPanel);
    return d->peekRatio;
}

void QtSheetPanel::setPeekRatio(qreal value)
{
    Q_D(QtSheetPanel);
    if(d->peekRatio != value)
    {
        d->peekRatio = value;
        Q_EMIT peekRatioChanged();
    }
}

qreal QtSheetPanel::halfRatio() const
{
    Q_D(const QtSheetPanel);
    return d->halfRatio;
}

void QtSheetPanel::setHalfRatio(qreal value)
{
    Q_D(QtSheetPanel);
    if(d->halfRatio != value)
    {
        d->halfRatio = value;
        Q_EMIT halfRatioChanged();
    }
}

qreal QtSheetPanel::fullRatio() const
{
    Q_D(const QtSheetPanel);
    return d->fullRatio;
}

void QtSheetPanel::setFullRatio(qreal value)
{
    Q_D(QtSheetPanel);
    if(d->fullRatio != value)
    {
        d->fullRatio = value;
        Q_EMIT fullRatioChanged();
    }
}

bool QtSheetPanel::dragHandleVisible() const
{
    Q_D(const QtSheetPanel);
    return d->dragHandleVisible;
}

void QtSheetPanel::setDragHandleVisible(bool value)
{
    Q_D(QtSheetPanel);
    if(d->dragHandleVisible != value)
    {
        d->dragHandleVisible = value;
        Q_EMIT dragHandleVisibleChanged();
    }
}

bool QtSheetPanel::closeOnOverlayClick() const
{
    Q_D(const QtSheetPanel);
    return d->closeOnOverlayClick;
}

void QtSheetPanel::setCloseOnOverlayClick(bool value)
{
    Q_D(QtSheetPanel);
    if(d->closeOnOverlayClick != value)
    {
        d->closeOnOverlayClick = value;
        Q_EMIT closeOnOverlayClickChanged();
    }
}

qreal QtSheetPanel::overlayOpacity() const
{
    Q_D(const QtSheetPanel);
    return d->overlayOpacity;
}

void QtSheetPanel::setOverlayOpacity(qreal value)
{
    Q_D(QtSheetPanel);
    if(d->overlayOpacity != value)
    {
        d->overlayOpacity = value;
        Q_EMIT overlayOpacityChanged();
    }
}