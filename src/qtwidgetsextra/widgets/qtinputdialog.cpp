#include "qtinputdialog.h"
#include "qtdoublespinbox.h"
#include "qtmaskwidget.h"
#include "qttheme.h"
#include "widgets/qtappbar.h"
#include "widgets/qtlineedit.h"
#include "widgets/qtpushbutton.h"
#include "widgets/qtspinbox.h"
#include "widgets/qttext.h"
#include <qdialog.h>
#include <qlineedit.h>

class QtInputDialogPrivate
{
    Q_DECLARE_PUBLIC(QtInputDialog)
public:
    QString titleText;
    QString subTitleText;
    QString labelText;
    QString textValue;
    int intValue;
    double doubleValue;
    QString okButtonText;
    QString cancelButtonText;
    QString placeholderText;
    int inputMinimumWidth;
    int inputMaximumWidth;

    QtInputDialog* q_ptr;

    qint64 currentWinID{ 0 };
    QtAppBar* appBar{ nullptr };
    QtThemeType::ThemeMode themeMode;
    QtMaskWidget* maskWidget{ nullptr };
    QtText* titleLabel{ nullptr };
    QtText* subTitleLabel{ nullptr };
    QtText* inputLabel{ nullptr };
    QtLineEdit* lineEdit{ nullptr };
    QtSpinBox* spinBox{ nullptr };
    QtDoubleSpinBox* doubleSpinBox{ nullptr };
    QTextEdit* textEdit{ nullptr };
    QVBoxLayout* contentLayout{ nullptr };
    QWidget* buttonWidget{ nullptr };
    QtPushButton* okButton{ nullptr };
    QtPushButton* cancelButton{ nullptr };
    bool isMultiLine{ false };
    bool isIntMode{ false };
    bool isDoubleMode{ false };

    QtInputDialogPrivate(QtInputDialog* q);
    ~QtInputDialogPrivate();
    void onOkButtonClicked();
    void onCancelButtonClicked();

    void doCloseAnimation(bool isAccept);
};

QtInputDialogPrivate::QtInputDialogPrivate(QtInputDialog* q)
    : q_ptr(q)
{
}
QtInputDialogPrivate::~QtInputDialogPrivate()
{
}

void QtInputDialogPrivate::onOkButtonClicked()
{
    Q_Q(QtInputDialog);
    doCloseAnimation(true);
}
void QtInputDialogPrivate::onCancelButtonClicked()
{
    Q_Q(QtInputDialog);
    doCloseAnimation(false);
}

