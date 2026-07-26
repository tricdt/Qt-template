#ifndef T_TREEITEM_H
#define T_TREEITEM_H

#include <QModelIndex>
#include <QObject>
#include <qtmetamacros.h>

#include "QtProperty.h"
class T_TreeItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<T_TreeItem*> childrenItems READ childrenItems WRITE setChildrenItems NOTIFY
            childrenItemsChanged);
    Q_PROPERTY(bool isChecked READ isChecked WRITE setIsChecked NOTIFY isCheckedChanged);
    Q_PROPERTY(T_TreeItem* parentItem READ parentItem WRITE setParentItem NOTIFY parentItemChanged);

public:
    explicit T_TreeItem(QString itemTitle, T_TreeItem* parent = nullptr);
    ~T_TreeItem();

    QList<T_TreeItem*> childrenItems() const;
    void setChildrenItems(QList<T_TreeItem*> value);

    bool isChecked() const;
    void setIsChecked(bool value);

    T_TreeItem* parentItem() const;
    void setParentItem(T_TreeItem* value);

    QString itemKey() const;
    QString itemTitle() const;

    void setChildChecked(bool isChecked);
    Qt::CheckState childCheckState();

    void appendChildItem(T_TreeItem* childItem);
    bool isHasChild() const;
    int row() const;

Q_SIGNALS:
    void childrenItemsChanged();
    void isCheckedChanged();
    void parentItemChanged();

private:
    QList<T_TreeItem*> _childrenItems;
    bool _isChecked;
    T_TreeItem* _parentItem;
    QString _itemKey = "";
    QString _itemTitle = "";
    bool _isExpanded{ false };
};

#endif // T_TREEITEM_H
