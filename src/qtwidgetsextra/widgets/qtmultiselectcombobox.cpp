#include "qtmultiselectcombobox.h"
#include "itemviews/views/qtcomboboxview.h"
#include "qttheme.h"
#include "styles/qtcomboboxstyle.h"
#include "widgets/qtapplication.h"
#include "widgets/qtscrollbar.h"
#include <qapplication.h>
#include <qlayout.h>
#include <qpropertyanimation.h>
#include <qtypes.h>
#include <qvariantanimation.h>

class QtMultiSelectComboBoxPrivate
{
    Q_DECLARE_PUBLIC(QtMultiSelectComboBox)
public:
    QtComboBoxStyle* comboBoxStyle{ nullptr };
    QtComboBoxView* comboView{ nullptr };
    QVector<bool> itemSelection;
    bool isFirstPopup{ false };
    bool isAllowHidePopup{ false };
    QString currentText;
    QStringList selectedTextList;
    QtThemeType::ThemeMode themeMode;

    qreal expandIconRotate;
    qreal expandMarkWidth;
    int borderRadius;

    QtMultiSelectComboBox* q_ptr;
    QtMultiSelectComboBoxPrivate(QtMultiSelectComboBox* q);
    ~QtMultiSelectComboBoxPrivate();

    void onItemPressed(const QModelIndex& index);
    void refreshCurrentIndexs();
    void adjustSelectedVector();
};

QtMultiSelectComboBoxPrivate::QtMultiSelectComboBoxPrivate(QtMultiSelectComboBox* q)
    : q_ptr(q)
{
}
QtMultiSelectComboBoxPrivate::~QtMultiSelectComboBoxPrivate()
{
}

