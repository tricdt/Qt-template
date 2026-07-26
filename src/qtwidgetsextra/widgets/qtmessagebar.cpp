#include "qtmessagebar.h"
#include "qttheme.h"
#include "widgets/qticonbutton.h"
#include <qdatetime.h>
#include <qtypes.h>
#include <qvariant.h>
#include <qvariantanimation.h>

class QtMessageBarPrivate
{
    Q_DECLARE_PUBLIC(QtMessageBar)
public:
    QtMessageBar* q_ptr;
    qreal opacity;
    qreal timePercent;
    int messageBarClosedY{ 0 };
    int messageBarFinishY{ 0 };
    QtMessageBarPrivate(QtMessageBar* q);
    ~QtMessageBarPrivate();
    void onOtherMessageBarEnd();
    void messageBarEnd();

    QtThemeType::ThemeMode themeMode;
    int borderRadius{ 6 };
    QString title{ "" };
    QString text{ "" };
    QtMessageBarType::PositionPolicy policy;
    QtMessageBarType::MessageMode messageMode;
    qreal createTime{ 0 };

    int leftPadding{ 20 };
    int titleLeftSpacing{ 30 };
    int textLeftSpacing{ 15 };
    int closeButtonLeftRightMargin{ 20 };
    int closeButtonWidth{ 30 };
    int messageBarHorizontalMargin{ 20 };
    int messageBarVerticalBottomMargin{ 20 };
    int messageBarVerticalTopMargin{ 20 };
    int messageBarSpacing{ 15 };
    int shadowBorderWidth{ 6 };
    qreal timePercentHeight{ 2 };

    bool isNormalDisplay{ false };
    bool isOtherMessageBarEnd{ false };
    QtIconButton* closeButton{ nullptr };
    void messageBarCreate(int displayMsec);

    void calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
    QList<int> otherMessageBarTotalData();
    qreal calculateTargetPosY();

    bool judgeCreateOrder(QtMessageBar* otherMessageBar);
    void updateActiveMap(bool isActive);

    void drawSuccess(QPainter* painter);
    void drawWarning(QPainter* painter);
    void drawInformation(QPainter* painter);
    void drawError(QPainter* painter);
};

QMap<QtMessageBarType::PositionPolicy, QList<QtMessageBar*>*> messageBarActiveMap;

QtMessageBarPrivate::QtMessageBarPrivate(QtMessageBar* q)
    : q_ptr(q)
{
    messageBarClosedY = 0;
    messageBarFinishY = 0;
    timePercent = 100;
    createTime = QDateTime::currentMSecsSinceEpoch();
}
QtMessageBarPrivate::~QtMessageBarPrivate()
{
}