void QtInputDialogPrivate::doCloseAnimation(bool isAccept)
{
    Q_Q(QtInputDialog);
    maskWidget->doMaskAnimation(0);
    isAccept ? q->accept() : q->reject();
}
Q_TAKEOVER_NATIVEEVENT_CPP(QtInputDialog, d_func()->appBar);
QtInputDialog::QtInputDialog(QWidget* parent)
    : QDialog{ parent }
    , d_ptr(new QtInputDialogPrivate(this))
{
    Q_D(QtInputDialog);
    d->maskWidget = new QtMaskWidget(parent);
    d->maskWidget->move(0, 0);
    d->maskWidget->setFixedSize(parent->size());
    d->maskWidget->setVisible(false);

    resize(400, 250);
    setWindowModality(Qt::ApplicationModal);

    d->appBar = new QtAppBar(this);
    d->appBar->setWindowButtonFlags(QtAppBarType::NoneButtonHint);
    d->appBar->setIsFixedSize(true);
    d->appBar->setAppBarHeight(0);
#ifdef Q_OS_WIN
    createWinId();
#endif

    d->titleText = "";
    d->subTitleText = "";
    d->labelText = "";
    d->textValue = "";
    d->intValue = 0;
    d->doubleValue = 0.0;
    d->okButtonText = "OK";
    d->cancelButtonText = "Cancel";
    d->placeholderText = "";
    d->inputMinimumWidth = 100;
    d->inputMaximumWidth = QWIDGETSIZE_MAX;

    d->titleLabel = new QtText(d->titleText, this);
    d->titleLabel->setTextStyle(QtTextType::Title);
    d->subTitleLabel = new QtText(d->subTitleText, this);
    d->subTitleLabel->setTextStyle(QtTextType::Body);

    d->inputLabel = new QtText(d->labelText, this);
    d->inputLabel->setTextStyle(QtTextType::Body);
    d->lineEdit = new QtLineEdit(this);
    d->lineEdit->setText(d->textValue);
    d->lineEdit->setPlaceholderText(d->placeholderText);
    d->lineEdit->setIsClearButtonEnable(true);

    connect(d->lineEdit, &QtLineEdit::textChanged, this, [this](const QString& text) {
        d_ptr->textValue = text;
        Q_EMIT textValueChanged(text);
        bool okInt;
        int intValue = text.toInt(&okInt);
        if(okInt)
        {
            d_ptr->intValue = intValue;
            Q_EMIT intValueChanged(intValue);
        }
        bool okDouble;
        double doubleValue = text.toDouble(&okDouble);
        if(okDouble)
        {
            d_ptr->doubleValue = doubleValue;
            Q_EMIT doubleValueChanged(doubleValue);
        }
    });

    d->cancelButton = new QtPushButton(d->cancelButtonText, this);
    connect(d->cancelButton, &QtPushButton::clicked, this,
        [this]() { d_ptr->onCancelButtonClicked(); });

    d->cancelButton->setMinimumSize(0, 0);
    d->cancelButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->cancelButton->setFixedHeight(38);
    d->cancelButton->setBorderRadius(6);

    d->okButton = new QtPushButton(d->okButtonText, this);
    connect(d->okButton, &QtPushButton::clicked, this, [this]() { d_ptr->onOkButtonClicked(); });
    d->okButton->setLightDefaultColor(QtThemeColor(QtThemeType::Light, PrimaryNormal));
    d->okButton->setLightHoverColor(QtThemeColor(QtThemeType::Light, PrimaryHover));
    d->okButton->setLightPressColor(QtThemeColor(QtThemeType::Light, PrimaryPress));
    d->okButton->setLightTextColor(Qt::white);
    d->okButton->setDarkDefaultColor(QtThemeColor(QtThemeType::Dark, PrimaryNormal));
    d->okButton->setDarkHoverColor(QtThemeColor(QtThemeType::Dark, PrimaryHover));
    d->okButton->setDarkPressColor(QtThemeColor(QtThemeType::Dark, PrimaryPress));
    d->okButton->setDarkTextColor(Qt::black);
    d->okButton->setMinimumSize(0, 0);
    d->okButton->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    d->okButton->setFixedHeight(38);
    d->okButton->setBorderRadius(6);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget* contentWidget = new QWidget(this);
    d->contentLayout = new QVBoxLayout(contentWidget);
    d->contentLayout->setContentsMargins(15, 25, 15, 15);
    d->contentLayout->addWidget(d->titleLabel);
    d->contentLayout->addSpacing(2);
    d->contentLayout->addWidget(d->subTitleLabel);
    d->contentLayout->addSpacing(15);
    d->contentLayout->addWidget(d->inputLabel);
    d->contentLayout->addSpacing(5);
    d->contentLayout->addWidget(d->lineEdit);
    d->contentLayout->addStretch();

    d->buttonWidget = new QWidget(this);
    d->buttonWidget->setFixedHeight(60);
    QHBoxLayout* buttonLayout = new QHBoxLayout(d->buttonWidget);
    buttonLayout->setContentsMargins(15, 11, 15, 11);
    buttonLayout->addWidget(d->cancelButton);
    buttonLayout->addWidget(d->okButton);

    mainLayout->addWidget(contentWidget);
    mainLayout->addWidget(d->buttonWidget);

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });

    QTimer::singleShot(0, d->lineEdit, [this]() { d_ptr->lineEdit->setFocus(); });
}

