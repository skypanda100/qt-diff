#ifndef ZTREEITEM
#define ZTREEITEM

#include <QList>
#include <QVariant>
#include <QVector>

//! [0]
class ZTreeItem
{
public:
    explicit ZTreeItem(const QVector<QVariant> &data, ZTreeItem *parent = 0);
    ~ZTreeItem();

    ZTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    ZTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<ZTreeItem*> childItems;
    QVector<QVariant> itemData;
    ZTreeItem *parentItem;
};
//! [0]

#endif // ZTREEITEM

