#include "zfolderwidget.h"
#include "tree/ztreemodel.h"

ZFolderWidget::ZFolderWidget(QWidget *parent)
    :QTreeView(parent)
{
    initData();
    initUI();
    initConnect();
}

ZFolderWidget::~ZFolderWidget()
{

}

void ZFolderWidget::initData()
{
    mHeader << "#"
            << "Path"
            << "Extension"
            << "Status"
            << "Lines added"
            << "Lines removed"
            << "Lines modified";
}

void ZFolderWidget::initUI()
{
    ZTreeModel *model = new ZTreeModel(mHeader, "");
    setModel(model);
    for (int column = 0; column < model->columnCount(); ++column){
        resizeColumnToContents(column);
    }
    setAlternatingRowColors(true);
}

void ZFolderWidget::initConnect()
{

}
