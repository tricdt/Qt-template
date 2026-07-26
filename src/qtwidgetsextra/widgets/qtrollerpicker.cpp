#include "qtrollerpicker.h"
#include "qtroller.h"
#include "qtrollerpickercontainer.h"
#include "qttheme.h"
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qpushbutton.h>
#include <qtmetamacros.h>

class QtRollerPickerPrivate
{
    Q_DECLARE_PUBLIC(QtRollerPicker)
public:
    int borderRadius;
    QtRollerPicker* q_ptr;
    int pickerRollerHeight{ 245 };
    QtThemeType::ThemeMode themeMode;
    QHBoxLayout* containerLayout{ nullptr };
    QtRollerPickerContainer* rollerPickerContainer{ nullptr };
    QtRollerPickerPrivate(QtRollerPicker* q);
    ~QtRollerPickerPrivate();
    void onRollerPickerClicked();
    void onOverButtonClicked();
    void onCancelButtonClicked();
    int rollerTotalWidth() const;
};

QtRollerPickerPrivate::QtRollerPickerPrivate(QtRollerPicker* q)
    : q_ptr(q)
{
}
QtRollerPickerPrivate::~QtRollerPickerPrivate()
{
}

void QtRollerPickerPrivate::onRollerPickerClicked()
{
    Q_Q(QtRollerPicker);
    QPoint targetPos(q->mapToGlobal(QPoint(-6, (q->height() - rollerPickerContainer->height() +
                                                   rollerPickerContainer->buttonAreaHeight()) /
                                                   2)));
    rollerPickerContainer->show();
    rollerPickerContainer->setGeometry(
        QRect(targetPos, QSize(rollerTotalWidth() + 16,
                             pickerRollerHeight + rollerPickerContainer->buttonAreaHeight())));
    rollerPickerContainer->doPickerAnimation();
}
void QtRollerPickerPrivate::onOverButtonClicked()
{
    Q_Q(QtRollerPicker);
    Q_EMIT q->currentDataChanged(q->currentData());
}
void QtRollerPickerPrivate::onCancelButtonClicked()
{
    Q_Q(QtRollerPicker);
}

int QtRollerPickerPrivate::rollerTotalWidth() const
{
    int totalWidth = 0;
    for(auto rollerItem : rollerPickerContainer->rollerList())
    {
        totalWidth += rollerItem->width();
    }
    return totalWidth;
}