QtInputDialog::~QtInputDialog()
{
}

QString QtInputDialog::textInput(QWidget* parent, const QString& title, const QString& subtitle,
    const QString& label, const QString& text, bool* ok, const QString& okButtonText,
    const QString& cancelButtonText, int inputMinWidth, int inputMaxWidth)
{
    QtInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setTextValue(text);
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if(ok)
        *ok = accepted;

    return accepted ? dialog.textValue() : QString();
}

int QtInputDialog::intInput(QWidget* parent, const QString& title, const QString& subtitle,
    const QString& label, int value, int minValue, int maxValue, int step, bool* ok,
    const QString& okButtonText, const QString& cancelButtonText, int inputMinWidth,
    int inputMaxWidth)
{
    QtInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setIntValue(value);
    dialog.setTextValue(QString::number(value));
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();
    dialog.setIntRange(minValue, maxValue, step);

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if(ok)
        *ok = accepted;

    return accepted ? dialog.intValue() : value;
}

double QtInputDialog::doubleInput(QWidget* parent, const QString& title, const QString& subtitle,
    const QString& label, double value, double minValue, double maxValue, int decimals, bool* ok,
    const QString& okButtonText, const QString& cancelButtonText, int inputMinWidth,
    int inputMaxWidth)
{
    QtInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setDoubleValue(value);
    dialog.setTextValue(QString::number(value, 'f', decimals));
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();
    dialog.setDoubleRange(minValue, maxValue, decimals);

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if(ok)
        *ok = accepted;

    return accepted ? dialog.doubleValue() : value;
}

QString QtInputDialog::multiLineTextInput(QWidget* parent, const QString& title,
    const QString& subtitle, const QString& label, const QString& text, bool* ok,
    const QString& okButtonText, const QString& cancelButtonText, int inputMinWidth,
    int inputMaxWidth)
{
    QtInputDialog dialog(parent);
    dialog.setTitleText(title);
    dialog.setSubTitleText(subtitle);
    dialog.setLabelText(label);
    dialog.setTextValue(text);
    dialog.setOkButtonText(okButtonText);
    dialog.setCancelButtonText(cancelButtonText);
    dialog.setInputMinimumWidth(inputMinWidth);
    dialog.setInputMaximumWidth(inputMaxWidth);
    dialog.updateLabels();
    dialog.setMultiLine(true);

    bool accepted = (dialog.exec() == QDialog::Accepted);
    if(ok)
        *ok = accepted;

    return accepted ? dialog.textValue() : QString();
}

void QtInputDialog::setTextEchoMode(QLineEdit::EchoMode mode)
{
    Q_D(QtInputDialog);
    d->lineEdit->setEchoMode(mode);
}
QLineEdit::EchoMode QtInputDialog::textEchoMode() const
{
    Q_D(const QtInputDialog);
    return d->lineEdit->echoMode();
}

