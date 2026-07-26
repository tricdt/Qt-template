#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtMarkdownViewerPrivate;
class QTWIDGETSEXTRA_EXPORT QtMarkdownViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString markdown READ markdown WRITE setMarkdown NOTIFY markdownChanged)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY borderRadiusChanged)
public:
    explicit QtMarkdownViewer(QWidget* parent = nullptr);
    ~QtMarkdownViewer();

    const QString& markdown() const;
    void setMarkdown(const QString& value);

    int borderRadius() const;
    void setBorderRadius(int value);

Q_SIGNALS:
    void markdownChanged();
    void borderRadiusChanged();

private:
    QScopedPointer<class QtMarkdownViewerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtMarkdownViewer)
};
