#pragma once

#include "T_BasePage.h"
#include "widgets/qtcommandbar.h"
#include "widgets/qtexpander.h"
#include "widgets/qtflyout.h"
#include "widgets/qtinfobadge.h"
#include "widgets/qtpagination.h"
#include "widgets/qtpersonpicture.h"
#include "widgets/qtpushbutton.h"
#include "widgets/qtratingcontrol.h"
#include "widgets/qtsplitbutton.h"
#include "widgets/qtsteps.h"
#include "widgets/qttag.h"
#include "widgets/qtteachingtip.h"
#include "widgets/qtpasswordbox.h"
#include <QObject>
#include <qwidget.h>
#include "widgets/qtnumberbox.h"
#include "widgets/qttimeline.h"

class T_NewComponents : public T_BasePage
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit T_NewComponents(QWidget* parent = nullptr);
    ~T_NewComponents();

private:
    // InfoBadge
    QtInfoBadge* _dotBadge{ nullptr };
    QtInfoBadge* _valueBadge{ nullptr };
    QtInfoBadge* _iconBadge{ nullptr };

    // Expander
    QtExpander* _expander{ nullptr };
    QtExpander* _expanderUp{ nullptr };

    // TeachingTip
    QtTeachingTip* _teachingTip{ nullptr };
    QtPushButton* _teachingTipTarget{ nullptr };

    // PasswordBox
    QtPasswordBox* _passwordBox{nullptr};

    // Tag
    QtTag* _closableTag{nullptr};
    QtTag* _checkableTag{nullptr};
    // SplitButton
    QtSplitButton* _splitButton{nullptr};
    // PersonPicture
    QtPersonPicture* _personPicture{nullptr};
    QtPersonPicture* _personPictureInitials{nullptr};
    QtPersonPicture* _personPictureDefault{nullptr};
    // Pagination
    QtPagination* _pagination{nullptr};
    // Flyout
    QtFlyout* _flyout{nullptr};
    // Steps
    QtSteps* _steps{nullptr};
    // Timeline
    QtTimeline* _timeline{nullptr};
    // CommandBar
    QtCommandBar* _commandBar{nullptr};
    // RatingControl
    QtRatingControl* _ratingControl{nullptr};
    // NumberBox
    QtNumberBox* _numberBox{nullptr};
    // // Wizard
    // ElaWizard* _wizard{nullptr};
    // // NotificationCenter
    // ElaNotificationCenter* _notificationCenter{nullptr};
    // // MarkdownViewer
    // ElaMarkdownViewer* _markdownViewer{nullptr};
    // // Captcha
    // ElaCaptcha* _captcha{nullptr};
    // // DropDownButton
    // ElaDropDownButton* _dropDownButton{nullptr};
    // // SelectorBar
    // ElaSelectorBar* _selectorBar{nullptr};
    // // VirtualList
    // ElaVirtualList* _virtualList{nullptr};
    // // SplashScreen
    // ElaSplashScreen* _splashScreen{nullptr};
    // // SheetPanel
    // ElaSheetPanel* _sheetPanel{nullptr};
    // // Transfer
    // ElaTransfer* _transfer{nullptr};
    // // Spotlight
    // ElaSpotlight* _spotlight{nullptr};
    // ElaCountdown* _countdown{nullptr};
    // ElaPopconfirm* _popconfirm{nullptr};
    // // Watermark
    // ElaWatermark* _watermark{nullptr};
    // // Splitter
    // ElaSplitter* _splitter{nullptr};
    // // QRCode
    // ElaQRCode* _qrCode{nullptr};
    // // FloatButton
    // ElaFloatButton* _floatButton{nullptr};
    // ElaEmojiPicker* _emojiPicker{nullptr};
    // // InfoBar
    // ElaInfoBar* _infoBarInfo{nullptr};
    // ElaInfoBar* _infoBarSuccess{nullptr};
    // ElaInfoBar* _infoBarWarning{nullptr};
    // ElaInfoBar* _infoBarError{nullptr};
    // // AutoComplete
    // ElaAutoComplete* _autoComplete{nullptr};
    // // TreeSelect
    // ElaTreeSelect* _treeSelect{nullptr};
    // // UploadArea
    // ElaUploadArea* _uploadArea{nullptr};
    // // CopyButton
    // ElaCopyButton* _copyButton{nullptr};
Q_SIGNALS:
};
