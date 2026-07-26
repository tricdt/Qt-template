#include "qtdrawercontainer.h"
#include "qttheme.h"
#include <qboxlayout.h>
#include <qpropertyanimation.h>

class QtDrawerContainerPrivate
{
    Q_DECLARE_PUBLIC(QtDrawerContainer)
public:
    int borderRadius;
    QPixmap containerPix;
    qreal opacity;
    QtDrawerContainer* q_ptr;
    bool isShowBorder{ true };
    QtThemeType::ThemeMode themeMode;
    QVBoxLayout* mainLayout{ nullptr };
    QVBoxLayout* containerLayout{ nullptr };
    QWidget* containerWidget{ nullptr };
    QList<QWidget*> drawerWidgetList;
    QtDrawerContainerPrivate(QtDrawerContainer* q);
    ~QtDrawerContainerPrivate();
};

QtDrawerContainerPrivate::QtDrawerContainerPrivate(QtDrawerContainer* q)
    : q_ptr(q)
{
}
QtDrawerContainerPrivate::~QtDrawerContainerPrivate()
{
}

QtDrawerContainer::QtDrawerContainer(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtDrawerContainerPrivate(this))
{
    Q_D(QtDrawerContainer);
    d->borderRadius = 6;
    d->opacity = 0;
    d->containerPix = QPixmap();
    setObjectName("QtDrawerContainer");
    setStyleSheet("#QtDrawerContainer{background-color:transparent;}");

    d->mainLayout = new QVBoxLayout(this);
    d->mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    d->mainLayout->setContentsMargins(0, 0, 0, 0);

    d->containerWidget = new QWidget(this);
    d->containerWidget->setObjectName("QtDrawerContainerWidget");
    d->containerWidget->setStyleSheet("#QtDrawerContainerWidget{background-color:transparent;}");
    d->containerWidget->setVisible(false);

    d->containerLayout = new QVBoxLayout(d->containerWidget);
    d->containerLayout->setContentsMargins(0, 0, 0, 0);
    d->containerLayout->setSpacing(0);

    d->mainLayout->addWidget(d->containerWidget);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtDrawerContainer::~QtDrawerContainer()
{
}

void QtDrawerContainer::addWidget(QWidget* widget)
{
    Q_D(QtDrawerContainer);
    if(!widget || d->drawerWidgetList.contains(widget))
    {
        return;
    }
    d->containerLayout->addWidget(widget);
    d->drawerWidgetList.append(widget);
}

void QtDrawerContainer::removeWidget(QWidget* widget)
{
    Q_D(QtDrawerContainer);
    if(!widget)
    {
        return;
    }
    d->containerLayout->removeWidget(widget);
    d->drawerWidgetList.removeOne(widget);
}

void QtDrawerContainer::doDrawerAnimation(bool isExpand)
{
    Q_D(QtDrawerContainer);
    if(d->containerLayout->count() == 0)
    {
        return;
    }
    d->containerWidget->setVisible(true);
    d->mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    int expandHeight = height();
    d->isShowBorder = true;
    setFixedHeight(expandHeight);
    d->containerPix = grab(rect());
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "opacity");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) { update(); });
    connect(opacityAnimation, &QPropertyAnimation::finished, this, [this, isExpand]() {
        d_ptr->containerPix = QPixmap();
        d_ptr->isShowBorder = isExpand;
        d_ptr->containerWidget->setVisible(isExpand);
        d_ptr->mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    });
    opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    opacityAnimation->setDuration(300);
    opacityAnimation->setStartValue(d->opacity);
    opacityAnimation->setEndValue(isExpand ? 1.0 : 0.0);
    opacityAnimation->start(QPropertyAnimation::DeleteWhenStopped);
    d->containerWidget->setVisible(false);
    d->isShowBorder = false;
}

void QtDrawerContainer::paintEvent(QPaintEvent* event)
{
    Q_D(QtDrawerContainer);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if(d->isShowBorder)
    {
        painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
        painter.setBrush(QtThemeColor(d->themeMode, BasicBaseAlpha));
        QRect foregroundRect(
            1, 1 - 2 * d->borderRadius, width() - 2, height() - 2 + 2 * d->borderRadius);
        painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
        int drawerHeight = 0;
        for(int i = 0; i < d->drawerWidgetList.count() - 1; i++)
        {
            QWidget* drawerWidget = d->drawerWidgetList[i];
            drawerHeight += drawerWidget->height();
            painter.drawLine(0, drawerHeight, width(), drawerHeight);
        }
    }
    if(!d->containerPix.isNull())
    {
        painter.setOpacity(d->opacity);
        painter.drawPixmap(
            QRect(0, -height() * (1 - d->opacity), width(), height()), d->containerPix);
    }
    painter.restore();
}

int QtDrawerContainer::borderRadius() const
{
    Q_D(const QtDrawerContainer);
    return d->borderRadius;
}
void QtDrawerContainer::setBorderRadius(int value)
{
    Q_D(QtDrawerContainer);
    d->borderRadius = value;
}
QPixmap QtDrawerContainer::containerPix() const
{
    Q_D(const QtDrawerContainer);
    return d->containerPix;
}
void QtDrawerContainer::setContainerPix(QPixmap value)
{
    Q_D(QtDrawerContainer);
    d->containerPix = value;
}
qreal QtDrawerContainer::opacity() const
{
    Q_D(const QtDrawerContainer);
    return d->opacity;
}
void QtDrawerContainer::setOpacity(qreal value)
{
    Q_D(QtDrawerContainer);
    d->opacity = value;
}