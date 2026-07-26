#include "qtrollerpickercontainer.h"
#include "qtdef.h"
#include "qtroller.h"
#include "qttheme.h"
#include <qevent.h>
#include <qpropertyanimation.h>
#include <qtmetamacros.h>

class QtRollerPickerContainerPrivate
{
    Q_DECLARE_PUBLIC(QtRollerPickerContainer)
public:
    int animationPixOffsetY;
    int buttonAreaHeight;
    QtRollerPickerContainer* q_ptr;
    int buttonMargin{ 5 };
    int buttonSpacing{ 5 };
    QPixmap animationPix;
    QRect overButtonRect;
    QRect cancelButtonRect;
    bool isOverButtonHover{ false };
    bool isCancelButtonHover{ false };
    bool isOverButtonClicked{ false };
    QList<QtRoller*> rollerList;
    QList<int> historyIndexList;
    QtThemeType::ThemeMode themeMode;
    QtRollerPickerContainerPrivate(QtRollerPickerContainer* q);
    ~QtRollerPickerContainerPrivate();
    void handleSaveOrReset(bool isSave);
};

QtRollerPickerContainerPrivate::QtRollerPickerContainerPrivate(QtRollerPickerContainer* q)
    : q_ptr(q)
{
}
QtRollerPickerContainerPrivate::~QtRollerPickerContainerPrivate()
{
}

void QtRollerPickerContainerPrivate::handleSaveOrReset(bool isSave)
{
    if(isSave)
    {
        historyIndexList.clear();
        for(auto roller : rollerList)
        {
            historyIndexList.append(roller->currentIndex());
        }
    }
    else
    {
        for(int i = 0; i < rollerList.count(); i++)
        {
            auto roller = rollerList[i];
            roller->setCurrentIndex(historyIndexList[i]);
        }
    }
}

QtRollerPickerContainer::QtRollerPickerContainer(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtRollerPickerContainerPrivate(this))
{
    Q_D(QtRollerPickerContainer);
    d->animationPixOffsetY = 0;
    d->buttonAreaHeight = 38;
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
    setContentsMargins(2, 2, 2, d->buttonAreaHeight);
#else
    setContentsMargins(8, 8, 8, 6 + d->buttonAreaHeight);
#endif
    setObjectName("QtCalendarPickerContainer");
    setStyleSheet("#QtCalendarPickerContainer{background-color:transparent}");

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
}

QtRollerPickerContainer::~QtRollerPickerContainer()
{
}

void QtRollerPickerContainer::doPickerAnimation()
{
    Q_D(QtRollerPickerContainer);
    d->handleSaveOrReset(true);
    if(!d->animationPix.isNull())
    {
        d->animationPix = QPixmap();
    }
    d->animationPix = this->grab(rect());
    QPropertyAnimation* offsetAnimation = new QPropertyAnimation(this, "animationPixOffsetY");
    connect(offsetAnimation, &QPropertyAnimation::finished, this, [this]() {
        d_ptr->animationPix = QPixmap();
        update();
    });
    connect(offsetAnimation, &QPropertyAnimation::valueChanged, this,
        [this](const QVariant& value) { update(); });
    offsetAnimation->setEasingCurve(QEasingCurve::OutCubic);
    offsetAnimation->setDuration(175);
    offsetAnimation->setStartValue(70);
    offsetAnimation->setEndValue(0);
    offsetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtRollerPickerContainer::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtRollerPickerContainer);
    auto currentPos = event->pos();
    if(d->overButtonRect.contains(currentPos))
    {
        d->isOverButtonHover = true;
        d->isCancelButtonHover = false;
        update();
    }
    else if(d->cancelButtonRect.contains(currentPos))
    {
        d->isCancelButtonHover = true;
        d->isOverButtonHover = false;
        update();
    }
    else
    {
        if(d->isCancelButtonHover || d->isOverButtonHover)
        {
            d->isCancelButtonHover = false;
            d->isOverButtonHover = false;
            update();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void QtRollerPickerContainer::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtRollerPickerContainer);
    auto currentPos = event->pos();
    if(d->overButtonRect.contains(currentPos))
    {
        d->isOverButtonClicked = true;
        Q_EMIT overButtonClicked();
        hide();
    }
    else if(d->cancelButtonRect.contains(currentPos))
    {
        d->handleSaveOrReset(false);
        Q_EMIT cancelButtonClicked();
        hide();
    }
    QWidget::mouseReleaseEvent(event);
}

