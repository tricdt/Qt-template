#include "qtdoublespinbox.h"
#include "qtdef.h"
#include "qttheme.h"
#include "styles/qtspinboxstyle.h"
#include "widgets/qtmenu.h"
#include <qclipboard.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <qpropertyanimation.h>
#include <qtimer.h>
#include <qvariantanimation.h>

class QtDoubleSpinBoxPrivate
{
    Q_DECLARE_PUBLIC(QtDoubleSpinBox)
    Q_DECLARE_TR_FUNCTIONS(QtDoubleSpinBoxPrivate)
public:
    QtSpinBoxType::ButtonMode buttonMode;
    QtDoubleSpinBox* q_ptr;
    qreal expandMarkWidth;
    QtSpinBoxStyle* style{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QtDoubleSpinBoxPrivate(QtDoubleSpinBox* q);
    ~QtDoubleSpinBoxPrivate();
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
    QtMenu* createStandardContextMenu();
    void changeTheme();
};

QtDoubleSpinBoxPrivate::QtDoubleSpinBoxPrivate(QtDoubleSpinBox* q)
    : q_ptr(q)
{
}
QtDoubleSpinBoxPrivate::~QtDoubleSpinBoxPrivate()
{
}

void QtDoubleSpinBoxPrivate::onThemeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtDoubleSpinBox);
    this->themeMode = themeMode;
    if(q->isVisible())
    {
        changeTheme();
    }
    else
    {
        QTimer::singleShot(1, q, [this] { changeTheme(); });
    }
}
QtMenu* QtDoubleSpinBoxPrivate::createStandardContextMenu()
{
    Q_Q(QtDoubleSpinBox);
    QLineEdit* lineEdit = q->lineEdit();
    QtMenu* menu = new QtMenu(q);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(lineEdit->isUndoAvailable());
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::undo);

        action =
            menu->addQtIconAction(QtIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(lineEdit->isRedoAvailable());
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!lineEdit->isReadOnly() && lineEdit->hasSelectedText() &&
                           lineEdit->echoMode() == QLineEdit::Normal);
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::cut);
    }

    action = menu->addQtIconAction(QtIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(lineEdit->hasSelectedText() && lineEdit->echoMode() == QLineEdit::Normal);
    QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::copy);

    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(
            !lineEdit->isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        QObject::connect(action, &QAction::triggered, lineEdit, &QLineEdit::paste);
    }
#endif
    if(!lineEdit->isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::DeleteLeft, tr("删除"));
        action->setEnabled(
            !lineEdit->isReadOnly() && !lineEdit->text().isEmpty() && lineEdit->hasSelectedText());
        QObject::connect(action, &QAction::triggered, q, [this, lineEdit](bool checked) {
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
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(
        !lineEdit->text().isEmpty() && !(lineEdit->selectedText() == lineEdit->text()));
    QObject::connect(action, &QAction::triggered, q, &QtDoubleSpinBox::selectAll);
    return menu;
}
void QtDoubleSpinBoxPrivate::changeTheme()
{
}

QtDoubleSpinBox::QtDoubleSpinBox(QWidget* parent)
    : QDoubleSpinBox{ parent }
    , d_ptr(new QtDoubleSpinBoxPrivate(this))
{
    Q_D(QtDoubleSpinBox);
    d->expandMarkWidth = 0;
    setLocale(QLocale::C);
    setFixedSize(115, 35);
#ifdef Q_OS_MACOS
    setAttribute(Qt::WA_Hover);
    setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    d->style = new QtSpinBoxStyle(style());
    setStyle(d->style);
    lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
#ifdef Q_OS_MACOS
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;"
                              "border:none;outline:none;");
#else
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
#endif
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
}

QtDoubleSpinBox::~QtDoubleSpinBox()
{
    Q_D(QtDoubleSpinBox);
    delete d->style;
}

void QtDoubleSpinBox::setButtonMode(QtSpinBoxType::ButtonMode buttonMode)
{
    Q_D(QtDoubleSpinBox);
    if(minimumWidth() < 90)
    {
        setMinimumWidth(90);
    }
    d->style->setButtonMode(buttonMode);
    switch(buttonMode)
    {
        case QtSpinBoxType::Inline: {
            lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
#ifdef Q_OS_MACOS
            lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-"
                                      "bottom:3px;border:none;outline:none;");
#else
            lineEdit()->setStyleSheet(
                "background-color:transparent;padding-left:10px;padding-bottom:3px;");
#endif
            break;
        }
        case QtSpinBoxType::Compact:
        case QtSpinBoxType::Side:
        case QtSpinBoxType::PMSide: {
            lineEdit()->setAlignment(Qt::AlignCenter);
#ifdef Q_OS_MACOS
            lineEdit()->setStyleSheet(
                "background-color:transparent;padding-bottom:3px;border:none;outline:none;");
#else
            lineEdit()->setStyleSheet("background-color:transparent;padding-bottom:3px;");
#endif
            break;
        }
    }
    d->onThemeChanged(qtTheme->getThemeMode());
    setFrame(hasFrame());
    Q_EMIT buttonModeChanged();
}

QtSpinBoxType::ButtonMode QtDoubleSpinBox::buttonMode() const
{
    Q_D(const QtDoubleSpinBox);
    return d->style->buttonMode();
}

void QtDoubleSpinBox::focusInEvent(QFocusEvent* event)
{
    Q_D(QtDoubleSpinBox);
    if(event->reason() == Qt::MouseFocusReason)
    {
        QVariantAnimation* markAnimation = new QVariantAnimation(this);
        connect(
            markAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
                d_ptr->expandMarkWidth = value.toReal();
                update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->expandMarkWidth);
        markAnimation->setEndValue(width() / (qreal) 2 - 3.0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QDoubleSpinBox::focusInEvent(event);
}

void QtDoubleSpinBox::focusOutEvent(QFocusEvent* event)
{
    Q_D(QtDoubleSpinBox);
    if(event->reason() != Qt::PopupFocusReason)
    {
        QVariantAnimation* markAnimation = new QVariantAnimation(this);
        connect(
            markAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
                d_ptr->expandMarkWidth = value.toReal();
                update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->expandMarkWidth);
        markAnimation->setEndValue(0.0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QDoubleSpinBox::focusOutEvent(event);
}

void QtDoubleSpinBox::paintEvent(QPaintEvent* event)
{
    Q_D(QtDoubleSpinBox);
    QDoubleSpinBox::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
    painter.drawRoundedRect(
        QRectF(width() / 2 - d->expandMarkWidth, height() - 2.5, d->expandMarkWidth * 2, 2.5), 2,
        2);
    painter.restore();
}

void QtDoubleSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(QtDoubleSpinBox);
    QtMenu* menu = d->createStandardContextMenu();
    if(!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addQtIconAction(QtIconType::Plus, tr("+"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addQtIconAction(QtIconType::Minus, tr("-"));
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
