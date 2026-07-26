#include "qtcolorpicker.h"
#include "qtdef.h"
#include "qttheme.h"
#include <qevent.h>
#include <qpainterpath.h>

class QtColorPickerPrivate
{
    Q_DECLARE_PUBLIC(QtColorPicker)
public:
    QtColorPicker* q_ptr;
    QImage colorPickerImage;
    QPoint selectedPoint;
    QColor selectedColor;

    QtThemeType::ThemeMode themeMode;
    QtColorPickerPrivate(QtColorPicker* q);
    ~QtColorPickerPrivate();
    QPoint adjustPointLimit(QPoint point);
};

QtColorPickerPrivate::QtColorPickerPrivate(QtColorPicker* q)
    : q_ptr(q)
{
}
QtColorPickerPrivate::~QtColorPickerPrivate()
{
}

QPoint QtColorPickerPrivate::adjustPointLimit(QPoint point)
{
    Q_Q(QtColorPicker);
    QRect pickerRect = q->rect();
    if(point.x() < 3)
    {
        point.setX(3);
    }
    else if(point.x() > pickerRect.width() - 3)
    {
        point.setX(pickerRect.width() - 3);
    }
    if(point.y() < 3)
    {
        point.setY(3);
    }
    else if(point.y() > pickerRect.height() - 3)
    {
        point.setY(pickerRect.height() - 3);
    }
    return point;
}

QtColorPicker::QtColorPicker(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtColorPickerPrivate(this))
{
    Q_D(QtColorPicker);
    setFixedSize(260, 260);
    d->themeMode = qtTheme->getThemeMode();
    QPixmap colorPickerPix(QSize(360, 360));
    QImage colorPickerImage = colorPickerPix.toImage();
    for(int y = 0; y < colorPickerImage.height(); y++)
    {
        for(int x = 0; x < colorPickerImage.width(); x++)
        {
            QColor pixColor;
            pixColor.setHsvF((qreal) 1 / colorPickerPix.width() * x,
                1 - (qreal) 1 / (colorPickerPix.height() - 1) * y, 1);
            pixColor = pixColor.toRgb();
            colorPickerImage.setPixelColor(x, y, pixColor);
        }
    }
    d->colorPickerImage = colorPickerImage;
}

QtColorPicker::~QtColorPicker()
{
}

void QtColorPicker::setSelectedColor(QColor color)
{
    Q_D(QtColorPicker);
    d->selectedColor = std::move(color);
    d->selectedPoint = QPoint(qRound(3 + 254 * d->selectedColor.hueF() + 0.5f),
        qRound(257 - 254 * d->selectedColor.saturationF() + 0.5f));
    update();
}
QColor QtColorPicker::selectedColor() const
{
    Q_D(const QtColorPicker);
    return d->selectedColor;
}

void QtColorPicker::mousePressEvent(QMouseEvent* event)
{
    Q_D(QtColorPicker);
    d->selectedPoint = d->adjustPointLimit(event->pos());
    QPointF colorPoint(
        (d->selectedPoint.x() - 3) / 254.0 * 359, (d->selectedPoint.y() - 3) / 254.0 * 359);
    QColor pixColor;
    pixColor.setHsvF((qreal) 1 / d->colorPickerImage.width() * colorPoint.x(),
        1 - (qreal) 1 / (d->colorPickerImage.height() - 1) * colorPoint.y(), 1);
    d->selectedColor = pixColor.toRgb();
    Q_EMIT selectedColorChanged(d->selectedColor);
    QWidget::mousePressEvent(event);
    update();
}

void QtColorPicker::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(QtColorPicker);
    d->selectedPoint = d->adjustPointLimit(event->pos());
    QPointF colorPoint(
        (d->selectedPoint.x() - 3) / 254.0 * 359, (d->selectedPoint.y() - 3) / 254.0 * 359);
    QColor pixColor;
    pixColor.setHsvF((qreal) 1 / d->colorPickerImage.width() * colorPoint.x(),
        1 - (qreal) 1 / (d->colorPickerImage.height() - 1) * colorPoint.y(), 1);
    d->selectedColor = pixColor.toRgb();
    Q_EMIT selectedColorChanged(d->selectedColor);
    QWidget::mouseReleaseEvent(event);
    update();
}

void QtColorPicker::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(QtColorPicker);
    d->selectedPoint = d->adjustPointLimit(event->pos());
    QPointF colorPoint(
        (d->selectedPoint.x() - 3) / 254.0 * 359, (d->selectedPoint.y() - 3) / 254.0 * 359);
    QColor pixColor;
    pixColor.setHsvF((qreal) 1 / d->colorPickerImage.width() * colorPoint.x(),
        1 - (qreal) 1 / (d->colorPickerImage.height() - 1) * colorPoint.y(), 1);
    d->selectedColor = pixColor.toRgb();
    Q_EMIT selectedColorChanged(d->selectedColor);
    QWidget::mouseMoveEvent(event);
    update();
}

void QtColorPicker::paintEvent(QPaintEvent* event)
{
    Q_D(QtColorPicker);
    int penWidth = 3;
    int borderRadius = 5;
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QRect pickerRect = rect();
    pickerRect.adjust(penWidth, penWidth, -penWidth, -penWidth);
    painter.setPen(QPen(QtThemeColor(d->themeMode, BasicBorder), penWidth));
    painter.drawRoundedRect(pickerRect, borderRadius, borderRadius);
    QPainterPath path;
    path.addRoundedRect(pickerRect, borderRadius, borderRadius);
    painter.setClipPath(path);
    painter.drawImage(pickerRect, d->colorPickerImage);

    if(!d->selectedPoint.isNull())
    {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(d->selectedPoint, 8, 8);
    }
    painter.restore();
}