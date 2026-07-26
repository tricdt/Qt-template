#pragma once

#include "qtwidgetsextra.h"
#include <QWidget>

class QtSplitterPrivate;
class QTWIDGETSEXTRA_EXPORT QtSplitter : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int handleWidth READ handleWidth WRITE setHandleWidth NOTIFY handleWidthChanged)
    Q_PROPERTY(int gripLength READ gripLength WRITE setGripLength NOTIFY gripLengthChanged)
public:
    explicit QtSplitter(QWidget* parent = nullptr);
    ~QtSplitter();

    int handleWidth() const;
    void setHandleWidth(int value);

    int gripLength() const;
    void setGripLength(int value);

Q_SIGNALS:
    void handleWidthChanged();
    void gripLengthChanged();

private:
    QScopedPointer<class QtSplitterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QtSplitter)
};
