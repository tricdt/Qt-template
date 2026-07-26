#include "qtcombobox.h"
#include "qtdef.h"
#include "qttheme.h"
#include "styles/qtcomboboxstyle.h"
#include "widgets/qtscrollbar.h"
#include <qabstractitemview.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <qevent.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpropertyanimation.h>

class QtComboBoxPrivate
{
    Q_DECLARE_PUBLIC(QtComboBox)
public:
    bool isAllowHidePopup;
    QtComboBoxStyle* comboBoxStyle{ nullptr };
    QtThemeType::ThemeMode themeMode;

    int borderRadius;

    QtComboBox* q_ptr;
    QtComboBoxPrivate(QtComboBox* q);
    ~QtComboBoxPrivate();

    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtComboBoxPrivate::QtComboBoxPrivate(QtComboBox* q)
    : borderRadius(3)
    , q_ptr(q)
{
}
QtComboBoxPrivate::~QtComboBoxPrivate()
{
}

void QtComboBoxPrivate::onThemeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtComboBox);
    this->themeMode = themeMode;
    auto lineEdit = q->lineEdit();
    if(lineEdit)
    {
        QPalette palette = lineEdit->palette();
        palette.setColor(QPalette::Text, QtThemeColor(this->themeMode, BasicText));
        palette.setColor(QPalette::PlaceholderText, this->themeMode == QtThemeType::Light
                                                        ? QColor(0x00, 0x00, 0x00, 128)
                                                        : QColor(0xBA, 0xBA, 0xBA));
        lineEdit->setPalette(palette);
    }
}

QtComboBox::QtComboBox(QWidget* parent)
    : QComboBox{ parent }
    , d_ptr(new QtComboBoxPrivate(this))
{
    Q_D(QtComboBox);
    d->themeMode = qtTheme->getThemeMode();
    setObjectName("QtComboBox");
    setFixedHeight(35);
    d->comboBoxStyle = new QtComboBoxStyle(style());
    setStyle(d->comboBoxStyle);
    setView(new QListView(this));
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QtScrollBar* scrollBar = new QtScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);
    QtScrollBar* floatVScrollBar = new QtScrollBar(scrollBar, comboBoxView);
    floatVScrollBar->setIsAnimation(true);
    comboBoxView->setAutoScroll(false);
    comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
    comboBoxView->setObjectName("QtComboBoxView");
    comboBoxView->setStyleSheet("#QtComboBoxView{background-color:transparent;}");
    comboBoxView->setStyle(d->comboBoxStyle);
    QWidget* container = this->findChild<QFrame*>();
    if(container)
    {
        container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setObjectName("QtComboBoxContainer");
        container->setStyle(d->comboBoxStyle);
        QLayout* layout = container->layout();
        while(layout->count())
        {
            layout->takeAt(0);
        }
        layout->addWidget(view());
        layout->setContentsMargins(6, 0, 6, 6);
#ifndef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        container->setStyleSheet("background-color:transparent;");
#endif
#endif
    }
    QComboBox::setMaxVisibleItems(5);
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](const QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
}

QtComboBox::~QtComboBox()
{
    Q_D(QtComboBox);
    delete d->comboBoxStyle;
}

int QtComboBox::borderRadius() const
{
    Q_D(const QtComboBox);
    return d->borderRadius;
}
void QtComboBox::setBorderRadius(int value)
{
    Q_D(QtComboBox);
    d->borderRadius = value;
}

void QtComboBox::setEditable(bool editable)
{
    Q_D(QtComboBox);
    QComboBox::setEditable(editable);
    if(editable)
    {
        lineEdit()->setStyle(d->comboBoxStyle);
        d->onThemeChanged(d->themeMode);
    }
}

