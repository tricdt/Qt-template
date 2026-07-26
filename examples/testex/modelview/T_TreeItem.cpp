#include "T_TreeItem.h"

#include <QUuid>
T_TreeItem::T_TreeItem(QString itemTitle, T_TreeItem* parent)
    : QObject{ parent }
{
    _itemKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _itemTitle = itemTitle;
    _parentItem = parent;
    _isChecked = false;
}

T_TreeItem::~T_TreeItem()
{
    qDeleteAll(_childrenItems);
}

QList<T_TreeItem*> T_TreeItem::childrenItems() const
{
    return _childrenItems;
}
void T_TreeItem::setChildrenItems(QList<T_TreeItem*> value)
{
    _childrenItems = value;
}

bool T_TreeItem::isChecked() const
{
    return _isChecked;
}
void T_TreeItem::setIsChecked(bool value)
{
    _isChecked = value;
}

T_TreeItem* T_TreeItem::parentItem() const
{
    return _parentItem;
}
void T_TreeItem::setParentItem(T_TreeItem* value)
{
    _parentItem = value;
}

QString T_TreeItem::itemKey() const
{
    return _itemKey;
}
QString T_TreeItem::itemTitle() const
{
    return _itemTitle;
}

void T_TreeItem::setChildChecked(bool isChecked)
{
    if(isChecked)
    {
        for(auto node : _childrenItems)
        {
            node->setIsChecked(isChecked);
            node->setChildChecked(isChecked);
        }
    }
    else
    {
        for(auto node : _childrenItems)
        {
            node->setChildChecked(isChecked);
            node->setIsChecked(isChecked);
        }
    }
}
Qt::CheckState T_TreeItem::childCheckState()
{
    bool isAllChecked = true;
    bool isAnyChecked = false;
    for(auto node : _childrenItems)
    {
        if(node->isChecked())
        {
            isAnyChecked = true;
        }
        else
        {
            isAllChecked = false;
        }
        Qt::CheckState childState = node->childCheckState();
        if(childState == Qt::PartiallyChecked)
        {
            isAllChecked = false;
            isAnyChecked = true;
            break;
        }
        else if(childState == Qt::Unchecked)
        {
            isAllChecked = false;
        }
    }
    if(_childrenItems.count() > 0)
    {
        if(isAllChecked)
        {
            return Qt::Checked;
        }
        if(isAnyChecked)
        {
            return Qt::PartiallyChecked;
        }
        return Qt::Unchecked;
    }
    return Qt::Checked;
}

void T_TreeItem::appendChildItem(T_TreeItem* childItem)
{
    _childrenItems.append(childItem);
}
bool T_TreeItem::isHasChild() const
{
    if(_childrenItems.count() > 0)
    {
        return true;
    }
    return false;
}
int T_TreeItem::row() const
{
    if(_parentItem)
    {
        return _parentItem->childrenItems().indexOf(const_cast<T_TreeItem*>(this));
    }
    return 0;
}
