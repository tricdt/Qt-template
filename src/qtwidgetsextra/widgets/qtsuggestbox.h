#pragma once

#include "qtdef.h"
#include "qtwidgetsextra.h"
#include <QWidget>

class QtSuggestBoxPrivate;
struct QtSuggestDataPrivate;
class QTWIDGETSEXTRA_EXPORT QtSuggestBox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity
            NOTIFY caseSensitivityChanged)
public:
    explicit QtSuggestBox(QWidget* parent = nullptr);
    ~QtSuggestBox();

    void setPlaceholderText(const QString& placeholderText);
    void setFixedSize(const QSize& size);
    void setFixedSize(int w, int h);
    void setFixedHeight(int h);

    struct QTWIDGETSEXTRA_EXPORT SuggestData
    {
    public:
        explicit SuggestData();
        explicit SuggestData(QtIconType::IconName icon, const QString& suggestText,
            const QVariantMap& suggestData = {});
        ~SuggestData();

        QtIconType::IconName qtIcon() const;
        void setQtIcon(QtIconType::IconName value);

        QString suggestText() const;
        void setSuggestText(QString value);

        QString suggestKey() const;
        void setSuggestKey(QString value);

        QVariantMap suggestData() const;
        void setSuggestData(QVariantMap value);

    private:
        QtIconType::IconName _qtIcon;
        QString _suggestText;
        QString _suggestKey;
        QVariantMap _suggestData;
    };

    int borderRadius() const;
    void setBorderRadius(int value);

    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity value);

Q_SIGNALS:
    void borderRadiusChanged();
    void caseSensitivityChanged();

private:
    QScopedPointer<class QtSuggestBoxPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSuggestBox)
};
