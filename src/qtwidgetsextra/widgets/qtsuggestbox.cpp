#include "qtsuggestbox.h"

class QtSuggestBoxPrivate
{
    Q_DECLARE_PUBLIC(QtSuggestBox)
public:
    int borderRadius;
    Qt::CaseSensitivity caseSensitivity;
    QtSuggestBox* q_ptr;
    QtSuggestBoxPrivate(QtSuggestBox* q);
    ~QtSuggestBoxPrivate();
};

QtSuggestBoxPrivate::QtSuggestBoxPrivate(QtSuggestBox* q)
    : q_ptr(q)
{
}
QtSuggestBoxPrivate::~QtSuggestBoxPrivate()
{
}

QtSuggestBox::SuggestData::SuggestData()
{
}
QtSuggestBox::SuggestData::SuggestData(
    QtIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData)
    : _qtIcon(icon)
    , _suggestText(suggestText)
    , _suggestData(suggestData)
{
}
QtSuggestBox::SuggestData::~SuggestData()
{
}

QtSuggestBox::QtSuggestBox(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtSuggestBoxPrivate(this))
{
}

QtSuggestBox::~QtSuggestBox()
{
}

void QtSuggestBox::setPlaceholderText(const QString& placeholderText)
{
}
void QtSuggestBox::setFixedSize(const QSize& size)
{
}
void QtSuggestBox::setFixedSize(int w, int h)
{
}
void QtSuggestBox::setFixedHeight(int h)
{
}

QtIconType::IconName QtSuggestBox::SuggestData::qtIcon() const
{
    return _qtIcon;
}
void QtSuggestBox::SuggestData::setQtIcon(QtIconType::IconName value)
{
    _qtIcon = value;
}

QString QtSuggestBox::SuggestData::suggestText() const
{
    return _suggestText;
}
void QtSuggestBox::SuggestData::setSuggestText(QString value)
{
    _suggestText = value;
}

QString QtSuggestBox::SuggestData::suggestKey() const
{
    return _suggestKey;
}
void QtSuggestBox::SuggestData::setSuggestKey(QString value)
{
    _suggestKey = value;
}

QVariantMap QtSuggestBox::SuggestData::suggestData() const
{
    return _suggestData;
}
void QtSuggestBox::SuggestData::setSuggestData(QVariantMap value)
{
    _suggestData = value;
}

int QtSuggestBox::borderRadius() const
{
    Q_D(const QtSuggestBox);
    return d->borderRadius;
}
void QtSuggestBox::setBorderRadius(int value)
{
    Q_D(QtSuggestBox);
    d->borderRadius = value;
}

Qt::CaseSensitivity QtSuggestBox::caseSensitivity() const
{
    Q_D(const QtSuggestBox);
    return d->caseSensitivity;
}
void QtSuggestBox::setCaseSensitivity(Qt::CaseSensitivity value)
{
    Q_D(QtSuggestBox);
    d->caseSensitivity = value;
}