void QtInputDialog::setIntRange(int minValue, int maxValue, int step)
{
    Q_D(QtInputDialog);
    if(d->isIntMode)
        return;

    d->isIntMode = true;

    if(!d->spinBox)
    {
        d->spinBox = new QtSpinBox(this);
        d->spinBox->setButtonMode(QtSpinBoxType::PMSide);
        d->spinBox->setMinimumWidth(d->inputMinimumWidth);
        d->spinBox->setMaximumWidth(d->inputMaximumWidth);
        d->spinBox->setMinimum(minValue);
        d->spinBox->setMaximum(maxValue);
        d->spinBox->setSingleStep(step);
        d->spinBox->setValue(d->intValue);

        connect(d->spinBox, QOverload<int>::of(&QtSpinBox::valueChanged), this, [this](int value) {
            d_ptr->intValue = value;
            d_ptr->textValue = QString::number(value);
            Q_EMIT intValueChanged(value);
            Q_EMIT textValueChanged(d_ptr->textValue);
        });
    }

    d->lineEdit->setVisible(false);
    d->spinBox->setVisible(true);
    d->spinBox->setMinimum(minValue);
    d->spinBox->setMaximum(maxValue);
    d->spinBox->setSingleStep(step);
    d->spinBox->setValue(d->intValue);

    if(d->contentLayout)
    {
        d->contentLayout->removeWidget(d->lineEdit);
        d->contentLayout->insertWidget(6, d->spinBox);
    }
}
void QtInputDialog::setDoubleRange(double minValue, double maxValue, int decimals)
{
    Q_D(QtInputDialog);
    if(d->isDoubleMode)
        return;

    d->isDoubleMode = true;

    if(!d->doubleSpinBox)
    {
        d->doubleSpinBox = new QtDoubleSpinBox(this);
        d->doubleSpinBox->setButtonMode(QtSpinBoxType::PMSide);
        d->doubleSpinBox->setMinimumWidth(d->inputMinimumWidth);
        d->doubleSpinBox->setMaximumWidth(d->inputMaximumWidth);
        d->doubleSpinBox->setMinimum(minValue);
        d->doubleSpinBox->setMaximum(maxValue);
        d->doubleSpinBox->setDecimals(decimals);
        d->doubleSpinBox->setValue(d->doubleValue);

        connect(d->doubleSpinBox, QOverload<double>::of(&QtDoubleSpinBox::valueChanged), this,
            [this, decimals](double value) {
                d_ptr->doubleValue = value;
                d_ptr->textValue = QString::number(value, 'f', decimals);
                Q_EMIT doubleValueChanged(value);
                Q_EMIT textValueChanged(d_ptr->textValue);
            });
    }

    d->lineEdit->setVisible(false);
    d->doubleSpinBox->setVisible(true);
    d->doubleSpinBox->setMinimum(minValue);
    d->doubleSpinBox->setMaximum(maxValue);
    d->doubleSpinBox->setDecimals(decimals);
    d->doubleSpinBox->setValue(d->doubleValue);

    if(d->contentLayout)
    {
        d->contentLayout->removeWidget(d->lineEdit);
        d->contentLayout->insertWidget(6, d->doubleSpinBox);
    }
}
void QtInputDialog::setMultiLine(bool multiLine)
{
    Q_D(QtInputDialog);
    if(d->isMultiLine == multiLine)
        return;

    d->isMultiLine = multiLine;

    if(multiLine)
    {
        if(!d->textEdit)
        {
            d->textEdit = new QTextEdit(this);
            d->textEdit->setPlainText(d->textValue);
            d->textEdit->setMinimumHeight(150);
            connect(d->textEdit, &QTextEdit::textChanged, this, [this]() {
                d_ptr->textValue = d_ptr->textEdit->toPlainText();
                Q_EMIT textValueChanged(d_ptr->textValue);
            });
        }
        d->lineEdit->setVisible(false);
        d->textEdit->setVisible(true);
        d->textEdit->setPlainText(d->textValue);

        if(d->contentLayout)
        {
            d->contentLayout->removeWidget(d->lineEdit);
            d->contentLayout->insertWidget(6, d->textEdit);
        }
        resize(400, 350);
    }
    else
    {
        if(d->textEdit)
        {
            d->textEdit->setVisible(false);
        }
        d->lineEdit->setVisible(true);
        d->lineEdit->setText(d->textValue);

        if(d->contentLayout && d->textEdit)
        {
            d->contentLayout->removeWidget(d->textEdit);
            d->contentLayout->insertWidget(4, d->lineEdit);
        }
        resize(400, 250);
    }
}

void QtInputDialog::updateLabels()
{
    Q_D(QtInputDialog);
    d->titleLabel->setText(d->titleText);
    d->subTitleLabel->setText(d->subTitleText);
    d->inputLabel->setText(d->labelText);
    d->cancelButton->setText(d->cancelButtonText);
    d->okButton->setText(d->okButtonText);
}

