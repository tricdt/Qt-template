#include "qtkeybindercontainer.h"
#include "qtkeybinder.h"
#include "qttheme.h"
#include "utils/qtkeybinderfnmonitor.h"
#include <qevent.h>
class QtKeyBinderContainerPrivate
{
    Q_DECLARE_PUBLIC(QtKeyBinderContainer)
public:
    QString binderKeyText;
    quint32 nativeVirtualBinderKey;
    QtKeyBinderContainer* q_ptr;
    QString historyBinderKeyText;
    quint32 historyNativeVirtualBinderKey{ 0 };
    QtThemeType::ThemeMode themeMode;
    QtKeyBinder* keyBinder{ nullptr };
    QtKeyBinderContainerPrivate(QtKeyBinderContainer* q);
    ~QtKeyBinderContainerPrivate();
};

QtKeyBinderContainerPrivate::QtKeyBinderContainerPrivate(QtKeyBinderContainer* q)
    : q_ptr(q)
{
}
QtKeyBinderContainerPrivate::~QtKeyBinderContainerPrivate()
{
}

QtKeyBinderContainer::QtKeyBinderContainer(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtKeyBinderContainerPrivate(this))
{
    Q_D(QtKeyBinderContainer);
    d->keyBinder = dynamic_cast<QtKeyBinder*>(parent);
    setStyleSheet("#QtKeyBinderContainer{background-color:transparent;}");
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setFixedHeight(140);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(16);
    setFont(textFont);
    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this, [this](QtThemeType::ThemeMode themeMode) {
        d_ptr->themeMode = themeMode;
        update();
    });
    qtStartFnKeyMonitor([this]() {
        if(hasFocus())
        {
            d_ptr->binderKeyText = "Globe";
            d_ptr->nativeVirtualBinderKey = 0x3F;
            update();
        }
    });
}

QtKeyBinderContainer::~QtKeyBinderContainer()
{
    qtStopFnKeyMonitor();
}

void QtKeyBinderContainer::logOrResetHistoryData(bool isLog)
{
    Q_D(QtKeyBinderContainer);
    if(isLog)
    {
        d->historyBinderKeyText = d->binderKeyText;
        d->historyNativeVirtualBinderKey = d->nativeVirtualBinderKey;
    }
    else
    {
        d->binderKeyText = d->historyBinderKeyText;
        d->nativeVirtualBinderKey = d->historyNativeVirtualBinderKey;
        update();
    }
}
void QtKeyBinderContainer::saveBinderChanged()
{
    Q_D(QtKeyBinderContainer);
    Q_EMIT d->keyBinder->binderKeyTextChanged(d->binderKeyText);
    Q_EMIT d->keyBinder->nativeVirtualBinderKeyChanged(d->nativeVirtualBinderKey);
    if(d->binderKeyText.isEmpty())
    {
        d->keyBinder->setText(QStringLiteral("  Key: Binder      "));
    }
    else
    {
        d->keyBinder->setText(QStringLiteral("  Key: %1      ").arg(d->binderKeyText));
    }
}

bool QtKeyBinderContainer::event(QEvent* event)
{
    Q_D(QtKeyBinderContainer);
    switch(event->type())
    {
        case QEvent::KeyPress: {
            QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
            if(!keyEvent->isAutoRepeat())
            {
                switch(keyEvent->key())
                {
                    case Qt::Key_Shift: {
                        d->binderKeyText = "Shift";
                        break;
                    }
                    case Qt::Key_Control: {
                        d->binderKeyText = "Ctrl";
                        break;
                    }
                    case Qt::Key_Alt: {
#ifdef Q_OS_MACOS
                        d->binderKeyText = "Option";
#else
                        d->binderKeyText = "Alt";
#endif
                        break;
                    }
                    case Qt::Key_Meta: {
#ifdef Q_OS_MACOS
                        d->binderKeyText = "Cmd";
#else
                        d->binderKeyText = "Win";
#endif
                        break;
                    }
                    default: {
                        d->binderKeyText = QKeySequence(keyEvent->key()).toString();
                        break;
                    }
                }
                d->nativeVirtualBinderKey = keyEvent->nativeVirtualKey();
                update();
            }
            event->accept();
            return true;
        }
        default: {
            break;
        }
    }
    return QWidget::event(event);
}

void QtKeyBinderContainer::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtKeyBinderContainer);
    switch(event->button())
    {
        case Qt::LeftButton: {
            d->binderKeyText = "Mouse1";
            d->nativeVirtualBinderKey = 0x01;
            break;
        }
        case Qt::RightButton: {
            d->binderKeyText = "Mouse2";
            d->nativeVirtualBinderKey = 0x02;
            break;
        }
        case Qt::MiddleButton: {
            d->binderKeyText = "Middle";
            d->nativeVirtualBinderKey = 0x04;
            break;
        }
        case Qt::BackButton: {
            d->binderKeyText = "Back";
            d->nativeVirtualBinderKey = 0x05;
            break;
        }
        case Qt::ForwardButton: {
            d->binderKeyText = "Forward";
            d->nativeVirtualBinderKey = 0x06;
            break;
        }
        default: {
            break;
        }
    }
    QWidget::mousePressEvent(event);
    update();
}

void QtKeyBinderContainer::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    setFocus();
}

void QtKeyBinderContainer::paintEvent(QPaintEvent* event)
{
    Q_D(QtKeyBinderContainer);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(QtThemeColor(d->themeMode, BasicText));
    painter.drawText(
        QRect(20, 20, width(), 50), Qt::AlignLeft | Qt::AlignTop, u8"Press any key to bind");
    if(d->binderKeyText.isEmpty())
    {
        painter.restore();
        return;
    }
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(QtThemeColor(d->themeMode, PrimaryNormal));
    qreal textWidth = fontMetrics().horizontalAdvance(d->binderKeyText) * 1.3;
    qreal textHeight = fontMetrics().height() * 1.2;
    textWidth = qMax(textWidth, 35.0);
    textHeight = qMax(textHeight, 35.0);
    painter.drawRoundedRect(
        QRectF((width() - textWidth) / 2.0, rect().center().y() + 10, textWidth, textHeight), 5, 5);
    painter.setPen(QtThemeColor(d->themeMode, BasicTextInvert));
    painter.drawText(
        QRectF((width() - textWidth) / 2.0, rect().center().y() + 10, textWidth, textHeight),
        Qt::AlignCenter, d->binderKeyText);
    painter.restore();
}

QString QtKeyBinderContainer::binderKeyText() const
{
    Q_D(const QtKeyBinderContainer);
    return d->binderKeyText;
}
void QtKeyBinderContainer::setBinderKeyText(QString value)
{
    Q_D(QtKeyBinderContainer);
    d->binderKeyText = value;
}
quint32 QtKeyBinderContainer::nativeVirtualBinderKey() const
{
    Q_D(const QtKeyBinderContainer);
    return d->nativeVirtualBinderKey;
}
void QtKeyBinderContainer::setNativeVirtualBinderKey(quint32 value)
{
    Q_D(QtKeyBinderContainer);
    d->nativeVirtualBinderKey = value;
}