#include "qtcolordialog.h"
#include "itemviews/delegates/qtcolordisplaydelegate.h"
#include "itemviews/models/qtcolordisplaymodel.h"
#include "itemviews/views/qtbaselistview.h"
#include "qtcolorpicker.h"
#include "qtcolorpreview.h"
#include "qtcombobox.h"
#include "qtlineedit.h"
#include "qtpushbutton.h"
#include "qttheme.h"
#include "styles/qtcolorvaluesliderstyle.h"
#include "utils/qtintvalidator.h"
#include "widgets/qttext.h"
#include "widgets/qttoolbutton.h"

class QtColorDialogPrivate
{
    Q_DECLARE_PUBLIC(QtColorDialog)
public:
    QColor currentColor;
    QtColorDialog* q_ptr;
    QtThemeType::ThemeMode themeMode;
    QtAppBar* appBar{ nullptr };
    QtColorPicker* colorPicker{ nullptr };
    QtColorPreview* colorPreview{ nullptr };
    QSlider* colorValueSlider{ nullptr };
    QtColorValueSliderStyle* colorValueSliderStyle{ nullptr };
    QtLineEdit* htmlEdit{ nullptr };
    QtComboBox* modeComboBox{ nullptr };
    QtLineEdit* firstEdit{ nullptr };
    QtLineEdit* secondEdit{ nullptr };
    QtLineEdit* thridEdit{ nullptr };
    QtText* firstText{ nullptr };
    QtText* secondText{ nullptr };
    QtText* thridText{ nullptr };

    QtBaseListView* basicColorView{ nullptr };
    QtColorDisplayModel* basicColorModel{ nullptr };
    QtColorDisplayDelegate* basicColorDelegate{ nullptr };

    QtToolButton* addCustomColorButton{ nullptr };
    QtToolButton* removeCustomColorButton{ nullptr };
    QtBaseListView* customColorView{ nullptr };
    QtColorDisplayModel* customColorModel{ nullptr };
    QtColorDisplayDelegate* customColorDelegate{ nullptr };

    QtPushButton* overButton{ nullptr };
    QtPushButton* cancelButton{ nullptr };

    QtColorDialogPrivate(QtColorDialog* q);
    ~QtColorDialogPrivate();
    void onColorPickerColorChanged(QColor selectedColor);
    void onColorValueSliderChanged(int value);

    void onColorModeChanged(int index);
    void onHtmlEditFocusOut(QString text);
    void onHtmlEditChanged(const QString& text);
    void onColorEditChanged(const QString& text);

    void onBasicColorViewClicked(const QModelIndex& index);
    void onCustomColorViewClicked(const QModelIndex& index);

    void onAddCustomColorButtonClicked();
    void onRemoveCustomColorButtonClicked();
    void initBasicColor();
    void initCustomColor();
    void updateHtmlEditValue();
    void updateEditValue();
    void updateColorPreview();
    void updateColorValueSlider();
    QString completeColorText(QString text) const;
    QString hexRgbValue() const;
    QColor colorFromEdit() const;
};

QtColorDialogPrivate::QtColorDialogPrivate(QtColorDialog* q)
    : q_ptr(q)
{
}
QtColorDialogPrivate::~QtColorDialogPrivate()
{
}

