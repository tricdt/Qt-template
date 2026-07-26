#include "qtexponentialblur.h"
#include "qttheme.h"

class QtExponentialBlurPrivate
{
    Q_DECLARE_PUBLIC(QtExponentialBlur)
public:
    QtExponentialBlur* q_ptr;
    QtThemeType::ThemeMode themeMode;
    static int _aprec;
    static int _zprec;
    static void drawExponentialBlur(QImage& image, const quint16& qRadius);
    static void drawRowBlur(QImage& image, const int& row, const int& alpha);
    static void drawColumnBlur(QImage& image, const int& column, const int& alpha);
    static void drawInnerBlur(unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha);
    QtExponentialBlurPrivate(QtExponentialBlur* q);
};

QtExponentialBlurPrivate::QtExponentialBlurPrivate(QtExponentialBlur* q)
    : q_ptr(q)
{
}

int QtExponentialBlurPrivate::_aprec = 12;
int QtExponentialBlurPrivate::_zprec = 7;

void QtExponentialBlurPrivate::drawExponentialBlur(QImage& image, const quint16& qRadius)
{
    if(qRadius < 1)
    {
        return;
    }
    image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    int alpha = (int) ((1 << _aprec) * (1.0f - std::exp(-2.3f / (qRadius + 1.f))));
    int height = image.height();
    int width = image.width();
    for(int row = 0; row < height; row++)
    {
        drawRowBlur(image, row, alpha);
    }

    for(int col = 0; col < width; col++)
    {
        drawColumnBlur(image, col, alpha);
    }
}
void QtExponentialBlurPrivate::drawRowBlur(QImage& image, const int& row, const int& alpha)
{
    int zR, zG, zB, zA;

    QRgb* ptr = (QRgb*) image.scanLine(row);
    int width = image.width();

    zR = *((unsigned char*) ptr) << _zprec;
    zG = *((unsigned char*) ptr + 1) << _zprec;
    zB = *((unsigned char*) ptr + 2) << _zprec;
    zA = *((unsigned char*) ptr + 3) << _zprec;

    for(int index = 0; index < width; index++)
    {
        drawInnerBlur((unsigned char*) &ptr[index], zR, zG, zB, zA, alpha);
    }
    for(int index = width - 2; index >= 0; index--)
    {
        drawInnerBlur((unsigned char*) &ptr[index], zR, zG, zB, zA, alpha);
    }
}
void QtExponentialBlurPrivate::drawColumnBlur(QImage& image, const int& column, const int& alpha)
{
    int zR, zG, zB, zA;

    QRgb* ptr = (QRgb*) image.bits();
    ptr += column;
    int height = image.height();
    int width = image.width();

    zR = *((unsigned char*) ptr) << _zprec;
    zG = *((unsigned char*) ptr + 1) << _zprec;
    zB = *((unsigned char*) ptr + 2) << _zprec;
    zA = *((unsigned char*) ptr + 3) << _zprec;

    for(int index = width; index < (height - 1) * width; index += width)
    {
        drawInnerBlur((unsigned char*) &ptr[index], zR, zG, zB, zA, alpha);
    }
    for(int index = (height - 2) * width; index >= 0; index -= width)
    {
        drawInnerBlur((unsigned char*) &ptr[index], zR, zG, zB, zA, alpha);
    }
}
void QtExponentialBlurPrivate::drawInnerBlur(
    unsigned char* bptr, int& zR, int& zG, int& zB, int& zA, int alpha)
{
    int R, G, B, A;
    R = *bptr;
    G = *(bptr + 1);
    B = *(bptr + 2);
    A = *(bptr + 3);

    zR += (alpha * ((R << _zprec) - zR)) >> _aprec;
    zG += (alpha * ((G << _zprec) - zG)) >> _aprec;
    zB += (alpha * ((B << _zprec) - zB)) >> _aprec;
    zA += (alpha * ((A << _zprec) - zA)) >> _aprec;

    *bptr = zR >> _zprec;
    *(bptr + 1) = zG >> _zprec;
    *(bptr + 2) = zB >> _zprec;
    *(bptr + 3) = zA >> _zprec;
}

QtExponentialBlur::QtExponentialBlur(QWidget* parent)
    : QObject(parent)
    , d_ptr(new QtExponentialBlurPrivate(this))
{
}

QtExponentialBlur::~QtExponentialBlur()
{
}

QPixmap QtExponentialBlur::doExponentialBlur(QImage img, const quint16& blurRadius)
{
    QImage shadowImage = img.convertToFormat(QImage::Format_ARGB32);
    QtExponentialBlur::getInstance()->d_ptr->drawExponentialBlur(shadowImage, blurRadius);
    return QPixmap::fromImage(shadowImage);
}
