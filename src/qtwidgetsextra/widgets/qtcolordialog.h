#pragma once

#include "qtappbar.h"
#include "qtwidgetsextra.h"
#include <QWidget>
#include <qdialog.h>
#include <qtmetamacros.h>

class QtColorDialogPrivate;
class QTWIDGETSEXTRA_EXPORT QtColorDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(
        QColor currentColor READ currentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit QtColorDialog(QWidget* parent = nullptr);
    ~QtColorDialog();

    QList<QColor> customColorList() const;
    QColor customColor(int index) const;
    QString currentColorRGB() const;

    QColor currentColor() const;
    void setCurrentColor(QColor value);

Q_SIGNALS:
    void currentColorChanged();
    void colorSelected(const QColor& color);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QScopedPointer<class QtColorDialogPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtColorDialog)
};