void QtColorDialogPrivate::onColorPickerColorChanged(QColor selectedColor)
{
    Q_Q(QtColorDialog);
    QColor valueColor = selectedColor.toHsv();
    valueColor.setHsvF(
        valueColor.hueF(), valueColor.saturationF(), colorValueSlider->value() / 255.0);
    currentColor = valueColor;
    updateHtmlEditValue();
    updateEditValue();
    updateColorPreview();
    updateColorValueSlider();
    Q_EMIT q->currentColorChanged();
}
void QtColorDialogPrivate::onColorValueSliderChanged(int value)
{
    Q_Q(QtColorDialog);
    QColor baseColor = currentColor;
    baseColor.setHsvF(baseColor.hueF(), baseColor.saturationF(), colorValueSlider->value() / 255.0);
    q->setCurrentColor(baseColor);
}
void QtColorDialogPrivate::onColorModeChanged(int index)
{
    Q_Q(QtColorDialog);
    if(index == 0)
    {
        firstText->setText("R");
        secondText->setText("G");
        thridText->setText("B");
        firstEdit->setValidator(new QtIntValidator(0, 255, q));
    }
    else if(index == 1)
    {
        firstText->setText("H");
        secondText->setText("S");
        thridText->setText("V");
        firstEdit->setValidator(new QtIntValidator(0, 359, q));
    }
    updateEditValue();
}
void QtColorDialogPrivate::onHtmlEditFocusOut(QString text)
{
    Q_Q(QtColorDialog);
    if(text == "#")
    {
        htmlEdit->setText("#000000");
    }
    else if(text.length() > 1 && text.length() < 7)
    {
        htmlEdit->setText(completeColorText(text));
    }
}
void QtColorDialogPrivate::onHtmlEditChanged(const QString& text)
{
    Q_Q(QtColorDialog);
    if(text.length() > 1)
    {
        q->setCurrentColor(QColor(completeColorText(text)));
    }
}
void QtColorDialogPrivate::onColorEditChanged(const QString& text)
{
    Q_Q(QtColorDialog);
    q->setCurrentColor(colorFromEdit());
}
void QtColorDialogPrivate::onBasicColorViewClicked(const QModelIndex& index)
{
    Q_Q(QtColorDialog);
    QColor basicColor = index.data(Qt::UserRole).value<QColor>();
    q->setCurrentColor(basicColor);
}
void QtColorDialogPrivate::onCustomColorViewClicked(const QModelIndex& index)
{
    Q_Q(QtColorDialog);
    QColor color = index.data(Qt::UserRole).value<QColor>();
    if(color.isValid())
    {
        q->setCurrentColor(color);
    }
}
void QtColorDialogPrivate::onAddCustomColorButtonClicked()
{
    Q_Q(QtColorDialog);
    QModelIndexList selectedIndexs = customColorView->selectionModel()->selectedIndexes();
    if(selectedIndexs.count() > 0)
    {
        customColorModel->replaceDisplayColor(currentColor, selectedIndexs[0].row());
    }
}
void QtColorDialogPrivate::onRemoveCustomColorButtonClicked()
{
    Q_Q(QtColorDialog);
    QModelIndexList selectedIndexs = customColorView->selectionModel()->selectedIndexes();
    if(selectedIndexs.count() > 0)
    {
        customColorModel->replaceDisplayColor(QColor(), selectedIndexs[0].row());
    }
}

