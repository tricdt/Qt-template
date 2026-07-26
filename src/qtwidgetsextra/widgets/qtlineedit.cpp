#include "qtlineedit.h"
#include "qtdef.h"
#include "qtevent.h"
#include "qtmenu.h"
#include "qttheme.h"
#include "styles/qtlineeditstyle.h"
#include <qclipboard.h>
#include <qevent.h>
#include <qtmetamacros.h>
#include <qvariantanimation.h>
#include "qtapplication.h"

class QtLineEditPrivate
{
    Q_DECLARE_PUBLIC(QtLineEdit)
public:
    int borderRadius;
    bool isClearButtonEnable;
    QtLineEdit* q_ptr;
    qreal expandMarkWidth;
    QtThemeType::ThemeMode themeMode;
    QtEvent* focusEvent{ nullptr };
    qreal textSpacing{ 0.5 };
    QtLineEditPrivate(QtLineEdit* q);
    ~QtLineEditPrivate();
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtLineEditPrivate::QtLineEditPrivate(QtLineEdit* q)
    : q_ptr(q)
{
}
QtLineEditPrivate::~QtLineEditPrivate()
{
}

void QtLineEditPrivate::onThemeChanged(QtThemeType::ThemeMode themeMode)
{    Q_Q(QtLineEdit);
    this->themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::Text, QtThemeColor(themeMode, BasicText));
    palette.setColor(QPalette::PlaceholderText, this->themeMode == QtThemeType::Light ? QColor(0x00, 0x00, 0x00, 128) : QColor(0xBA, 0xBA, 0xBA));
    q->setPalette(palette);
}
QtLineEdit::QtLineEdit(QWidget* parent)
    : QLineEdit{ parent }
    , d_ptr(new QtLineEditPrivate(this))
{
    Q_D(QtLineEdit);
    setObjectName("QtLineEdit");
    setFixedHeight(35);
    d->themeMode = qtTheme->getThemeMode();
    d->borderRadius = 6;
    d->expandMarkWidth = 0.0;
    d->isClearButtonEnable = true;
    setFocusPolicy(Qt::StrongFocus);
    d->focusEvent = new QtEvent("WMWindowClicked", "onWMWindowClickedEvent", this);
    d->focusEvent->registerAndInit();
    setMouseTracking(true);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
    setFont(textFont);
    setStyle(new QtLineEditStyle(style()));
    setStyleSheet("#QtLineEdit{background-color:transparent;padding-left: 10px;}");
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
    setVisible(true);
}

QtLineEdit::~QtLineEdit()
{
    delete this->style();
}

void QtLineEdit::onWMWindowClickedEvent(QVariantMap data)
{
    Q_D(QtLineEdit);
    QtAppBarType::WMMouseActionType actionType =
        data.value("WMClickType").value<QtAppBarType::WMMouseActionType>();
    if(actionType == QtAppBarType::WMLBUTTONDOWN)
    {
        if(hasSelectedText() && hasFocus())
        {
            clearFocus();
        }
    }
    else if(actionType == QtAppBarType::WMLBUTTONUP || actionType == QtAppBarType::WMNCLBUTTONDOWN)
    {
        if(QtApplication::containsCursorToItem(this) ||
            (actionType == QtAppBarType::WMLBUTTONUP && hasSelectedText()))
        {
            return;
        }
        if(hasFocus())
        {
            clearFocus();
        }
    }
}

int QtLineEdit::borderRadius() const
{
    Q_D(const QtLineEdit);
    return d->borderRadius;
}
void QtLineEdit::setBorderRadius(int value)
{
    Q_D(QtLineEdit);
    d->borderRadius = value;
}
bool QtLineEdit::isClearButtonEnable() const
{
    Q_D(const QtLineEdit);
    return d->isClearButtonEnable;
}
void QtLineEdit::setIsClearButtonEnable(int value)
{
    Q_D(QtLineEdit);
    d->isClearButtonEnable = value;
    setClearButtonEnabled(value);
    Q_EMIT isClearButtonEnableChanged();
}

void QtLineEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(QtLineEdit);
    Q_EMIT focusIn(this->text());
    if(event->reason() == Qt::MouseFocusReason)
    {
        if(d->isClearButtonEnable)
        {
            setClearButtonEnabled(true);
        }
        QVariantAnimation* markAnimation = new QVariantAnimation(this);
        connect(
            markAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
                d_ptr->expandMarkWidth = value.toReal();
                update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->expandMarkWidth);
        markAnimation->setEndValue(width() / (qreal) 2 - d->borderRadius / (qreal) 2);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QLineEdit::focusInEvent(event);
}

void QtLineEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(QtLineEdit);
    Q_EMIT focusOut(this->text());
    if(event->reason() != Qt::PopupFocusReason)
    {
        if(d->isClearButtonEnable)
        {
            setClearButtonEnabled(false);
        }
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
        Q_EMIT wmFocusOut(text());
    }
    QLineEdit::focusOutEvent(event);
}

void QtLineEdit::paintEvent(QPaintEvent* event)
{
    Q_D(QtLineEdit);
    if(palette().color(QPalette::Text) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
    painter.drawRoundedRect(
        QRectF(width() / 2 - d->expandMarkWidth, height() - 2.5, d->expandMarkWidth * 2.0, 2.5),
        2.0, 2.0);
    painter.restore();
}

void QtLineEdit::contextMenuEvent(QContextMenuEvent* event)
{
    QtMenu* menu = new QtMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &QtLineEdit::undo);

        action =
            menu->addQtIconAction(QtIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &QtLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::KnifeKitchen, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, &QtLineEdit::cut);
    }

    action = menu->addQtIconAction(QtIconType::Copy, tr("复制"), QKeySequence::Copy);
    action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &QtLineEdit::copy);

    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &QtLineEdit::paste);
    }
#endif
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::DeleteLeft, tr("删除"));
        action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if(hasSelectedText())
            {
                int startIndex = selectionStart();
                int endIndex = selectionEnd();
                setText(text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if(!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
    connect(action, &QAction::triggered, this, &QtLineEdit::selectAll);
    menu->popup(event->globalPos());
}