void QtMessageBarPrivate::onOtherMessageBarEnd()
{
    Q_Q(QtMessageBar);
    qreal targetPosY = calculateTargetPosY();
    QVariantAnimation* closePosAnimation = new QVariantAnimation(q);
    QObject::connect(
        closePosAnimation, &QVariantAnimation::valueChanged, q, [this](const QVariant& value) {
            messageBarClosedY = value.toInt();
            q_ptr->move(q_ptr->pos().x(), value.toUInt());
        });
    closePosAnimation->setEasingCurve(QEasingCurve::OutCubic);
    closePosAnimation->setDuration(220);
    closePosAnimation->setStartValue(q->pos().y());
    closePosAnimation->setEndValue(targetPosY);
    closePosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
void QtMessageBarPrivate::messageBarEnd()
{
    Q_Q(QtMessageBar);
    closeButton->setEnabled(false);
    updateActiveMap(false);
    QVariantAnimation* barFinishedOpacityAnimation = new QVariantAnimation(q);
    QObject::connect(barFinishedOpacityAnimation, &QVariantAnimation::valueChanged, q,
        [this](const QVariant& value) {
            closeButton->setOpacity(opacity);
            q_ptr->update();
        });
    QObject::connect(barFinishedOpacityAnimation, &QPropertyAnimation::finished, q,
        [this]() { q_ptr->deleteLater(); });
    barFinishedOpacityAnimation->setDuration(300);
    barFinishedOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barFinishedOpacityAnimation->setStartValue(1);
    barFinishedOpacityAnimation->setEndValue(0);
    barFinishedOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    // 通知同类型的其他MessageBar
    for(const auto messageBar : *messageBarActiveMap[policy])
    {
        if(messageBar->d_ptr->isNormalDisplay)
        {
            messageBar->d_ptr->onOtherMessageBarEnd();
        }
        else
        {
            messageBar->d_ptr->isOtherMessageBarEnd = true;
        }
    }
}

void QtMessageBarPrivate::messageBarCreate(int displayMsec)
{
    Q_Q(QtMessageBar);
    q->show();
    QFont font = q->font();
    font.setPixelSize(16);
    font.setWeight(QFont::Bold);
    q->setFont(font);
    int titleWidth = q->fontMetrics().horizontalAdvance(title);
    font.setPixelSize(14);
    font.setWeight(QFont::Medium);
    q->setFont(font);
    int textWidth = q->fontMetrics().horizontalAdvance(text);
    int fixedWidth = closeButtonLeftRightMargin + leftPadding + titleLeftSpacing + textLeftSpacing +
                     closeButtonWidth + titleWidth + textWidth + 2 * shadowBorderWidth;
    q->setFixedWidth(fixedWidth > 500 ? 500 : fixedWidth);
    int startX = 0;
    int startY = 0;
    int endX = 0;
    int endY = 0;
    updateActiveMap(true);
    qDebug() << q->parentWidget();
    calculateInitialPos(startX, startY, endX, endY);
    QPropertyAnimation* barPosAnimation = new QPropertyAnimation(q, "pos");
    QObject::connect(barPosAnimation, &QPropertyAnimation::finished, q, [this, displayMsec]() {
        isNormalDisplay = true;
        if(isOtherMessageBarEnd)
        {
            onOtherMessageBarEnd();
        }
        QTimer::singleShot(displayMsec, q_ptr, [this]() { messageBarEnd(); });
        QVariantAnimation* timePercentAnimation = new QVariantAnimation(q_ptr);
        QObject::connect(timePercentAnimation, &QVariantAnimation::valueChanged, q_ptr,
            [this]() { q_ptr->update(); });
        timePercentAnimation->setStartValue(100);
        timePercentAnimation->setEndValue(0);
        timePercentAnimation->setEasingCurve(QEasingCurve::Linear);
        timePercentAnimation->setDuration(displayMsec);
        timePercentAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    });
    switch(policy)
    {
        case QtMessageBarType::Top:
        case QtMessageBarType::Bottom: {
            barPosAnimation->setDuration(250);
            break;
        }
        default: {
            barPosAnimation->setDuration(350);
            break;
        }
    }
    barPosAnimation->setStartValue(QPoint(startX, startY));
    barPosAnimation->setEndValue(QPoint(endX, endY));
    barPosAnimation->setEasingCurve(QEasingCurve::InOutSine);
    barPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QtMessageBarPrivate::calculateInitialPos(int& startX, int& startY, int& endX, int& endY)
{
    Q_Q(QtMessageBar);
    QList<int> resultList = otherMessageBarTotalData();
    int minimumHeightTotal = resultList[0];
    int indexLessCount = resultList[1];
    int lastEndY = endY;
    switch(policy)
    {
        case QtMessageBarType::Top: {
            startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
            startY = minimumHeightTotal + messageBarSpacing * indexLessCount +
                     messageBarVerticalTopMargin - 25;
            endX = startX;
            endY = minimumHeightTotal + messageBarSpacing * indexLessCount +
                   messageBarVerticalTopMargin;
            break;
        }
        case QtMessageBarType::Left: {
            startX = -q->minimumWidth();
            startY = minimumHeightTotal + messageBarSpacing * indexLessCount +
                     q->parentWidget()->height() / 2;
            endX = messageBarHorizontalMargin;
            endY = startY;
            break;
        }
        case QtMessageBarType::Bottom: {
            startX = q->parentWidget()->width() / 2 - q->minimumWidth() / 2;
            startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                     messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin - 25;
            endX = startX;
            endY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                   messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
            break;
        }
        case QtMessageBarType::Right: {
            startX = q->parentWidget()->width();
            startY = minimumHeightTotal + messageBarSpacing * indexLessCount +
                     q->parentWidget()->height() / 2;
            endX = q->parentWidget()->width() - q->minimumWidth() - messageBarHorizontalMargin;
            endY = startY;
            break;
        }
        case QtMessageBarType::TopRight: {
            startX = q->parentWidget()->width();
            startY = minimumHeightTotal + messageBarSpacing * indexLessCount +
                     messageBarVerticalTopMargin;
            endX = q->parentWidget()->width() - q->minimumWidth() - messageBarHorizontalMargin;
            endY = startY;
            break;
        }
        case QtMessageBarType::TopLeft: {
            startX = -q->minimumWidth();
            startY = minimumHeightTotal + messageBarSpacing * indexLessCount +
                     messageBarVerticalTopMargin;
            endX = messageBarHorizontalMargin;
            endY = startY;
            break;
        }
        case QtMessageBarType::BottomRight: {
            startX = q->parentWidget()->width();
            startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                     messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
            endX = q->parentWidget()->width() - q->minimumWidth() - messageBarHorizontalMargin;
            endY = startY;
            break;
        }
        case QtMessageBarType::BottomLeft: {
            startX = -q->minimumWidth();
            startY = q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                     messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
            endX = messageBarHorizontalMargin;
            endY = startY;
            break;
        }
    }
    if(endY == lastEndY)
    {
        return;
    }
    if(endY < messageBarVerticalTopMargin ||
        endY > q->parentWidget()->height() - messageBarVerticalBottomMargin - q->minimumHeight())
    {
        (*messageBarActiveMap[policy])[0]->d_ptr->messageBarEnd();
        calculateInitialPos(startX, startY, endX, endY);
    }
}
QList<int> QtMessageBarPrivate::otherMessageBarTotalData()
{
    Q_Q(QtMessageBar);
    QList<int> resultList;
    int minimumHeightTotal = 0;
    int indexLessCount = 0;
    QList<QtMessageBar*>* messageBarList = messageBarActiveMap[policy];
    for(const auto messageBar : *messageBarList)
    {
        if(messageBar == q)
        {
            continue;
        }
        if(judgeCreateOrder(messageBar))
        {
            indexLessCount++;
            minimumHeightTotal += messageBar->minimumHeight();
        }
    }
    resultList.append(minimumHeightTotal);
    resultList.append(indexLessCount);
    return resultList;
}
qreal QtMessageBarPrivate::calculateTargetPosY()
{
    Q_Q(QtMessageBar);
    QList<int> resultList = otherMessageBarTotalData();
    int minimumHeightTotal = resultList[0];
    int indexLessCount = resultList[1];
    switch(policy)
    {
        case QtMessageBarType::Top:
        case QtMessageBarType::TopRight:
        case QtMessageBarType::TopLeft: {
            return minimumHeightTotal + messageBarSpacing * indexLessCount +
                   messageBarVerticalTopMargin;
        }
        case QtMessageBarType::Left:
        case QtMessageBarType::Right: {
            return minimumHeightTotal + messageBarSpacing * indexLessCount +
                   q->parentWidget()->height() / 2;
        }
        case QtMessageBarType::Bottom:
        case QtMessageBarType::BottomRight:
        case QtMessageBarType::BottomLeft: {
            return q->parentWidget()->height() - q->minimumHeight() - minimumHeightTotal -
                   messageBarSpacing * indexLessCount - messageBarVerticalBottomMargin;
        }
    }
    return 0;
}

bool QtMessageBarPrivate::judgeCreateOrder(QtMessageBar* otherMessageBar)
{
    if(otherMessageBar->d_ptr->createTime < createTime)
    {
        // otherMessageBar先创建
        return true;
    }
    else
    {
        return false;
    }
}
void QtMessageBarPrivate::updateActiveMap(bool isActive)
{
    Q_Q(QtMessageBar);
    QtMessageBarType::PositionPolicy _policy = policy;
    if(isActive)
    {
        if(messageBarActiveMap.contains(_policy))
        {
            messageBarActiveMap[_policy]->append(q);
        }
        else
        {
            QList<QtMessageBar*>* messageBarList = new QList<QtMessageBar*>();
            messageBarList->append(q);
            messageBarActiveMap.insert(_policy, messageBarList);
        }
    }
    else
    {
        if(messageBarActiveMap.contains(_policy))
        {
            if(messageBarActiveMap[_policy]->count() > 0)
            {
                messageBarActiveMap[_policy]->removeOne(q);
            }
        }
    }
}

void QtMessageBarPrivate::drawSuccess(QPainter* painter)
{
    Q_Q(QtMessageBar);
    painter->save();
    painter->setBrush(QColor(0xE0, 0xF6, 0xDD));
    QRect foregroundRect(shadowBorderWidth, shadowBorderWidth, q->width() - 2 * shadowBorderWidth,
        q->height() - 2 * shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, borderRadius, borderRadius);
    // 图标绘制
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0x11, 0x77, 0x10));
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(12);
    painter->setFont(iconFont);
    painter->drawText(leftPadding, 0, q->width(), q->height(), Qt::AlignVCenter,
        QChar(static_cast<ushort>(QtIconType::Check)));
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, borderRadius, borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0x3C, 0x96, 0x4B));
    painter->drawRoundedRect(
        QRectF(foregroundRect.x(), foregroundRect.bottom() - timePercentHeight,
            foregroundRect.width() * timePercent / 100.0, timePercentHeight + 1),
        2, 2);
    painter->restore();
    painter->setPen(QPen(Qt::black));
}
void QtMessageBarPrivate::drawWarning(QPainter* painter)
{
    Q_Q(QtMessageBar);
    painter->save();
    painter->setBrush(QColor(0xFF, 0xF4, 0xCE));
    QRect foregroundRect(shadowBorderWidth, shadowBorderWidth, q->width() - 2 * shadowBorderWidth,
        q->height() - 2 * shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, borderRadius, borderRadius);
    // 图标绘制
    // exclamation
    painter->setPen(Qt::black);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0xF8, 0xE2, 0x23));
    painter->drawText(leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "!");
    // 时间进度条绘制
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, borderRadius, borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0xC4, 0xAD, 0x59));
    painter->drawRoundedRect(
        QRectF(foregroundRect.x(), foregroundRect.bottom() - timePercentHeight,
            foregroundRect.width() * timePercent / 100.0, timePercentHeight + 1),
        2, 2);
    painter->restore();
    painter->setPen(Qt::black);
}
void QtMessageBarPrivate::drawInformation(QPainter* painter)
{
    Q_Q(QtMessageBar);
    painter->save();
    painter->setBrush(QColor(0xF4, 0xF4, 0xF4));
    QRect foregroundRect(shadowBorderWidth, shadowBorderWidth, q->width() - 2 * shadowBorderWidth,
        q->height() - 2 * shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, borderRadius, borderRadius);
    // 图标绘制
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0x00, 0x66, 0xB4));
    painter->drawText(leftPadding + 4, 0, q->width(), q->height(), Qt::AlignVCenter, "i");
    // 时间进度条绘制
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, borderRadius, borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0x00, 0x66, 0xB4));
    painter->drawRoundedRect(
        QRectF(foregroundRect.x(), foregroundRect.bottom() - timePercentHeight,
            foregroundRect.width() * timePercent / 100.0, timePercentHeight + 1),
        2, 2);
    painter->restore();
    painter->setPen(Qt::black);
}
void QtMessageBarPrivate::drawError(QPainter* painter)
{
    Q_Q(QtMessageBar);
    painter->save();
    painter->setBrush(QColor(0xFD, 0xE7, 0xE9));
    QRect foregroundRect(shadowBorderWidth, shadowBorderWidth, q->width() - 2 * shadowBorderWidth,
        q->height() - 2 * shadowBorderWidth);
    painter->drawRoundedRect(foregroundRect, borderRadius, borderRadius);
    // 图标绘制
    painter->setPen(Qt::white);
    QPainterPath textPath;
    textPath.addEllipse(QPoint(leftPadding + 6, q->height() / 2), 9, 9);
    painter->setClipPath(textPath);
    painter->fillPath(textPath, QColor(0xBA, 0x2D, 0x20));
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(13);
    painter->setFont(iconFont);
    painter->drawText(leftPadding + 1, 0, q->width(), q->height(), Qt::AlignVCenter,
        QChar(static_cast<ushort>(QtIconType::Xmark)));
    QPainterPath clipPath;
    clipPath.addRoundedRect(foregroundRect, borderRadius, borderRadius);
    painter->setClipPath(clipPath);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0xCC, 0x5C, 0x65));
    painter->drawRoundedRect(
        QRectF(foregroundRect.x(), foregroundRect.bottom() - timePercentHeight,
            foregroundRect.width() * timePercent / 100.0, timePercentHeight + 1),
        2, 2);
    painter->restore();
    painter->setPen(Qt::black);
}