void QtRollerPickerContainer::leaveEvent(QEvent* event)
{
    Q_D(QtRollerPickerContainer);
    d->isOverButtonHover = false;
    d->isCancelButtonHover = false;
    update();
    QWidget::leaveEvent(event);
}

void QtRollerPickerContainer::hideEvent(QHideEvent* event)
{
    Q_D(QtRollerPickerContainer);
    if(d->isOverButtonClicked)
    {
        d->isOverButtonClicked = false;
    }
    else
    {
        d->handleSaveOrReset(false);
    }
    QWidget::hideEvent(event);
}

void QtRollerPickerContainer::paintEvent(QPaintEvent* event)
{
    Q_D(QtRollerPickerContainer);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if(!d->animationPix.isNull())
    {
        painter.setClipRect(
            QRect(0, d->animationPixOffsetY, width(), height() - 2 * d->animationPixOffsetY));
        painter.drawPixmap(rect(), d->animationPix);
    }
    else
    {
#if defined(Q_OS_WIN) && QT_VERSION == QT_VERSION_CHECK(6, 11, 0)
        QRect foregroundRect = rect();
#else
        qtTheme->drawEffectShadow(&painter, rect(), 6, 5);
        QRect foregroundRect(6, 6, rect().width() - 2 * 6, rect().height() - 2 * 6);
#endif
        painter.setPen(QtThemeColor(d->themeMode, PopupBorder));
        painter.setBrush(QtThemeColor(d->themeMode, PopupBase));
        painter.drawRoundedRect(foregroundRect, 8, 8);
        painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
        int rollerXOffset = 8;
        for(int i = 0; i < d->rollerList.count(); i++)
        {
            rollerXOffset += d->rollerList[i]->width();
            if(i != d->rollerList.count() - 1)
            {
                painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
                painter.drawLine(rollerXOffset, foregroundRect.y(), rollerXOffset,
                    foregroundRect.bottom() - d->buttonAreaHeight);
            }
        }
        painter.drawLine(foregroundRect.x(), foregroundRect.bottom() - d->buttonAreaHeight,
            foregroundRect.right(), foregroundRect.bottom() - d->buttonAreaHeight);
        d->overButtonRect = QRect(foregroundRect.x() + d->buttonMargin,
            foregroundRect.bottom() - d->buttonAreaHeight + d->buttonMargin,
            (foregroundRect.width() - 2 * d->buttonMargin - d->buttonSpacing) / 2,
            d->buttonAreaHeight - 2 * d->buttonMargin + 1);
        d->cancelButtonRect = QRect(d->overButtonRect.right() + d->buttonSpacing,
            d->overButtonRect.y(), d->overButtonRect.width(), d->overButtonRect.height());
        painter.setPen(Qt::NoPen);
        painter.setBrush(QtThemeColor(d->themeMode, BasicHover));
        if(d->isOverButtonHover)
        {
            painter.drawRoundedRect(d->overButtonRect, 5, 5);
        }
        else if(d->isCancelButtonHover)
        {
            painter.drawRoundedRect(d->cancelButtonRect, 5, 5);
        }
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter.setFont(iconFont);
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        painter.drawText(
            d->overButtonRect, Qt::AlignCenter, QChar((unsigned short) QtIconType::Check));
        painter.drawText(
            d->cancelButtonRect, Qt::AlignCenter, QChar((unsigned short) QtIconType::Xmark));
    }
    painter.restore();
}

int QtRollerPickerContainer::animationPixOffsetY() const
{
    Q_D(const QtRollerPickerContainer);
    return d->animationPixOffsetY;
}
void QtRollerPickerContainer::setAnimationPixOffsetY(int value)
{
    Q_D(QtRollerPickerContainer);
    d->animationPixOffsetY = value;
    Q_EMIT animationPixOffsetYChanged();
}

QList<QtRoller*>& QtRollerPickerContainer::rollerList()
{
    Q_D(QtRollerPickerContainer);
    return d->rollerList;
}

int QtRollerPickerContainer::buttonAreaHeight() const
{
    Q_D(const QtRollerPickerContainer);
    return d->buttonAreaHeight;
}
void QtRollerPickerContainer::setButtonAreaHeight(int value)
{
    Q_D(QtRollerPickerContainer);
    d->buttonAreaHeight = value;
}