#include "qtpasswordbox.h"
#include "qtapplication.h"
#include "qtdef.h"
#include "qtevent.h"
#include "qticon.h"
#include "qttheme.h"
#include "styles/qtlineeditstyle.h"
#include "widgets/qtmenu.h"
#include <qvariantanimation.h>

class QtPasswordBoxPrivate
{
    Q_DECLARE_PUBLIC(QtPasswordBox)
public:
    int borderRadius;
    bool isPasswordVisible;
    qreal expandMarkWidth;
    QtPasswordBox* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtEvent* focusEvent{ nullptr };
    QAction* toggleAction{ nullptr };
    qreal textSpacing{ 0.5 };
    QtPasswordBoxPrivate(QtPasswordBox* q);
    ~QtPasswordBoxPrivate();
    void onWMWindowClickedEvent(QVariantMap data);
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtPasswordBoxPrivate::QtPasswordBoxPrivate(QtPasswordBox* q)
    : q_ptr(q)
{
}
QtPasswordBoxPrivate::~QtPasswordBoxPrivate()
{
}

void QtPasswordBoxPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(QtPasswordBox);
    QtAppBarType::WMMouseActionType actionType =
        data.value("WMClickType").value<QtAppBarType::WMMouseActionType>();
    if(actionType == QtAppBarType::WMLBUTTONDOWN)
    {
        if(q->hasSelectedText() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if(actionType == QtAppBarType::WMLBUTTONUP || actionType == QtAppBarType::WMNCLBUTTONDOWN)
    {
        if(QtApplication::containsCursorToItem(q) ||
            (actionType == QtAppBarType::WMLBUTTONUP && q->hasSelectedText()))
        {
            return;
        }
        if(q->hasFocus())
        {
            q->clearFocus();
        }
    }
}
void QtPasswordBoxPrivate::onThemeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtPasswordBox);
    this->themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::Text, QtThemeColor(this->themeMode, BasicText));
    palette.setColor(QPalette::PlaceholderText, this->themeMode == QtThemeType::Light
                                                    ? QColor(0x00, 0x00, 0x00, 128)
                                                    : QColor(0xBA, 0xBA, 0xBA));
    q->setPalette(palette);
    // Update toggle icon color for new theme
    if(toggleAction)
    {
        if(isPasswordVisible)
        {
            toggleAction->setIcon(QtIcon::getInstance()->getQtIcon(
                QtIconType::EyeSlash, 22, 32, 32, QtThemeColor(this->themeMode, BasicText)));
        }
        else
        {
            toggleAction->setIcon(QtIcon::getInstance()->getQtIcon(
                QtIconType::Eye, 22, 32, 32, QtThemeColor(this->themeMode, BasicText)));
        }
    }
}

QtPasswordBox::QtPasswordBox(QWidget* parent)
    : QLineEdit{ parent }
    , d_ptr(new QtPasswordBoxPrivate(this))
{
    Q_D(QtPasswordBox);
    d->q_ptr = this;
    setObjectName("QtPasswordBox");
    setFixedHeight(35);
    d->themeMode = qtTheme->getThemeMode();
    d->borderRadius = 6;
    d->expandMarkWidth = 0.0;
    d->isPasswordVisible = false;
    setEchoMode(QLineEdit::Password);
    setFocusPolicy(Qt::StrongFocus);
    // 事件总线
    d->focusEvent = new QtEvent("WMWindowClicked", "onWMWindowClickedEvent", this);
    d->focusEvent->registerAndInit();
    setMouseTracking(true);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->textSpacing);
    setFont(textFont);
    setStyle(new QtLineEditStyle(style()));
    setStyleSheet(
        "#QtPasswordBox{background-color:transparent;padding-left: 10px;padding-right: 36px;}");
    // Toggle action for password visibility
    d->toggleAction = new QAction(this);
    d->toggleAction->setIcon(QtIcon::getInstance()->getQtIcon(
        QtIconType::Eye, 22, 32, 32, QtThemeColor(d->themeMode, BasicText)));
    addAction(d->toggleAction, QLineEdit::TrailingPosition);
    connect(d->toggleAction, &QAction::triggered, this,
        [this]() { setIsPasswordVisible(!isPasswordVisible()); });
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
    setVisible(true);
}

QtPasswordBox::~QtPasswordBox()
{
    delete this->style();
}

void QtPasswordBox::onWMWindowClickedEvent(QVariantMap data)
{
    Q_D(QtPasswordBox);
    d->onWMWindowClickedEvent(data);
}

int QtPasswordBox::borderRadius() const
{
    Q_D(const QtPasswordBox);
    return d->borderRadius;
}
void QtPasswordBox::setBorderRadius(int value)
{
    Q_D(QtPasswordBox);
    d->borderRadius = value;
    Q_EMIT borderRadiusChanged();
}
bool QtPasswordBox::isPasswordVisible() const
{
    Q_D(const QtPasswordBox);
    return d->isPasswordVisible;
}
void QtPasswordBox::setIsPasswordVisible(bool isPasswordVisible)
{
    Q_D(QtPasswordBox);
    d->isPasswordVisible = isPasswordVisible;
    if(isPasswordVisible)
    {
        setEchoMode(QLineEdit::Normal);
        d->toggleAction->setIcon(QtIcon::getInstance()->getQtIcon(
            QtIconType::EyeSlash, 22, 32, 32, QtThemeColor(d->themeMode, BasicText)));
    }
    else
    {
        setEchoMode(QLineEdit::Password);
        d->toggleAction->setIcon(QtIcon::getInstance()->getQtIcon(
            QtIconType::Eye, 22, 32, 32, QtThemeColor(d->themeMode, BasicText)));
    }
    Q_EMIT isPasswordVisibleChanged();
}

void QtPasswordBox::focusInEvent(QFocusEvent* event)
{
    Q_D(QtPasswordBox);
    Q_EMIT focusIn(this->text());
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
        markAnimation->setEndValue(width() / (qreal) 2 - d->borderRadius / (qreal) 2);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QLineEdit::focusInEvent(event);
}

void QtPasswordBox::focusOutEvent(QFocusEvent* event)
{
    Q_D(QtPasswordBox);
    Q_EMIT focusOut(this->text());
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
        Q_EMIT wmFocusOut(text());
    }
    QLineEdit::focusOutEvent(event);
}

void QtPasswordBox::paintEvent(QPaintEvent* event)
{
    Q_D(QtPasswordBox);
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
        QRectF(width() / 2 - d->expandMarkWidth, height() - 2.5, d->expandMarkWidth * 2, 2.5),
        2, 2);
    painter.restore();
}

void QtPasswordBox::contextMenuEvent(QContextMenuEvent* event)
{
    QtMenu* menu = new QtMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if(!isReadOnly())
    {
        action =
            menu->addQtIconAction(QtIconType::ArrowRotateLeft, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &QtPasswordBox::undo);

        action =
            menu->addQtIconAction(QtIconType::ArrowRotateRight, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &QtPasswordBox::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if(!isReadOnly())
    {
        action = menu->addQtIconAction(QtIconType::Paste, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &QtPasswordBox::paste);
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
    connect(action, &QAction::triggered, this, &QtPasswordBox::selectAll);
    menu->popup(event->globalPos());
}
