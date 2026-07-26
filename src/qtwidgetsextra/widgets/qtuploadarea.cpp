#include "qtuploadarea.h"

class QtUploadAreaPrivate
{
    Q_DECLARE_PUBLIC(QtUploadArea)
public:
    int borderRadius;
    QString title;
    QString subTitle;
    QStringList acceptedSuffixes;
    int maxFileCount;
    qint64 maxFileSize;
    bool isMultiple;
    QString dialogTitle;
    QtUploadArea* q_ptr;
    QtUploadAreaPrivate(QtUploadArea* q);
    ~QtUploadAreaPrivate();
};

QtUploadAreaPrivate::QtUploadAreaPrivate(QtUploadArea* q)
    : q_ptr(q)
{
}
QtUploadAreaPrivate::~QtUploadAreaPrivate()
{
}

QtUploadArea::QtUploadArea(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtUploadAreaPrivate(this))
{
}

QtUploadArea::~QtUploadArea()
{
}

int QtUploadArea::borderRadius() const
{
    Q_D(const QtUploadArea);
    return d->borderRadius;
}

void QtUploadArea::setBorderRadius(int value)
{
    Q_D(QtUploadArea);
    if(d->borderRadius != value)
    {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}

const QString& QtUploadArea::title() const
{
    Q_D(const QtUploadArea);
    return d->title;
}

void QtUploadArea::setTitle(const QString& value)
{
    Q_D(QtUploadArea);
    if(d->title != value)
    {
        d->title = value;
        Q_EMIT titleChanged();
    }
}

const QString& QtUploadArea::subTitle() const
{
    Q_D(const QtUploadArea);
    return d->subTitle;
}

void QtUploadArea::setSubTitle(const QString& value)
{
    Q_D(QtUploadArea);
    if(d->subTitle != value)
    {
        d->subTitle = value;
        Q_EMIT subTitleChanged();
    }
}

const QStringList& QtUploadArea::acceptedSuffixes() const
{
    Q_D(const QtUploadArea);
    return d->acceptedSuffixes;
}

void QtUploadArea::setAcceptedSuffixes(const QStringList& value)
{
    Q_D(QtUploadArea);
    if(d->acceptedSuffixes != value)
    {
        d->acceptedSuffixes = value;
        Q_EMIT acceptedSuffixesChanged();
    }
}

int QtUploadArea::maxFileCount() const
{
    Q_D(const QtUploadArea);
    return d->maxFileCount;
}

void QtUploadArea::setMaxFileCount(int value)
{
    Q_D(QtUploadArea);
    if(d->maxFileCount != value)
    {
        d->maxFileCount = value;
        Q_EMIT maxFileCountChanged();
    }
}

qint64 QtUploadArea::maxFileSize() const
{
    Q_D(const QtUploadArea);
    return d->maxFileSize;
}

void QtUploadArea::setMaxFileSize(qint64 value)
{
    Q_D(QtUploadArea);
    if(d->maxFileSize != value)
    {
        d->maxFileSize = value;
        Q_EMIT maxFileSizeChanged();
    }
}

bool QtUploadArea::isMultiple() const
{
    Q_D(const QtUploadArea);
    return d->isMultiple;
}

void QtUploadArea::setIsMultiple(bool value)
{
    Q_D(QtUploadArea);
    if(d->isMultiple != value)
    {
        d->isMultiple = value;
        Q_EMIT isMultipleChanged();
    }
}

const QString& QtUploadArea::dialogTitle() const
{
    Q_D(const QtUploadArea);
    return d->dialogTitle;
}

void QtUploadArea::setDialogTitle(const QString& value)
{
    Q_D(QtUploadArea);
    if(d->dialogTitle != value)
    {
        d->dialogTitle = value;
        Q_EMIT dialogTitleChanged();
    }
}