QtMessageBar::QtMessageBar(QtMessageBarType::PositionPolicy policy,
    QtMessageBarType::MessageMode messageMode, QString& title, QString& text, int displayMsec,
    QWidget* parent)
    : QWidget(parent)
    , d_ptr(new QtMessageBarPrivate(this))
{
    Q_D(QtMessageBar);
    d->borderRadius = 6;
    d->title = title;
    d->text = text;
    d->policy = policy;
    d->messageMode = messageMode;
    d->themeMode = qtTheme->getThemeMode();
    setFixedHeight(60);
    setMouseTracking(true);
    d->opacity = 1;
    setFont(QFont("微软雅黑"));
    parent->installEventFilter(this);
    d->closeButton = new QtIconButton(QtIconType::Xmark, 17, d->closeButtonWidth, 30, this);
    switch(d->messageMode)
    {
        case QtMessageBarType::Success: {
            d->closeButton->setLightHoverColor(QColor(0xCA, 0xDE, 0xC8));
            d->closeButton->setDarkHoverColor(QColor(0xCA, 0xDE, 0xC8));
            d->closeButton->setDarkIconColor(Qt::black);
            d->closeButton->setDarkHoverIconColor(Qt::black);
            break;
        }
        case QtMessageBarType::Warning: {
            d->closeButton->setLightHoverColor(QColor(0x5E, 0x4C, 0x22));
            d->closeButton->setDarkHoverColor(QColor(0x5E, 0x4C, 0x22));
            d->closeButton->setLightIconColor(Qt::white);
            d->closeButton->setDarkIconColor(Qt::white);
            d->closeButton->setLightHoverIconColor(Qt::white);
            break;
        }
        case QtMessageBarType::Information: {
            d->closeButton->setLightHoverColor(QColor(0xDE, 0xDE, 0xDE));
            d->closeButton->setDarkHoverColor(QColor(0xDE, 0xDE, 0xDE));
            d->closeButton->setDarkIconColor(Qt::black);
            d->closeButton->setDarkHoverIconColor(Qt::black);
            break;
        }
        case QtMessageBarType::Error: {
            d->closeButton->setLightHoverColor(QColor(0xF2, 0xDD, 0xE0));
            d->closeButton->setDarkHoverColor(QColor(0xF2, 0xDD, 0xE0));
            d->closeButton->setDarkIconColor(Qt::black);
            d->closeButton->setDarkHoverIconColor(Qt::black);
            break;
        }
    }
    d->closeButton->setBorderRadius(5);
    connect(d->closeButton, &QtIconButton::clicked, this, [this]() { d_ptr->messageBarEnd(); });
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 10, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(d->closeButton);
    setObjectName("QtMessageBar");
    setStyleSheet("#QtMessageBar{background-color:transparent;}");
    d->messageBarCreate(displayMsec);
}

