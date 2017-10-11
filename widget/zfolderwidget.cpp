#include "zfolderwidget.h"

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
    m_header << ""
}

void ZFolderWidget::initUI()
{

}

void ZFolderWidget::initConnect()
{

}
