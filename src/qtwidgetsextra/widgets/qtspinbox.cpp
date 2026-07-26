#include "qtspinbox.h"
#include "qtdef.h"
#include "qtmenu.h"
#include "qttheme.h"
#include "styles/qtspinboxstyle.h"
#include <qclipboard.h>
#include <qevent.h>
#include <qguiapplication.h>
#include <qlineedit.h>
#include <qpropertyanimation.h>
#include <qvariantanimation.h>

class QtSpinBoxPrivate
{
    Q_DECLARE_PUBLIC(QtSpinBox)
public:
    qreal expandMarkWidth;
    QtSpinBox* q_ptr;
    QtThemeType::ThemeMode themeMode;

    QtSpinBoxPrivate(QtSpinBox* q);
    ~QtSpinBoxPrivate();
    QtSpinBoxStyle* style{ nullptr };
    void onThemeChanged(QtThemeType::ThemeMode themeMode);

    QtMenu* createStandardContextMenu();
};

QtSpinBoxPrivate::QtSpinBoxPrivate(QtSpinBox* q)
    : q_ptr(q)
{
}
QtSpinBoxPrivate::~QtSpinBoxPrivate()
{
}

void QtSpinBoxPrivate::onThemeChanged(QtThemeType::ThemeMode _themeMode)
{
    Q_Q(QtSpinBox);
    themeMode = _themeMode;
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::transparent);
    palette.setColor(QPalette::Text, QtThemeColor(themeMode, BasicText));
    q->lineEdit()->setPalette(palette);
}

QtMenu* QtSpinBoxPrivate::createStandardContextMenu()
{
    Q_Q(QtSpinBox);
    QLineEdit* lineEdit = q->lineEdit();
    QtMenu* menu = new QtMenu(q);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(
            QtIconType::ArrowRotateLeft, QObject::tr("撤销"), QKeySequence::Undo);
        action->setEnabled(lineEdit->isUndoAvailable());
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

        action = menu->addQtIconAction(
            QtIconType::ArrowRotateRight, QObject::tr("恢复"), QKeySequence::Redo);
        action->setEnabled(lineEdit->isRedoAvailable());
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if(!lineEdit->isReadOnly())
    {
        action =
            menu->addQtIconAction(QtIconType::KnifeKitchen, QObject::tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() &&
                           lineEdit->echoMode() == QLineEdit::Normal);
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
    }

    action = menu->addQtIconAction(QtIconType::Copy, QObject::tr("复制"), QKeySequence::Copy);
    action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
    QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::Paste, QObject::tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(
            !lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
    }
#endif
    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::DeleteLeft, QObject::tr("删除"));
        action->setEnabled(
            !lineEdit->isReadOnly() && !lineEdit->text().isEmpty() && lineEdit->hasSelectedText());
        QObject::connect(action, &QAction::triggered, q, [lineEdit](bool) {
            if(lineEdit->hasSelectedText())
            {
                int startIndex = lineEdit->selectionStart();
                int endIndex = lineEdit->selectionEnd();
                lineEdit->setText(lineEdit->text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if(!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(QObject::tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(
        !lineEdit->text().isEmpty() && !(lineEdit->selectedText() == lineEdit->text()));
    QObject::connect(action, &QAction::triggered, q, &QtSpinBox::selectAll);
    return menu;
}

QtSpinBox::QtSpinBox(QWidget* parent)
    : QSpinBox{ parent }
    , d_ptr(new QtSpinBoxPrivate(this))
{
    Q_D(QtSpinBox);
    d->q_ptr = this;
    d->expandMarkWidth = 0;
    setFixedSize(115, 35);
    d->style = new QtSpinBoxStyle(style());
    setStyle(d->style);
    lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
}

QtSpinBox::~QtSpinBox()
{
    Q_D(QtSpinBox);
    delete d->style;
}

QtSpinBoxType::ButtonMode QtSpinBox::buttonMode() const
{
    Q_D(const QtSpinBox);
    return d->style->buttonMode();
}

void QtSpinBox::setButtonMode(QtSpinBoxType::ButtonMode buttonMode)
{
    Q_D(QtSpinBox);
    if(minimumWidth() < 90)
    {
        setMinimumWidth(90);
    }
    d->style->setButtonMode(buttonMode);
    switch(buttonMode)
    {
        case QtSpinBoxType::Inline: {
            lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            lineEdit()->setStyleSheet(
                "background-color:transparent;padding-left:10px;padding-bottom:3px;");
            break;
        }
        case QtSpinBoxType::Compact:
        case QtSpinBoxType::Side:
        case QtSpinBoxType::PMSide: {
            lineEdit()->setAlignment(Qt::AlignCenter);
            lineEdit()->setStyleSheet("background-color:transparent;padding-bottom:3px;");
            break;
        }
    }
    setFrame(hasFrame());
    d->onThemeChanged(qtTheme->getThemeMode());
    Q_EMIT buttonModeChanged(buttonMode);
}

void QtSpinBox::focusInEvent(QFocusEvent* event)
{
    Q_D(QtSpinBox);
    if(event->reason() == Qt::MouseFocusReason)
    {
        QVariantAnimation* markAnimation = new QVariantAnimation(this);
        connect(
            markAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
                d_ptr->expandMarkWidth = value.toReal();
                update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->expandMarkWidth);
        markAnimation->setEndValue(width() / (qreal) 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QSpinBox::focusInEvent(event);
}

void QtSpinBox::focusOutEvent(QFocusEvent* event)
{
    Q_D(QtSpinBox);
    if(event->reason() != Qt::PopupFocusReason)
    {
        QVariantAnimation* markAnimation = new QVariantAnimation(this);
        connect(
            markAnimation, &QPropertyAnimation::valueChanged, this, [this](const QVariant& value) {
                d_ptr->expandMarkWidth = value.toReal();
                update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->expandMarkWidth);
        markAnimation->setEndValue((qreal) 0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QSpinBox::focusOutEvent(event);
}

void QtSpinBox::paintEvent(QPaintEvent* event)
{
    Q_D(QtSpinBox);
    if(palette().color(QPalette::Text) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QSpinBox::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
    painter.drawRoundedRect(QRectF(width() / (qreal) 2 - d->expandMarkWidth, height() - 2.5,
                                d->expandMarkWidth * 2, 2.5),
        2, 2);
    painter.restore();
}

void QtSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(QtSpinBox);
    QtMenu* menu = d->createStandardContextMenu();
    if(!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addQtIconAction(QtIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addQtIconAction(QtIconType::Minus, tr("减少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos =
        (event->reason() == QContextMenuEvent::Mouse)
            ? event->globalPos()
            : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if(that && action)
    {
        if(action == up)
        {
            stepBy(1);
        }
        else if(action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}
