#pragma once

#include "qtwidgetsextra.h"
#include "widgets/qtappbar.h"
#include <QWidget>
#include <qdialog.h>
#include <qlineedit.h>
#include <qtmetamacros.h>

class QtInputDialogPrivate;
class QTWIDGETSEXTRA_EXPORT QtInputDialog : public QDialog
{
    Q_OBJECT
    Q_TAKEOVER_NATIVEEVENT_H
    Q_PROPERTY(QString titleText READ titleText WRITE setTitleText NOTIFY titleTextChanged)
    Q_PROPERTY(
        QString subTitleText READ subTitleText WRITE setSubTitleText NOTIFY subTitleTextChanged)
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText NOTIFY labelTextChanged)
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)
    Q_PROPERTY(int intValue READ intValue WRITE setIntValue NOTIFY intValueChanged)
    Q_PROPERTY(double doubleValue READ doubleValue WRITE setDoubleValue NOTIFY doubleValueChanged)
    Q_PROPERTY(
        QString okButtonText READ okButtonText WRITE setOkButtonText NOTIFY okButtonTextChanged)
    Q_PROPERTY(QString cancelButtonText READ cancelButtonText WRITE setCancelButtonText NOTIFY
            cancelButtonTextChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText NOTIFY
            placeholderTextChanged)
    Q_PROPERTY(int inputMinimumWidth READ inputMinimumWidth WRITE setInputMinimumWidth NOTIFY
            inputMinimumWidthChanged)
    Q_PROPERTY(int inputMaximumWidth READ inputMaximumWidth WRITE setInputMaximumWidth NOTIFY
            inputMaximumWidthChanged)
public:
    explicit QtInputDialog(QWidget* parent = nullptr);
    ~QtInputDialog();

    static QString textInput(QWidget* parent, const QString& title, const QString& subtitle,
        const QString& label, const QString& text = QString(), bool* ok = nullptr,
        const QString& okButtonText = "OK", const QString& cancelButtonText = "Cancel",
        int inputMinWidth = 100, int inputMaxWidth = QWIDGETSIZE_MAX);

    static int intInput(QWidget* parent, const QString& title, const QString& subtitle,
        const QString& label, int value = 0, int minValue = -2147483647, int maxValue = 2147483647,
        int step = 1, bool* ok = nullptr, const QString& okButtonText = "OK",
        const QString& cancelButtonText = "Cancel", int inputMinWidth = 100,
        int inputMaxWidth = QWIDGETSIZE_MAX);

    static double doubleInput(QWidget* parent, const QString& title, const QString& subtitle,
        const QString& label, double value = 0.0, double minValue = -2147483647.0,
        double maxValue = 2147483647.0, int decimals = 2, bool* ok = nullptr,
        const QString& okButtonText = "OK", const QString& cancelButtonText = "Cancel",
        int inputMinWidth = 100, int inputMaxWidth = QWIDGETSIZE_MAX);

    static QString multiLineTextInput(QWidget* parent, const QString& title,
        const QString& subtitle, const QString& label, const QString& text = QString(),
        bool* ok = nullptr, const QString& okButtonText = "OK",
        const QString& cancelButtonText = "Cancel", int inputMinWidth = 100,
        int inputMaxWidth = QWIDGETSIZE_MAX);

    void setTextEchoMode(QLineEdit::EchoMode mode);
    QLineEdit::EchoMode textEchoMode() const;

    void setIntRange(int minValue, int maxValue, int step = 1);
    void setDoubleRange(double minValue, double maxValue, int decimals = 2);
    void setMultiLine(bool multiLine);

    QString titleText() const;
    void setTitleText(QString value);

    QString subTitleText() const;
    void setSubTitleText(QString value);

    QString labelText() const;
    void setLabelText(QString value);

    QString textValue() const;
    void setTextValue(QString value);

    int intValue() const;
    void setIntValue(int value);

    double doubleValue() const;
    void setDoubleValue(double value);

    QString okButtonText() const;
    void setOkButtonText(QString value);

    QString cancelButtonText() const;
    void setCancelButtonText(QString value);

    QString placeholderText() const;
    void setPlaceholderText(QString value);

    int inputMinimumWidth() const;
    void setInputMinimumWidth(int value);

    int inputMaximumWidth() const;
    void setInputMaximumWidth(int value);

Q_SIGNALS:

    void textValueChanged(QString text);
    void intValueChanged(int value);
    void doubleValueChanged(double value);

    void titleTextChanged();
    void subTitleTextChanged();
    void labelTextChanged();
    void textValueChanged();
    void intValueChanged();
    void doubleValueChanged();
    void okButtonTextChanged();
    void cancelButtonTextChanged();
    void placeholderTextChanged();
    void inputMinimumWidthChanged();
    void inputMaximumWidthChanged();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

private:
    void updateLabels();

    QScopedPointer<class QtInputDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtInputDialog)
};
