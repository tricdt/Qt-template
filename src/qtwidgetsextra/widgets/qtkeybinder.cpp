#include "qtkeybinder.h"
#include "qtcontentdialog.h"
#include "qtdef.h"
#include "qtkeybindercontainer.h"
#include "qttheme.h"
#include <qcoreevent.h>
#include <qevent.h>
#include <qlabel.h>
class QtKeyBinderPrivate
{
    Q_DECLARE_PUBLIC(QtKeyBinder)
public:
    int borderRadius;
    QtKeyBinder* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtContentDialog* binderDialog{ nullptr };
    QtKeyBinderContainer* binderContainer{ nullptr };
    QtKeyBinderPrivate(QtKeyBinder* q);
    ~QtKeyBinderPrivate();
    void onThemeChanged(QtThemeType::ThemeMode themeMode);
};

QtKeyBinderPrivate::QtKeyBinderPrivate(QtKeyBinder* q)
    : q_ptr(q)
{
}
QtKeyBinderPrivate::~QtKeyBinderPrivate()
{
}

void QtKeyBinderPrivate::onThemeChanged(QtThemeType::ThemeMode themeMode)
{
    Q_Q(QtKeyBinder);
    this->themeMode = themeMode;
    QPalette palette = q->palette();
    palette.setColor(QPalette::Text, QtThemeColor(this->themeMode, BasicText));
    palette.setColor(QPalette::WindowText, QtThemeColor(this->themeMode, BasicText));
    q->setPalette(palette);
}

QtKeyBinder::QtKeyBinder(QWidget* parent)
    : QLabel{ parent }
    , d_ptr(new QtKeyBinderPrivate(this))
{
    Q_D(QtKeyBinder);
    d->borderRadius = 5;
    setFixedHeight(35);
    setMouseTracking(true);
    setStyleSheet("#QtKeyBinder{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(15);
    setFont(textFont);
    d->binderContainer = new QtKeyBinderContainer(this);
    setText(QStringLiteral("  Key: ") + QStringLiteral("Binder") + QStringLiteral("      "));
    d->binderDialog = new QtContentDialog(window());
    d->binderDialog->setCentralWidget(d->binderContainer);
    d->binderDialog->setLeftButtonText(u8"Cancel");
    d->binderDialog->setMiddleButtonText(u8"Reset");
    d->binderDialog->setRightButtonText(u8"Confirm");
    connect(d->binderDialog, &QtContentDialog::middleButtonClicked, this,
        [this]() { d_ptr->binderContainer->logOrResetHistoryData(false); });
    connect(d->binderDialog, &QtContentDialog::rightButtonClicked, this,
        [this]() { d_ptr->binderContainer->saveBinderChanged(); });
    d->onThemeChanged(qtTheme->getThemeMode());
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->onThemeChanged(themeMode); });
}

QtKeyBinder::~QtKeyBinder()
{
}

bool QtKeyBinder::event(QEvent* event)
{
    switch(event->type())
    {
        case QEvent::Enter:
        case QEvent::Leave: {
            update();
            break;
        }
        default: {
            break;
        }
    }
    return QLabel::event(event);
}

void QtKeyBinder::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtKeyBinder);
    if(event->button() == Qt::LeftButton)
    {
        d->binderDialog->show();
        d->binderContainer->setFocus();
        d->binderContainer->logOrResetHistoryData(true);
    }
    QLabel::mouseReleaseEvent(event);
}

void QtKeyBinder::paintEvent(QPaintEvent* event)
{
    Q_D(QtKeyBinder);
    if(palette().color(QPalette::WindowText) != QtThemeColor(d->themeMode, BasicText))
    {
        d->onThemeChanged(d->themeMode);
    }
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(QtThemeColor(d->themeMode, BasicBorder));
    painter.setBrush(underMouse() ? QtThemeColor(d->themeMode, BasicHover)
                                  : QtThemeColor(d->themeMode, BasicBase));
    QRect borderRect = rect();
    borderRect.adjust(1, 1, -1, -1);
    painter.drawRoundedRect(borderRect, d->borderRadius, d->borderRadius);
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(16);
    painter.setFont(iconFont);
    painter.setPen(QtThemeColor(d->themeMode, BasicText));
    QRect iconRect = rect();
    iconRect.adjust(0, 0, -10, 0);
    painter.drawText(
        iconRect, Qt::AlignVCenter | Qt::AlignRight, QChar((unsigned short) QtIconType::Pencil));
    painter.restore();
    QLabel::paintEvent(event);
}

int QtKeyBinder::borderRadius() const
{
    Q_D(const QtKeyBinder);
    return d->borderRadius;
}
void QtKeyBinder::setBorderRadius(int value)
{
    Q_D(QtKeyBinder);
    d->borderRadius = value;
}
QString QtKeyBinder::binderKeyText() const
{
    Q_D(const QtKeyBinder);
    return d->binderContainer->binderKeyText();
}
void QtKeyBinder::setBinderKeyText(QString value)
{
    Q_D(QtKeyBinder);
    d->binderContainer->setBinderKeyText(value);
}
quint32 QtKeyBinder::nativeVirtualBinderKey() const
{
    Q_D(const QtKeyBinder);
    return d->binderContainer->nativeVirtualBinderKey();
}
void QtKeyBinder::setNativeVirtualBinderKey(quint32 value)
{
    Q_D(QtKeyBinder);
    d->binderContainer->setNativeVirtualBinderKey(value);
}
