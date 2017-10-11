#include "ztreeitem.h"

#include <QStringList>

//! [0]
ZTreeItem::ZTreeItem(const QVector<QVariant> &data, ZTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}
//! [0]

//! [1]
ZTreeItem::~ZTreeItem()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
ZTreeItem *ZTreeItem::child(int number)
{
    return childItems.value(number);
}
//! [2]

//! [3]
int ZTreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int ZTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<ZTreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int ZTreeItem::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant ZTreeItem::data(int column) const
{
    return itemData.value(column);
}
//! [6]

//! [7]
bool ZTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        ZTreeItem *item = new ZTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}
//! [7]

//! [8]
bool ZTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (ZTreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
ZTreeItem *ZTreeItem::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool ZTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
//! [10]

bool ZTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (ZTreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

//! [11]
bool ZTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
//! [11]