void QtComboBox::showPopup()
{
    Q_D(QtComboBox);
    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);
    if(count() > 0)
    {
        QWidget* container = this->findChild<QFrame*>();
        if(container)
        {
            int containerHeight = 0;
            if(count() >= maxVisibleItems())
            {
                containerHeight = maxVisibleItems() * 35 + 8;
            }
            else
            {
                containerHeight = count() * 35 + 8;
            }
            view()->resize(view()->width(), containerHeight - 8);
            container->move(container->x(), container->y() + 3);
            QLayout* layout = container->layout();
            while(layout->count())
            {
                layout->takeAt(0);
            }
            QPropertyAnimation* fixedSizeAnimation =
                new QPropertyAnimation(container, "maximumHeight");
            connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this,
                [=](const QVariant& value) { container->setFixedHeight(value.toUInt()); });
            fixedSizeAnimation->setStartValue(1);
            fixedSizeAnimation->setEndValue(containerHeight);
            fixedSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
            fixedSizeAnimation->setDuration(400);
            fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(view(), "pos");
            connect(viewPosAnimation, &QPropertyAnimation::finished, this, [this, layout]() {
                d_ptr->isAllowHidePopup = true;
                layout->addWidget(view());
            });
            QPoint viewPos = view()->pos();
            viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
            viewPosAnimation->setEndValue(viewPos);
            viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
            viewPosAnimation->setDuration(400);
            viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        QPropertyAnimation* rotateAnimation =
            new QPropertyAnimation(d->comboBoxStyle, "expandIconRotate");
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
            [this](const QVariant&) { update(); });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->comboBoxStyle->expandIconRotate());
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* markAnimation =
            new QPropertyAnimation(d->comboBoxStyle, "expandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->comboBoxStyle->expandMarkWidth());
        markAnimation->setEndValue(width() / 2 - d->borderRadius - 6);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
void QtComboBox::hidePopup()
{
    Q_D(QtComboBox);
    if(d->isAllowHidePopup)
    {
        QWidget* container = this->findChild<QFrame*>();
        int containerHeight = container->height();
        if(container)
        {
            QLayout* layout = container->layout();
            while(layout->count())
            {
                layout->takeAt(0);
            }
            QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(view(), "pos");
            connect(viewPosAnimation, &QPropertyAnimation::finished, this,
                [this, layout, container, containerHeight]() {
                    layout->addWidget(view());
                    QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1),
                        Qt::NoButton, Qt::NoButton, Qt::NoModifier);
                    QApplication::sendEvent(parentWidget(), &focusEvent);
                    QComboBox::hidePopup();
                    container->setFixedHeight(containerHeight);
                });
            QPoint viewPos = view()->pos();
            connect(viewPosAnimation, &QPropertyAnimation::finished, this,
                [=]() { view()->move(viewPos); });
            viewPosAnimation->setStartValue(viewPos);
            viewPosAnimation->setEndValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
            viewPosAnimation->setEasingCurve(QEasingCurve::InCubic);
            viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* fixedSizeAnimation =
                new QPropertyAnimation(container, "maximumHeight");
            connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this,
                [=](const QVariant& value) { container->setFixedHeight(value.toUInt()); });
            fixedSizeAnimation->setStartValue(container->height());
            fixedSizeAnimation->setEndValue(1);
            fixedSizeAnimation->setEasingCurve(QEasingCurve::InCubic);
            fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            d->isAllowHidePopup = false;
        }
        QPropertyAnimation* rotateAnimation =
            new QPropertyAnimation(d->comboBoxStyle, "expandIconRotate");
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this,
            [this](const QVariant&) { update(); });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->comboBoxStyle->expandIconRotate());
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* markAnimation =
            new QPropertyAnimation(d->comboBoxStyle, "expandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->comboBoxStyle->expandMarkWidth());
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
void QtComboBox::paintEvent(QPaintEvent* event)
{
    Q_D(QtComboBox);
    if(lineEdit() &&
        lineEdit()->palette().color(QPalette::Text) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QComboBox::paintEvent(event);
}