void QtMultiSelectComboBoxPrivate::onItemPressed(const QModelIndex& index)
{
    Q_Q(QtMultiSelectComboBox);
    if(!comboView->selectionModel()->isSelected(index))
    {
        itemSelection[index.row()] = true;
    }
    else
    {
        itemSelection[index.row()] = false;
    }
    refreshCurrentIndexs();
    QVariantAnimation* markAnimation = new QVariantAnimation(q);
    QObject::connect(
        markAnimation, &QVariantAnimation::valueChanged, q, [this](const QVariant& value) {
            expandMarkWidth = value.toReal();
            q_ptr->update();
        });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(expandMarkWidth);
    qreal step = (q->width() / (qreal) 2 - borderRadius) / q->count();
    markAnimation->setEndValue(step * selectedTextList.count());
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtMultiSelectComboBoxPrivate::refreshCurrentIndexs()
{
    Q_Q(QtMultiSelectComboBox);
    QString str;
    adjustSelectedVector();
    QVector<bool> selectedIndexVector;
    for(int i = 0; i < q->count(); i++)
    {
        // 该位选中
        QModelIndex index = q->model()->index(i, 0);
        if(itemSelection[i])
        {
            comboView->selectionModel()->select(
                index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            if(!str.isEmpty())
            {
                str.append(",");
            }
            str.append(q->itemText(i));
            selectedIndexVector.append(true);
        }
        else
        {
            comboView->selectionModel()->select(index, QItemSelectionModel::Deselect);
            selectedIndexVector.append(false);
        }
    }
    if(str != currentText)
    {
        q->update();
        currentText = str;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        selectedTextList = currentText.split(",", Qt::SkipEmptyParts);
#else
        _selectedTextList = _currentText.split(",");
        if(_selectedTextList.count() == 1 && _selectedTextList[0].isEmpty())
        {
            _selectedTextList.clear();
        }
#endif
        q->setCurrentIndex(-1);
        Q_EMIT q->itemSelectionChanged(selectedIndexVector);
        Q_EMIT q->currentTextListChanged(selectedTextList);
    }
}
void QtMultiSelectComboBoxPrivate::adjustSelectedVector()
{
    Q_Q(QtMultiSelectComboBox);
    while(itemSelection.count() < q->count())
    {
        itemSelection.append(false);
    }
}

QtMultiSelectComboBox::QtMultiSelectComboBox(QWidget* parent)
    : QComboBox{ parent }
    , d_ptr(new QtMultiSelectComboBoxPrivate(this))
{
    Q_D(QtMultiSelectComboBox);
    d->borderRadius = 3;
    d->expandIconRotate = 0;
    d->expandMarkWidth = 0;
    d->themeMode = qtTheme->getThemeMode();
    setFixedHeight(35);

    d->comboBoxStyle = new QtComboBoxStyle(style());
    setStyle(d->comboBoxStyle);

    d->comboView = new QtComboBoxView(this);
    setView(d->comboView);
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QtScrollBar* scrollBar = new QtScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);
    QtScrollBar* floatVScrollBar = new QtScrollBar(scrollBar, comboBoxView);
    floatVScrollBar->setIsAnimation(true);
    comboBoxView->setAutoScroll(false);
    comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
    comboBoxView->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    connect(d->comboView, &QtComboBoxView::itemPressed, this,
        [this](const QModelIndex& index) { d_ptr->onItemPressed(index); });
    connect(this, QOverload<int>::of(&QtMultiSelectComboBox::currentIndexChanged), this,
        [this](int) { d_ptr->refreshCurrentIndexs(); });
    d->itemSelection.resize(32);
    d->itemSelection.fill(false);
    d->itemSelection[0] = true;
    QComboBox::setMaxVisibleItems(5);
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtMultiSelectComboBox::~QtMultiSelectComboBox()
{
}

int QtMultiSelectComboBox::borderRadius() const
{
    Q_D(const QtMultiSelectComboBox);
    return d->borderRadius;
}

void QtMultiSelectComboBox::setBorderRadius(int radius)
{
    Q_D(QtMultiSelectComboBox);
    d->borderRadius = radius;
}

void QtMultiSelectComboBox::setCurrentSelection(QString selection)
{
    Q_D(QtMultiSelectComboBox);
    d->itemSelection.fill(false);
    d->comboView->selectionModel()->clearSelection();
    for(int i = 0; i < this->count(); i++)
    {
        if(selection == itemText(i))
        {
            QModelIndex index = model()->index(i, 0);
            d->comboView->selectionModel()->select(
                index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            d->itemSelection[index.row()] = true;
        }
    }
    d->refreshCurrentIndexs();
}
void QtMultiSelectComboBox::setCurrentSelection(QStringList selection)
{
    Q_D(QtMultiSelectComboBox);
    d->comboView->selectionModel()->clearSelection();
    d->itemSelection.fill(false);
    for(int i = 0; i < this->count(); i++)
    {
        if(selection.contains(itemText(i)))
        {
            QModelIndex index = model()->index(i, 0);
            d->comboView->selectionModel()->select(
                index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            d->itemSelection[index.row()] = true;
        }
    }
    d->refreshCurrentIndexs();
}
void QtMultiSelectComboBox::setCurrentSelection(int index)
{
    Q_D(QtMultiSelectComboBox);
    if(index >= this->count() || index < 0)
    {
        return;
    }
    d->comboView->selectionModel()->clearSelection();
    d->itemSelection.fill(false);
    QModelIndex currentIndex = model()->index(index, 0);
    d->comboView->selectionModel()->select(
        currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    d->itemSelection[index] = true;
    d->refreshCurrentIndexs();
}
void QtMultiSelectComboBox::setCurrentSelection(QList<int> selectionIndex)
{
    Q_D(QtMultiSelectComboBox);
    d->itemSelection.fill(false);
    d->comboView->selectionModel()->clearSelection();
    for(auto index : selectionIndex)
    {
        if(index >= this->count() || index < 0)
        {
            continue;
        }
        QModelIndex currentIndex = model()->index(index, 0);
        d->comboView->selectionModel()->select(
            currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        d->itemSelection[index] = true;
    }
    d->refreshCurrentIndexs();
}
QStringList QtMultiSelectComboBox::currentSelection() const
{
    Q_D(const QtMultiSelectComboBox);
    return d->selectedTextList;
}
QList<int> QtMultiSelectComboBox::currentSelectionIndex() const
{
    Q_D(const QtMultiSelectComboBox);
    QList<int> indexList;
    for(int i = 0; i < d->itemSelection.count(); i++)
    {
        if(d->itemSelection[i])
        {
            indexList.append(i);
        }
    }
    return indexList;
}

void QtMultiSelectComboBox::paintEvent(QPaintEvent*)
{
    Q_D(QtMultiSelectComboBox);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(
        QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(isEnabled() ? underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                                : QtThemeColor(d->themeMode, BasicBase)
                                 : Qt::transparent);
    QRect foregroundRect = rect();
    foregroundRect.adjust(6, 1, -6, -1);
    painter.drawRoundedRect(foregroundRect, d->borderRadius, d->borderRadius);
    // 底边线绘制
    painter.setPen(QtThemeColor(d->themeMode, BasicBaseLine));
    painter.drawLine(foregroundRect.x() + d->borderRadius,
        foregroundRect.y() + foregroundRect.height(),
        foregroundRect.x() + foregroundRect.width() - d->borderRadius,
        foregroundRect.y() + foregroundRect.height());

    // 文字绘制
    painter.setPen(isEnabled() ? QtThemeColor(d->themeMode, BasicText)
                               : QtThemeColor(d->themeMode, BasicTextDisable));
    QString currentText = painter.fontMetrics().elidedText(
        d->currentText, Qt::ElideRight, foregroundRect.width() - 27 - width() * 0.05);
    painter.drawText(15, height() / 2 + painter.fontMetrics().ascent() / 2 - 1, currentText);
    // 展开指示器绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(
        d->themeMode == QtThemeType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
    painter.drawRoundedRect(
        QRectF(width() / 2 - d->expandMarkWidth, height() - 3, d->expandMarkWidth * 2, 3), 2, 2);
    // 展开图标绘制
    if(count() > 0)
    {
        QFont iconFont = QFont("ElaAwesome");
        iconFont.setPixelSize(17);
        painter.setFont(iconFont);
        painter.setPen(isEnabled() ? QtThemeColor(d->themeMode, BasicText)
                                   : QtThemeColor(d->themeMode, BasicTextDisable));
        QRectF expandIconRect(width() - 25, 0, 20, height());
        painter.translate(expandIconRect.x() + (qreal) expandIconRect.width() / 2 - 2,
            expandIconRect.y() + (qreal) expandIconRect.height() / 2);
        painter.rotate(d->expandIconRotate);
        painter.translate(-expandIconRect.x() - (qreal) expandIconRect.width() / 2 + 2,
            -expandIconRect.y() - (qreal) expandIconRect.height() / 2);
        painter.drawText(
            expandIconRect, Qt::AlignVCenter, QChar(static_cast<ushort>(QtIconType::AngleDown)));
        painter.restore();
    }
}
void QtMultiSelectComboBox::showPopup()
{
    Q_D(QtMultiSelectComboBox);
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

        QVariantAnimation* rotateAnimation = new QVariantAnimation(this);
        connect(
            rotateAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
                d_ptr->expandIconRotate = value.toReal();
                update();
            });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->expandIconRotate);
        rotateAnimation->setEndValue((qreal) -180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QVariantAnimation* markAnimation = new QVariantAnimation(this);
        connect(markAnimation, &QVariantAnimation::valueChanged, this,
            [this](const QVariant& value) { d_ptr->expandMarkWidth = value.toReal(); });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->expandMarkWidth);
        qreal step = (width() / (qreal) 2 - d->borderRadius) / count();
        markAnimation->setEndValue(step * d->selectedTextList.count());
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    d->refreshCurrentIndexs();
}
void QtMultiSelectComboBox::hidePopup()
{
    Q_D(QtMultiSelectComboBox);
    if(d->isFirstPopup && !this->view()->underMouse())
    {
        d->isFirstPopup = false;
        return;
    }
    if(qtApp->containsCursorToItem(d->comboView))
    {
        return;
    }
    else
    {
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
                connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
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
            QVariantAnimation* rotateAnimation = new QVariantAnimation(this);
            connect(rotateAnimation, &QVariantAnimation::valueChanged, this,
                [this](const QVariant& value) {
                    d_ptr->expandIconRotate = value.toReal();
                    update();
                });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->expandIconRotate);
            rotateAnimation->setEndValue((qreal) 0);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QVariantAnimation* markAnimation = new QVariantAnimation(this);
            connect(markAnimation, &QVariantAnimation::valueChanged, this,
                [this](const QVariant& value) { d_ptr->expandMarkWidth = value.toReal(); });
            markAnimation->setDuration(300);
            markAnimation->setEasingCurve(QEasingCurve::InOutSine);
            markAnimation->setStartValue(d->expandMarkWidth);
            markAnimation->setEndValue((qreal) 0);
            markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}
