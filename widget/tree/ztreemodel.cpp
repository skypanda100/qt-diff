#include <QtWidgets>

#include "ztreeitem.h"
#include "ztreemodel.h"
#include "env/zcons.h"
#include "util/zfile.h"
#include "ztreeitemmodel.h"

//! [0]
ZTreeModel::ZTreeModel(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new ZTreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}
//! [0]

//! [1]
ZTreeModel::~ZTreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int ZTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QVariant ZTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    ZTreeItem *item = getItem(index);
    QVariant dataVariant = item->data(index.column());
    ZTreeItemModel itemModel = dataVariant.value<ZTreeItemModel>();

    if(role == Qt::DecorationRole && itemModel.hasIcon())
    {
        return itemModel.icon();
    }

    if(role == Qt::TextColorRole)
    {
        return itemModel.color();
    }

    if(role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant();
    }

    return itemModel.value();
}

//! [3]
Qt::ItemFlags ZTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//! [3]

//! [4]
ZTreeItem *ZTreeModel::getItem(const QModelIndex &index) const
{
    if(index.isValid())
    {
        ZTreeItem *item = static_cast<ZTreeItem*>(index.internalPointer());
        if(item)
            return item;
    }
    return rootItem;
}
//! [4]

QVariant ZTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex ZTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]

//! [6]
    ZTreeItem *parentItem = getItem(parent);

    ZTreeItem *childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool ZTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool ZTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    ZTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

//! [7]
QModelIndex ZTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    ZTreeItem *childItem = getItem(index);
    ZTreeItem *parentItem = childItem->parent();

    if(parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool ZTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if(rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool ZTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    ZTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int ZTreeModel::rowCount(const QModelIndex &parent) const
{
    ZTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool ZTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QVector<int> roles;
    roles.append(role);
    ZTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if(result)
        emit dataChanged(index, index, roles);

    return result;
}

bool ZTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if(role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if(result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void ZTreeModel::setupModelData(const QStringList &lines, ZTreeItem *parent)
{
    QList<ZTreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if(lines[number].mid(position, 1) != " ")
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if(!lineData.isEmpty())
        {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if(position > indentations.last())
            {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if(parents.last()->childCount() > 0)
                {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            }
            else
            {
                while(position < indentations.last() && parents.count() > 0)
                {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            ZTreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}
