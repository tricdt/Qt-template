#include "qtmarkdownviewer.h"

class QtMarkdownViewerPrivate
{
    Q_DECLARE_PUBLIC(QtMarkdownViewer)
public:
QString markdown;
    int borderRadius;
    QtMarkdownViewer* q_ptr;
    QtMarkdownViewerPrivate(QtMarkdownViewer* q);
    ~QtMarkdownViewerPrivate();
};

QtMarkdownViewerPrivate::QtMarkdownViewerPrivate(QtMarkdownViewer* q) : q_ptr(q)
{
}
QtMarkdownViewerPrivate::~QtMarkdownViewerPrivate()
{
}

QtMarkdownViewer::QtMarkdownViewer(QWidget* parent)
    : QWidget{ parent }
    , d_ptr(new QtMarkdownViewerPrivate(this))
{
}

QtMarkdownViewer::~QtMarkdownViewer()
{
}

const QString& QtMarkdownViewer::markdown() const
{
    Q_D(const QtMarkdownViewer);
    return d->markdown;
}

void QtMarkdownViewer::setMarkdown(const QString &value)
{
    Q_D(QtMarkdownViewer);
    if (d->markdown != value) {
        d->markdown = value;
        Q_EMIT markdownChanged();
    }
}

int QtMarkdownViewer::borderRadius() const
{
    Q_D(const QtMarkdownViewer);
    return d->borderRadius;
}

void QtMarkdownViewer::setBorderRadius(int value)
{
    Q_D(QtMarkdownViewer);
    if (d->borderRadius != value) {
        d->borderRadius = value;
        Q_EMIT borderRadiusChanged();
    }
}