void QtColorDialogPrivate::initBasicColor()
{
    Q_Q(QtColorDialog);
    QList<QColor> basicColorList;
    basicColorList
        << QColor(0xF0, 0x87, 0x84) << QColor(0xEB, 0x33, 0x24) << QColor(0x77, 0x43, 0x42)
        << QColor(0x8E, 0x40, 0x3A) << QColor(0x3A, 0x06, 0x03) << QColor(0x9F, 0xFC, 0xFD)
        << QColor(0x73, 0xFB, 0xFD) << QColor(0x32, 0x82, 0xF6) << QColor(0x00, 0x23, 0xF5)
        << QColor(0x00, 0x12, 0x9A) << QColor(0x16, 0x41, 0x7C) << QColor(0x00, 0x0C, 0x7B)
        << QColor(0xFF, 0xFE, 0x91) << QColor(0xFF, 0xFD, 0x55) << QColor(0xF0, 0x9B, 0x59)
        << QColor(0xF0, 0x86, 0x50) << QColor(0x78, 0x43, 0x15) << QColor(0x81, 0x7F, 0x26)
        << QColor(0x7E, 0x84, 0xF7) << QColor(0x73, 0x2B, 0xF5) << QColor(0x35, 0x80, 0xBB)
        << QColor(0x00, 0x02, 0x3D) << QColor(0x58, 0x13, 0x5E) << QColor(0x3A, 0x08, 0x3E)
        << QColor(0xA1, 0xFB, 0x8E) << QColor(0xA1, 0xFA, 0x4F) << QColor(0x75, 0xF9, 0x4D)
        << QColor(0x75, 0xFA, 0x61) << QColor(0x75, 0xFA, 0x8D) << QColor(0x81, 0x80, 0x49)
        << QColor(0xEF, 0x88, 0xBE) << QColor(0xEE, 0x8A, 0xF8) << QColor(0xEA, 0x3F, 0xF7)
        << QColor(0xEA, 0x36, 0x80) << QColor(0x7F, 0x82, 0xBB) << QColor(0x75, 0x16, 0x3F)
        << QColor(0x37, 0x7D, 0x22) << QColor(0x37, 0x7E, 0x47) << QColor(0x36, 0x7E, 0x7F)
        << QColor(0x50, 0x7F, 0x80) << QColor(0x18, 0x3E, 0x0C) << QColor(0x17, 0x3F, 0x3F)
        << QColor(0x74, 0x1B, 0x7C) << QColor(0x39, 0x10, 0x7B) << QColor(0x00, 0x00, 0x00)
        << QColor(0x80, 0x80, 0x80) << QColor(0xC0, 0xC0, 0xC0) << QColor(0xFF, 0xFF, 0xFF);
    basicColorModel->appendDisplayColor(basicColorList);
}
void QtColorDialogPrivate::initCustomColor()
{
    Q_Q(QtColorDialog);
    QList<QColor> customColorList;
    for(int i = 0; i < 24; i++)
    {
        customColorList.append(QColor());
    }
    customColorModel->appendDisplayColor(customColorList);
}
void QtColorDialogPrivate::updateHtmlEditValue()
{
    Q_Q(QtColorDialog);
    if(!htmlEdit->hasFocus())
    {
        htmlEdit->setText(hexRgbValue());
    }
}
void QtColorDialogPrivate::updateEditValue()
{
    Q_Q(QtColorDialog);
    if(modeComboBox->currentIndex() == 0)
    {
        firstEdit->setText(QString::number(currentColor.red()));
        secondEdit->setText(QString::number(currentColor.green()));
        thridEdit->setText(QString::number(currentColor.blue()));
    }
    else
    {
        QColor hsvColor = currentColor.toHsv();
        int hue = hsvColor.hue();
        if(hue < 0)
        {
            hue = 0;
        }
        firstEdit->setText(QString::number(hue));
        secondEdit->setText(QString::number(hsvColor.saturation()));
        thridEdit->setText(QString::number(hsvColor.value()));
    }
}
void QtColorDialogPrivate::updateColorPreview()
{
    Q_Q(QtColorDialog);
    colorPreview->setBaseColor(currentColor);
    colorPreview->update();
}
void QtColorDialogPrivate::updateColorValueSlider()
{
    Q_Q(QtColorDialog);
    colorValueSlider->blockSignals(true);
    colorValueSlider->setValue(currentColor.value());
    colorValueSlider->blockSignals(false);
    colorValueSliderStyle->setBaseColor(currentColor);
    colorValueSlider->update();
}
QString QtColorDialogPrivate::completeColorText(QString text) const
{
    Q_Q(const QtColorDialog);
    text.remove("#");
    while(text.length() < 6)
    {
        text.prepend("0");
    }
    text.prepend("#");
    return text;
}
QString QtColorDialogPrivate::hexRgbValue() const
{
    Q_Q(const QtColorDialog);
    QString colorHex;
    QString red = QString::number(currentColor.red(), 16);
    if(red.length() < 2)
    {
        red.prepend("0");
    }
    QString green = QString::number(currentColor.green(), 16);
    if(green.length() < 2)
    {
        green.prepend("0");
    }
    QString blue = QString::number(currentColor.blue(), 16);
    if(blue.length() < 2)
    {
        blue.prepend("0");
    }
    colorHex = red + green + blue;
    colorHex = colorHex.toUpper();
    colorHex.prepend("#");
    return colorHex;
}
QColor QtColorDialogPrivate::colorFromEdit() const
{
    Q_Q(const QtColorDialog);
    QColor editColor;
    if(modeComboBox->currentIndex() == 0)
    {
        // RGB
        editColor.setRgb(
            firstEdit->text().toUInt(), secondEdit->text().toUInt(), thridEdit->text().toUInt());
    }
    else
    {
        // HSV
        editColor.setHsv(
            firstEdit->text().toUInt(), secondEdit->text().toUInt(), thridEdit->text().toUInt());
    }
    return editColor;
}

