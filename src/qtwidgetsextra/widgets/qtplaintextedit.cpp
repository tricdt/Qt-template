#include "qtplaintextedit.h"
#include "qtapplication.h"
#include "qtdef.h"
#include "qtevent.h"
#include "qtscrollbar.h"
#include "qttheme.h"
#include "styles/qtplaintexteditstyle.h"
#include "widgets/qtmenu.h"

class QtPlainTextEditPrivate
{
    Q_DECLARE_PUBLIC(QtPlainTextEdit)
public:
    QtPlainTextEdit* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtPlainTextEditStyle* style{ nullptr };
    QtEvent* focusEvent{ nullptr };
    QtPlainTextEditPrivate(QtPlainTextEdit* q);
    ~QtPlainTextEditPrivate();
    void onWMWindowClickedEvent(QVariantMap data);
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtPlainTextEditPrivate::QtPlainTextEditPrivate(QtPlainTextEdit* q)
    : q_ptr(q)
{
}
QtPlainTextEditPrivate::~QtPlainTextEditPrivate()
{
}

void QtPlainTextEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(QtPlainTextEdit);
    QtAppBarType::WMMouseActionType actionType =
        data.value("WMClickType").value<QtAppBarType::WMMouseActionType>();
    if(actionType == QtAppBarType::WMLBUTTONDOWN)
    {
        if(!q->toPlainText().isEmpty() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if(actionType == QtAppBarType::WMLBUTTONUP || actionType == QtAppBarType::WMNCLBUTTONDOWN)
    {
        if(QtApplication::containsCursorToItem(q) ||
            (actionType == QtAppBarType::WMLBUTTONUP && !q->toPlainText().isEmpty()))
        {
            return;
        }
        if(q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void QtPlainTextEditPrivate::onThemeChanged(QtThemeType::ThemeMode _themeMode)
{
    Q_Q(QtPlainTextEdit);
    themeMode = _themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::Text, QtThemeColor(themeMode, BasicText));
    palette.setColor(QPalette::PlaceholderText, _themeMode == QtThemeType::Light
                                                    ? QColor(0x00, 0x00, 0x00, 128)
                                                    : QColor(0xBA, 0xBA, 0xBA));
    q->setPalette(palette);
}

QtPlainTextEdit::QtPlainTextEdit(QWidget* parent)
    : QPlainTextEdit{ parent }
    , d_ptr(new QtPlainTextEditPrivate(this))
{
    Q_D(QtPlainTextEdit);
    setObjectName("QtPlainTextEdit");
    setStyleSheet("#QtPlainTextEdit{background-color:transparent;}");
    setHorizontalScrollBar(new QtScrollBar(this));
    setVerticalScrollBar(new QtScrollBar(this));
    setMouseTracking(true);
    d->focusEvent = new QtEvent("WMWindowClicked", "onWMWindowClickedEvent", this);
    d->focusEvent->registerAndInit();

    d->style = new QtPlainTextEditStyle(style());
    setStyle(d->style);
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
}

QtPlainTextEdit::QtPlainTextEdit(const QString& text, QWidget* parent)
    : QtPlainTextEdit(parent)
{
    setPlainText(text);
}

QtPlainTextEdit::~QtPlainTextEdit()
{
    Q_D(QtPlainTextEdit);
    delete d->style;
}

void QtPlainTextEdit::onWMWindowClickedEvent(QVariantMap data)
{
    Q_D(QtPlainTextEdit);
    d->onWMWindowClickedEvent(data);
}

void QtPlainTextEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(QtPlainTextEdit);
    if(event->reason() == Qt::MouseFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->style, "expandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this,
            [=](const QVariant& value) { update(); });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->style->expandMarkWidth());
        markAnimation->setEndValue(width() / 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusInEvent(event);
}

void QtPlainTextEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(QtPlainTextEdit);
    if(event->reason() != Qt::PopupFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->style, "expandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this,
            [this](const QVariant& value) { update(); });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->style->expandMarkWidth());
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusOutEvent(event);
}

void QtPlainTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    QtMenu* menu = new QtMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::ArrowRotateLeft, "撤销", QKeySequence::Undo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isUndoAvailable() : false);
        connect(action, &QAction::triggered, this, &QtPlainTextEdit::undo);

        action = menu->addQtIconAction(QtIconType::ArrowRotateRight, "恢复", QKeySequence::Redo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isRedoAvailable() : false);
        connect(action, &QAction::triggered, this, &QtPlainTextEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::KnifeKitchen, "剪切", QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && !textCursor().selectedText().isEmpty());
        connect(action, &QAction::triggered, this, &QtPlainTextEdit::cut);
    }

    action = menu->addQtIconAction(QtIconType::Copy, "复制", QKeySequence::Copy);
    action->setEnabled(!textCursor().selectedText().isEmpty());
    connect(action, &QAction::triggered, this, &QtPlainTextEdit::copy);

    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::Paste, "粘贴", QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &QtPlainTextEdit::paste);
    }
#endif
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::DeleteLeft, "删除");
        action->setEnabled(
            !isReadOnly() && !toPlainText().isEmpty() && !textCursor().selectedText().isEmpty());
        connect(action, &QAction::triggered, this, [this](bool checked) {
            if(!textCursor().selectedText().isEmpty())
            {
                textCursor().deleteChar();
            }
        });
    }
    if(!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction("全选");
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!toPlainText().isEmpty() && !(textCursor().selectedText() == toPlainText()));
    connect(action, &QAction::triggered, this, &QtPlainTextEdit::selectAll);
    menu->popup(event->globalPos());
    this->setFocus();
}

void QtPlainTextEdit::paintEvent(QPaintEvent* event)
{
    Q_D(QtPlainTextEdit);
    if(palette().color(QPalette::Text) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QPlainTextEdit::paintEvent(event);
}