QtRollerPicker::QtRollerPicker(QWidget* parent)
    : QPushButton{ parent }
    , d_ptr(new QtRollerPickerPrivate(this))
{
    Q_D(QtRollerPicker);
    d->borderRadius = 3;
    setMouseTracking(true);
    setFixedSize(90, 30);
    setObjectName("QtRollerPicker");
    setStyleSheet("#QtRollerPicker{background-color:transparent;}");
    QFont font = this->font();
    font.setPixelSize(16);
    setFont(font);
    d->rollerPickerContainer = new QtRollerPickerContainer(this);
    d->rollerPickerContainer->resize(
        90, d->pickerRollerHeight + d->rollerPickerContainer->buttonAreaHeight());
    d->rollerPickerContainer->hide();
    connect(this, &QPushButton::clicked, this, [this]() { d_ptr->onRollerPickerClicked(); });
    connect(d->rollerPickerContainer, &QtRollerPickerContainer::overButtonClicked, this,
        [this]() { d_ptr->onOverButtonClicked(); });
    connect(d->rollerPickerContainer, &QtRollerPickerContainer::cancelButtonClicked, this,
        [this]() { d_ptr->onCancelButtonClicked(); });

    d->containerLayout = new QHBoxLayout(d->rollerPickerContainer);
    d->containerLayout->setSpacing(0);
    d->containerLayout->setContentsMargins(0, 0, 0, 0);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtRollerPicker::~QtRollerPicker()
{
}

void QtRollerPicker::addRoller(const QStringList& itemList, bool isEnableLoop)
{
    Q_D(QtRollerPicker);
    if(itemList.isEmpty())
    {
        return;
    }
    QtRoller* roller = new QtRoller(this);
    roller->setIsContainer(true);
    roller->setItemList(itemList);
    roller->setMaxVisibleItems(7);
    roller->setIsEnableLoop(isEnableLoop);
    roller->setItemHeight(35);
    roller->setFixedHeight(d->pickerRollerHeight);
    connect(roller, &QtRoller::currentDataChanged, this,
        [this]() { Q_EMIT currentDataSelectionChanged(currentData()); });
    d->rollerPickerContainer->rollerList().append(roller);
    d->containerLayout->addWidget(roller);
    setFixedWidth(d->rollerTotalWidth());
}

void QtRollerPicker::removeRoller(int index)
{
    Q_D(QtRollerPicker);
    if(index >= d->rollerPickerContainer->rollerList().count())
    {
        return;
    }
    auto roller = d->rollerPickerContainer->rollerList()[index];
    d->rollerPickerContainer->rollerList().removeAt(index);
    d->containerLayout->removeWidget(roller);
    roller->deleteLater();
    setFixedWidth(d->rollerTotalWidth());
}

void QtRollerPicker::setRollerItemList(int index, const QStringList& itemList)
{
    Q_D(QtRollerPicker);
    if(index >= d->rollerPickerContainer->rollerList().count())
    {
        return;
    }
    auto roller = d->rollerPickerContainer->rollerList()[index];
    roller->setItemList(itemList);
}

void QtRollerPicker::setRollerWidth(int index, int width)
{
    Q_D(QtRollerPicker);
    if(index >= d->rollerPickerContainer->rollerList().count())
    {
        return;
    }
    auto roller = d->rollerPickerContainer->rollerList()[index];
    roller->setFixedWidth(width);
    setFixedWidth(d->rollerTotalWidth());
}

void QtRollerPicker::setCurrentData(int index, const QString& data)
{
    Q_D(QtRollerPicker);
    if(index >= d->rollerPickerContainer->rollerList().count())
    {
        return;
    }
    d->rollerPickerContainer->rollerList()[index]->setCurrentData(data);
}

void QtRollerPicker::setCurrentData(const QStringList& dataList)
{
    Q_D(QtRollerPicker);
    for(int i = 0; i < d->rollerPickerContainer->rollerList().count(); i++)
    {
        if(dataList.count() <= i)
        {
            return;
        }
        auto roller = d->rollerPickerContainer->rollerList()[i];
        roller->setCurrentData(dataList[i]);
    }
}

QString QtRollerPicker::currentData(int index) const
{
    Q_D(const QtRollerPicker);
    if(index >= d->rollerPickerContainer->rollerList().count())
    {
        return {};
    }
    return d->rollerPickerContainer->rollerList()[index]->currentData();
}

QStringList QtRollerPicker::currentData() const
{
    Q_D(const QtRollerPicker);
    QStringList dataList;
    for(auto roller : d->rollerPickerContainer->rollerList())
    {
        dataList.append(roller->currentData());
    }
    return dataList;
}

void QtRollerPicker::setCurrentIndex(int rollerIndex, int index)
{
    Q_D(QtRollerPicker);
    if(rollerIndex >= d->rollerPickerContainer->rollerList().count())
    {
        return;
    }
    d->rollerPickerContainer->rollerList()[rollerIndex]->setCurrentIndex(index);
}

void QtRollerPicker::setCurrentIndex(const QList<int>& indexList)
{
    Q_D(QtRollerPicker);
    for(int i = 0; i < d->rollerPickerContainer->rollerList().count(); i++)
    {
        if(indexList.count() <= i)
        {
            return;
        }
        auto roller = d->rollerPickerContainer->rollerList()[i];
        roller->setCurrentIndex(indexList[i]);
    }
}

int QtRollerPicker::currentIndex(int rollerIndex) const
{
    Q_D(const QtRollerPicker);
    if(rollerIndex >= d->rollerPickerContainer->rollerList().count())
    {
        return -1;
    }
    return d->rollerPickerContainer->rollerList()[rollerIndex]->currentIndex();
}

QList<int> QtRollerPicker::currentIndex() const
{
    Q_D(const QtRollerPicker);
    QList<int> currentIndexList;
    for(auto roller : d->rollerPickerContainer->rollerList())
    {
        currentIndexList.append(roller->currentIndex());
    }
    return currentIndexList;
}

void QtRollerPicker::paintEvent(QPaintEvent* event)
{
    Q_D(QtRollerPicker);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QRect baseRect = rect();
    baseRect.adjust(1, 1, -1, -1);
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                  : QtThemeColor(d->themeMode, BasicBase));
    painter.drawRoundedRect(baseRect, d->borderRadius, d->borderRadius);

    int rollerXOffset = 0;
    for(int i = 0; i < d->rollerPickerContainer->rollerList().count(); i++)
    {
        auto roller = d->rollerPickerContainer->rollerList()[i];
        painter.setPen(QtThemeColor(d->themeMode, BasicText));
        int rollerWidth = roller->width();
        painter.drawText(QRect(rollerXOffset, baseRect.y(), rollerWidth, baseRect.height()),
            Qt::AlignCenter, roller->currentData());
        rollerXOffset += rollerWidth;
        if(i != d->rollerPickerContainer->rollerList().count() - 1)
        {
            painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
            painter.drawLine(rollerXOffset, 0, rollerXOffset, baseRect.bottom());
        }
    }
    painter.restore();
}

int QtRollerPicker::borderRadius() const
{
    Q_D(const QtRollerPicker);
    return d->borderRadius;
}
void QtRollerPicker::setBorderRadius(int value)
{
    Q_D(QtRollerPicker);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}