QtMessageBar::~QtMessageBar()
{
}

void QtMessageBar::success(QtMessageBarType::PositionPolicy policy, QString title, QString text,
    int displayMsec, QWidget* parent)
{
    if(!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for(auto widget : widgetList)
        {
            if(widget->property("QtBaseClassName").toString() == "QtWindow")
            {
                parent = widget;
            }
        }
        if(!parent)
        {
            return;
        }
    }
    QtMessageBar* bar =
        new QtMessageBar(policy, QtMessageBarType::Success, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}
void QtMessageBar::warning(QtMessageBarType::PositionPolicy policy, QString title, QString text,
    int displayMsec, QWidget* parent)
{
    if(!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for(auto widget : widgetList)
        {
            if(widget->property("QtBaseClassName").toString() == "QtWindow")
            {
                parent = widget;
            }
        }
        if(!parent)
        {
            return;
        }
    }
    QtMessageBar* bar =
        new QtMessageBar(policy, QtMessageBarType::Warning, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}
void QtMessageBar::information(QtMessageBarType::PositionPolicy policy, QString title, QString text,
    int displayMsec, QWidget* parent)
{
    if(!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for(auto widget : widgetList)
        {
            if(widget->property("QtBaseClassName").toString() == "QtWindow")
            {
                parent = widget;
            }
        }
        if(!parent)
        {
            return;
        }
    }
    QtMessageBar* bar =
        new QtMessageBar(policy, QtMessageBarType::Information, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}
void QtMessageBar::error(QtMessageBarType::PositionPolicy policy, QString title, QString text,
    int displayMsec, QWidget* parent)
{
    if(!parent)
    {
        QList<QWidget*> widgetList = QApplication::topLevelWidgets();
        for(auto widget : widgetList)
        {
            if(widget->property("QtBaseClassName").toString() == "QtWindow")
            {
                parent = widget;
            }
        }
        if(!parent)
        {
            return;
        }
    }
    QtMessageBar* bar =
        new QtMessageBar(policy, QtMessageBarType::Error, title, text, displayMsec, parent);
    Q_UNUSED(bar);
}

void QtMessageBar::paintEvent(QPaintEvent*)
{
    Q_D(QtMessageBar);
    QPainter painter(this);
    painter.setOpacity(d->opacity);
    painter.setRenderHints(
        QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    qtTheme->drawEffectShadow(&painter, rect(), d->shadowBorderWidth, d->borderRadius);

    painter.save();
    painter.setPen(
        d->themeMode == QtThemeType::Light ? QColor(0xBE, 0xBA, 0xBE) : QColor(0x52, 0x50, 0x52));
    switch(d->messageMode)
    {
        case QtMessageBarType::Success: {
            d->drawSuccess(&painter);
            break;
        }
        case QtMessageBarType::Warning: {
            d->drawWarning(&painter);
            break;
        }
        case QtMessageBarType::Information: {
            d->drawInformation(&painter);
            break;
        }
        case QtMessageBarType::Error: {
            d->drawError(&painter);
            break;
        }
    }
    QFont font = this->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(16);
    painter.setFont(font);
    int titleTextWidth = painter.fontMetrics().horizontalAdvance(d->title) + 1;
    if(titleTextWidth > 100)
    {
        titleTextWidth = 100;
    }
    int textFlags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap | Qt::TextWrapAnywhere;
    painter.drawText(QRect(d->leftPadding + d->titleLeftSpacing, -1, titleTextWidth, height()),
        textFlags, d->title);
    font.setWeight(QFont::Light);
    font.setPixelSize(15);
    painter.setFont(font);
    painter.drawText(
        QRect(d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing, 0,
            width() - (d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing +
                          d->closeButtonWidth + d->closeButtonLeftRightMargin / 2),
            height() - d->timePercentHeight),
        textFlags, d->text);
    int textHeight =
        painter.fontMetrics()
            .boundingRect(
                QRect(d->leftPadding + d->titleLeftSpacing + titleTextWidth + d->textLeftSpacing, 0,
                    width() - (d->leftPadding + d->titleLeftSpacing + titleTextWidth +
                                  d->textLeftSpacing + d->closeButtonWidth +
                                  d->closeButtonLeftRightMargin),
                    height()),
                textFlags, d->text)
            .height();
    if(textHeight >= minimumHeight() - 20)
    {
        setMinimumHeight(textHeight + 20);
    }
    painter.restore();
}

bool QtMessageBar::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(QtMessageBar);
    if(watched == parentWidget())
    {
        switch(event->type())
        {
            case QEvent::Resize: {
                QResizeEvent* resizeEvent = dynamic_cast<QResizeEvent*>(event);
                QSize offsetSize = parentWidget()->size() - resizeEvent->oldSize();
                if(d->isNormalDisplay)
                {
                    switch(d->policy)
                    {
                        case QtMessageBarType::Top: {
                            this->move(parentWidget()->width() / 2 - minimumWidth() / 2, this->y());
                            break;
                        }
                        case QtMessageBarType::Bottom: {
                            this->move(parentWidget()->width() / 2 - minimumWidth() / 2,
                                this->pos().y() + offsetSize.height());
                            break;
                        }
                        case QtMessageBarType::Left:
                        case QtMessageBarType::TopLeft: {
                            this->move(d->messageBarHorizontalMargin, this->pos().y());
                            break;
                        }
                        case QtMessageBarType::BottomLeft: {
                            this->move(d->messageBarHorizontalMargin,
                                this->pos().y() + offsetSize.height());
                            break;
                        }
                        case QtMessageBarType::Right:
                        case QtMessageBarType::TopRight: {
                            this->move(parentWidget()->width() - minimumWidth() -
                                           d->messageBarHorizontalMargin,
                                this->y());
                            break;
                        }
                        case QtMessageBarType::BottomRight: {
                            this->move(parentWidget()->width() - minimumWidth() -
                                           d->messageBarHorizontalMargin,
                                this->pos().y() + offsetSize.height());
                            break;
                        }
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