void QtInputDialog::showEvent(QShowEvent* event)
{
    Q_D(QtInputDialog);
    if(parentWidget())
    {
        d->maskWidget->setVisible(true);
        d->maskWidget->raise();
        d->maskWidget->setFixedSize(parentWidget()->size());
        d->maskWidget->doMaskAnimation(90);
    }
    QDialog::showEvent(event);
}

void QtInputDialog::paintEvent(QPaintEvent* event)
{
    Q_D(QtInputDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, DialogBase));
    painter.drawRect(rect());
    painter.setBrush(QtThemeColor(d->themeMode, DialogLayoutArea));
    painter.drawRoundedRect(QRectF(0, height() - 60, width(), 60), 8, 8);
    painter.restore();
}

void QtInputDialog::keyPressEvent(QKeyEvent* event)
{
    Q_D(QtInputDialog);
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        d->onOkButtonClicked();
        event->accept();
        return;
    }
    else if(event->key() == Qt::Key_Escape)
    {
        d->onCancelButtonClicked();
        event->accept();
        return;
    }
    QDialog::keyPressEvent(event);
}

QString QtInputDialog::titleText() const
{
    Q_D(const QtInputDialog);
    return d->titleText;
}
void QtInputDialog::setTitleText(QString value)
{
    Q_D(QtInputDialog);
    d->titleText = value;
}
QString QtInputDialog::subTitleText() const
{
    Q_D(const QtInputDialog);
    return d->subTitleText;
}
void QtInputDialog::setSubTitleText(QString value)
{
    Q_D(QtInputDialog);
    d->subTitleText = value;
}
QString QtInputDialog::labelText() const
{
    Q_D(const QtInputDialog);
    return d->labelText;
}
void QtInputDialog::setLabelText(QString value)
{
    Q_D(QtInputDialog);
    d->labelText = value;
}
QString QtInputDialog::textValue() const
{
    Q_D(const QtInputDialog);
    return d->textValue;
}
void QtInputDialog::setTextValue(QString value)
{
    Q_D(QtInputDialog);
    d->textValue = value;
}
int QtInputDialog::intValue() const
{
    Q_D(const QtInputDialog);
    return d->intValue;
}
void QtInputDialog::setIntValue(int value)
{
    Q_D(QtInputDialog);
    d->intValue = value;
}
double QtInputDialog::doubleValue() const
{
    Q_D(const QtInputDialog);
    return d->doubleValue;
}
void QtInputDialog::setDoubleValue(double value)
{
    Q_D(QtInputDialog);
    d->doubleValue = value;
}
QString QtInputDialog::okButtonText() const
{
    Q_D(const QtInputDialog);
    return d->okButtonText;
}
void QtInputDialog::setOkButtonText(QString value)
{
    Q_D(QtInputDialog);
    d->okButtonText = value;
}
QString QtInputDialog::cancelButtonText() const
{
    Q_D(const QtInputDialog);
    return d->cancelButtonText;
}
void QtInputDialog::setCancelButtonText(QString value)
{
    Q_D(QtInputDialog);
    d->cancelButtonText = value;
}
QString QtInputDialog::placeholderText() const
{
    Q_D(const QtInputDialog);
    return d->placeholderText;
}
void QtInputDialog::setPlaceholderText(QString value)
{
    Q_D(QtInputDialog);
    d->placeholderText = value;
}
int QtInputDialog::inputMinimumWidth() const
{
    Q_D(const QtInputDialog);
    return d->inputMinimumWidth;
}
void QtInputDialog::setInputMinimumWidth(int value)
{
    Q_D(QtInputDialog);
    d->inputMinimumWidth = value;
}
int QtInputDialog::inputMaximumWidth() const
{
    Q_D(const QtInputDialog);
    return d->inputMaximumWidth;
}
void QtInputDialog::setInputMaximumWidth(int value)
{
    Q_D(QtInputDialog);
    d->inputMaximumWidth = value;
}