Q_TAKEOVER_NATIVEEVENT_CPP(QtColorDialog, d_func()->appBar);
QtColorDialog::QtColorDialog(QWidget* parent)
    : QDialog{ parent }
    , d_ptr(new QtColorDialogPrivate(this))
{
    Q_D(QtColorDialog);
    d->q_ptr = this;
    setFixedSize(600, 600); // 默认宽高
    setObjectName("QtColorDialog");
    setWindowTitle("QtColorDialog");
    setWindowModality(Qt::ApplicationModal);

    d->appBar = new QtAppBar(this);
    d->appBar->setAppBarHeight(30);
    d->appBar->setIsFixedSize(true);
    d->appBar->setIsStayTop(true);
    d->appBar->setWindowButtonFlags(QtAppBarType::CloseButtonHint);
    d->appBar->setIsDefaultClosed(false);
    connect(d->appBar, &QtAppBar::closeButtonClicked, this, [this]() { close(); });

    QtText* colorPickerText = new QtText("Edit Colors", this);
    colorPickerText->setTextPixelSize(17);
    QHBoxLayout* colorPickerTextLayout = new QHBoxLayout();
    colorPickerTextLayout->setContentsMargins(0, 0, 0, 0);
    colorPickerTextLayout->addSpacing(3);
    colorPickerTextLayout->addWidget(colorPickerText);

    d->colorPicker = new QtColorPicker(this);
    connect(d->colorPicker, &QtColorPicker::selectedColorChanged, this,
        [this](QColor selectedColor) { d_ptr->onColorPickerColorChanged(selectedColor); });

    QVBoxLayout* colorPickerLayout = new QVBoxLayout();
    colorPickerLayout->setContentsMargins(0, 0, 0, 0);
    colorPickerLayout->addWidget(d->colorPicker);
    colorPickerLayout->addStretch();

    d->colorPreview = new QtColorPreview(this);
    QVBoxLayout* colorPreviewLayout = new QVBoxLayout();
    colorPreviewLayout->setContentsMargins(0, 0, 0, 0);
    colorPreviewLayout->addWidget(d->colorPreview);
    colorPreviewLayout->addStretch();

    d->colorValueSlider = new QSlider(this);
    d->colorValueSlider->setFixedHeight(257);
    d->colorValueSlider->setOrientation(Qt::Vertical);
    d->colorValueSlider->setRange(0, 255);
    d->colorValueSlider->setValue(255);
    d->colorValueSliderStyle = new QtColorValueSliderStyle();
    d->colorValueSlider->setStyle(d->colorValueSliderStyle);
    connect(d->colorValueSlider, &QSlider::valueChanged, this,
        [this](int value) { d_ptr->onColorValueSliderChanged(value); });
    QVBoxLayout* colorValueSliderLayout = new QVBoxLayout();
    colorValueSliderLayout->setContentsMargins(0, 0, 0, 0);
    colorValueSliderLayout->addWidget(d->colorValueSlider);
    colorValueSliderLayout->addStretch();

    d->htmlEdit = new QtLineEdit(this);
    d->htmlEdit->setText("#000000");
    QtIntValidator* htmlValidator = new QtIntValidator(0x000000, 0xFFFFFF, this);
    htmlValidator->setIsHexMode(true);
    d->htmlEdit->setValidator(htmlValidator);
    d->htmlEdit->setFixedSize(120, 33);
    connect(d->htmlEdit, &QtLineEdit::focusOut, this,
        [this](QString text) { d_ptr->onHtmlEditFocusOut(text); });
    connect(d->htmlEdit, &QtLineEdit::textEdited, this,
        [this](const QString& text) { d_ptr->onHtmlEditChanged(text); });

    QHBoxLayout* htmlEditLayout = new QHBoxLayout();
    htmlEditLayout->setContentsMargins(0, 0, 0, 0);
    htmlEditLayout->addSpacing(6);
    htmlEditLayout->addWidget(d->htmlEdit);
    htmlEditLayout->addStretch();
    d->modeComboBox = new QtComboBox(this);
    d->modeComboBox->setFixedSize(132, 33);
    d->modeComboBox->addItem("RGB");
    d->modeComboBox->addItem("HSV");
    d->modeComboBox->setCurrentIndex(0);
    connect(d->modeComboBox, &QtComboBox::currentIndexChanged, this,
        [this](int index) { d_ptr->onColorModeChanged(index); });

    d->firstEdit = new QtLineEdit(this);
    d->firstEdit->setText("0");
    d->firstEdit->setFixedSize(120, 33);
    d->firstEdit->setValidator(new QtIntValidator(0, 255, this));
    d->firstText = new QtText("R", this);
    d->firstText->setTextPixelSize(13);
    connect(d->firstEdit, &QtLineEdit::textEdited, this,
        [this](const QString& text) { d_ptr->onColorEditChanged(text); });

    QHBoxLayout* firstEditLayout = new QHBoxLayout();
    firstEditLayout->setContentsMargins(0, 0, 0, 0);
    firstEditLayout->addSpacing(6);
    firstEditLayout->addWidget(d->firstEdit);
    firstEditLayout->addWidget(d->firstText);
    firstEditLayout->addStretch();

    d->secondEdit = new QtLineEdit(this);
    d->secondEdit->setText("0");
    d->secondEdit->setFixedSize(120, 33);
    d->secondEdit->setValidator(new QtIntValidator(0, 255, this));
    d->secondText = new QtText("G", this);
    d->secondText->setTextPixelSize(13);
    connect(d->secondEdit, &QtLineEdit::textEdited, this,
        [this](const QString& text) { d_ptr->onColorEditChanged(text); });

    QHBoxLayout* secondEditLayout = new QHBoxLayout();
    secondEditLayout->setContentsMargins(0, 0, 0, 0);
    secondEditLayout->addSpacing(6);
    secondEditLayout->addWidget(d->secondEdit);
    secondEditLayout->addWidget(d->secondText);
    secondEditLayout->addStretch();

    d->thridEdit = new QtLineEdit(this);
    d->thridEdit->setText("0");
    d->thridEdit->setFixedSize(120, 33);
    d->thridEdit->setValidator(new QtIntValidator(0, 255, this));
    d->thridText = new QtText("B", this);
    d->thridText->setTextPixelSize(13);
    connect(d->secondEdit, &QtLineEdit::textEdited, this,
        [this](const QString& text) { d_ptr->onColorEditChanged(text); });

    QHBoxLayout* thridEditLayout = new QHBoxLayout();
    thridEditLayout->setContentsMargins(0, 0, 0, 0);
    thridEditLayout->addSpacing(6);
    thridEditLayout->addWidget(d->thridEdit);
    thridEditLayout->addWidget(d->thridText);
    thridEditLayout->addStretch();

    QVBoxLayout* paramControlLayout = new QVBoxLayout();
    paramControlLayout->setContentsMargins(0, 0, 0, 0);
    paramControlLayout->setSpacing(10);
    paramControlLayout->addLayout(htmlEditLayout);
    paramControlLayout->addWidget(d->modeComboBox);
    paramControlLayout->addLayout(firstEditLayout);
    paramControlLayout->addLayout(secondEditLayout);
    paramControlLayout->addLayout(thridEditLayout);
    paramControlLayout->addStretch();

    QHBoxLayout* colorControlLayout = new QHBoxLayout();
    colorControlLayout->setSpacing(0);
    colorControlLayout->setContentsMargins(0, 0, 0, 0);
    colorControlLayout->addLayout(colorPickerLayout);
    colorControlLayout->addSpacing(5);
    colorControlLayout->addLayout(colorPreviewLayout);
    colorControlLayout->addSpacing(20);
    colorControlLayout->addLayout(colorValueSliderLayout);
    colorControlLayout->addSpacing(10);
    colorControlLayout->addLayout(paramControlLayout);
    colorControlLayout->addStretch();

    d->basicColorView = new QtBaseListView(this);
    d->basicColorView->setFixedSize(380, 170);
    d->basicColorView->setSelectionMode(QListView::NoSelection);
    d->basicColorView->setFlow(QListView::LeftToRight);
    d->basicColorView->setViewMode(QListView::IconMode);
    d->basicColorView->setResizeMode(QListView::Adjust);
    d->basicColorModel = new QtColorDisplayModel(this);
    d->basicColorView->setModel(d->basicColorModel);
    d->basicColorDelegate = new QtColorDisplayDelegate(this);
    d->basicColorView->setItemDelegate(d->basicColorDelegate);
    d->initBasicColor();
    connect(d->basicColorView, &QtBaseListView::clicked, this,
        [this](const QModelIndex& index) { d_ptr->onBasicColorViewClicked(index); });

    QtText* basicColorViewText = new QtText("Basic Colors", this);
    basicColorViewText->setTextPixelSize(14);
    QVBoxLayout* basicColorLayout = new QVBoxLayout();
    basicColorLayout->setContentsMargins(3, 0, 0, 0);
    basicColorLayout->addWidget(basicColorViewText);
    basicColorLayout->addSpacing(15);
    basicColorLayout->addWidget(d->basicColorView);
    basicColorLayout->addStretch();

    d->customColorView = new QtBaseListView(this);
    d->customColorView->setFixedSize(200, 170);
    d->customColorView->setSelectionMode(QListView::SingleSelection);
    d->customColorView->setFlow(QListView::LeftToRight);
    d->customColorView->setViewMode(QListView::IconMode);
    d->customColorView->setResizeMode(QListView::Adjust);
    d->customColorModel = new QtColorDisplayModel(this);
    d->customColorView->setModel(d->customColorModel);
    d->customColorDelegate = new QtColorDisplayDelegate(this);
    d->customColorView->setItemDelegate(d->customColorDelegate);
    d->initCustomColor();
    connect(d->customColorView, &QtBaseListView::clicked, this,
        [this](const QModelIndex& index) { d_ptr->onCustomColorViewClicked(index); });

    d->addCustomColorButton = new QtToolButton(this);
    d->addCustomColorButton->setIsTransparent(false);
    d->addCustomColorButton->setQtIcon(QtIconType::Plus);
    connect(d->addCustomColorButton, &QtToolButton::clicked, this,
        [this]() { d_ptr->onAddCustomColorButtonClicked(); });

    d->removeCustomColorButton = new QtToolButton(this);
    d->removeCustomColorButton->setIsTransparent(false);
    d->removeCustomColorButton->setQtIcon(QtIconType::Minus);
    connect(d->removeCustomColorButton, &QtToolButton::clicked, this,
        [this]() { d_ptr->onRemoveCustomColorButtonClicked(); });

    QtText* customColorViewText = new QtText("Custom Colors", this);
    customColorViewText->setTextPixelSize(14);
    QHBoxLayout* customButtonLayout = new QHBoxLayout();
    customButtonLayout->setContentsMargins(0, 0, 0, 0);
    customButtonLayout->addWidget(customColorViewText);
    customButtonLayout->addSpacing(35);
    customButtonLayout->addWidget(d->addCustomColorButton);
    customButtonLayout->addWidget(d->removeCustomColorButton);
    customButtonLayout->addStretch();

    QVBoxLayout* customColorLayout = new QVBoxLayout();
    customColorLayout->setContentsMargins(3, 0, 0, 0);
    customColorLayout->addLayout(customButtonLayout);
    customColorLayout->addSpacing(5);
    customColorLayout->addWidget(d->customColorView);
    customColorLayout->addStretch();

    QHBoxLayout* colorDisplayLayout = new QHBoxLayout();
    colorDisplayLayout->setContentsMargins(0, 0, 0, 0);
    colorDisplayLayout->addLayout(basicColorLayout);
    colorDisplayLayout->addLayout(customColorLayout);
    colorDisplayLayout->addStretch();

    d->overButton = new QtPushButton("OK", this);
    d->overButton->setBorderRadius(6);
    d->overButton->setLightDefaultColor(QtThemeColor(QtThemeType::Light, PrimaryNormal));
    d->overButton->setLightHoverColor(QtThemeColor(QtThemeType::Light, PrimaryHover));
    d->overButton->setLightPressColor(QtThemeColor(QtThemeType::Light, PrimaryPress));
    d->overButton->setLightTextColor(Qt::white);
    d->overButton->setDarkDefaultColor(QtThemeColor(QtThemeType::Dark, PrimaryNormal));
    d->overButton->setDarkHoverColor(QtThemeColor(QtThemeType::Dark, PrimaryHover));
    d->overButton->setDarkPressColor(QtThemeColor(QtThemeType::Dark, PrimaryPress));
    d->overButton->setDarkTextColor(Qt::white);
    connect(d->overButton, &QtPushButton::clicked, this, [this]() {
        Q_EMIT colorSelected(d_ptr->currentColor);
        close();
    });
    d->cancelButton = new QtPushButton("Cancel", this);
    d->cancelButton->setBorderRadius(6);
    connect(d->cancelButton, &QtPushButton::clicked, this, [this]() { close(); });
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(d->overButton);
    buttonLayout->addWidget(d->cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 11, 20);
    mainLayout->addLayout(colorPickerTextLayout);
    mainLayout->addSpacing(3);
    mainLayout->addLayout(colorControlLayout);
    mainLayout->addSpacing(80);
    mainLayout->addLayout(colorDisplayLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    d->themeMode = qtTheme->getThemeMode();
    connect(qtTheme, &QtTheme::themeModeChanged, this,
        [this](QtThemeType::ThemeMode themeMode) { d_ptr->themeMode = themeMode; });
    setCurrentColor(QColor(0x80, 0xFF, 0xEF));
}

QtColorDialog::~QtColorDialog()
{
    Q_D(QtColorDialog);
    delete d->colorValueSlider->style();
}

QList<QColor> QtColorDialog::customColorList() const
{
    Q_D(const QtColorDialog);
    return d->customColorModel->displayColorList();
}
QColor QtColorDialog::customColor(int index) const
{
    Q_D(const QtColorDialog);
    return d->customColorModel->displayColor(index);
}
QString QtColorDialog::currentColorRGB() const
{
    Q_D(const QtColorDialog);
    return d->hexRgbValue();
}

QColor QtColorDialog::currentColor() const
{
    Q_D(const QtColorDialog);
    return d->currentColor;
}
void QtColorDialog::setCurrentColor(QColor value)
{
    Q_D(QtColorDialog);
    d->currentColor = value.toHsv();
    d->updateHtmlEditValue();
    d->updateEditValue();
    d->updateColorPreview();
    d->updateColorValueSlider();
    d->colorPicker->setSelectedColor(d->currentColor);
    Q_EMIT currentColorChanged();
}

void QtColorDialog::paintEvent(QPaintEvent* event)
{
    Q_D(QtColorDialog);
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QtThemeColor(d->themeMode, DialogBase));
    painter.drawRect(rect());

    painter.setBrush(QtThemeColor(d->themeMode, DialogLayoutArea));
    painter.drawRect(QRect(0, height() - 78, width(), 78));
    painter.restore();
    QDialog::paintEvent(event);
}
