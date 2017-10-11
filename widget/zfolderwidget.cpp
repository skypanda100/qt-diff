#include "zfolderwidget.h"
#include "tree/ztreemodel.h"
#include "diff/zpathdiff.h"
#include "diff/zpathdiffmodel.h"
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"

ZFolderWidget::ZFolderWidget(QWidget *parent)
    :QWidget(parent)
{
    initData();
    initUI();
    initConnect();
}

ZFolderWidget::~ZFolderWidget()
{
    delete mPathEditSrc;
    delete mPathEditDst;
    delete mSearchButtonSrc;
    delete mSearchButtonDst;
    delete mTreeView;
}

void ZFolderWidget::compare()
{
    clearAll();

    QString srcBasePath = mPathEditSrc->text().trimmed();
    QString dstBasePath = mPathEditDst->text().trimmed();

    ZPathDiff pathDiff(srcBasePath, dstBasePath);
    QList<ZPathDiffModel> pathModelLst = pathDiff.execute();

    for(int i = 0;i < pathModelLst.size();i++)
    {
        ZPathDiffModel pathDiffModel = pathModelLst[i];
        int no = i + 1;
        QString path;
        QString extension;
        int status = pathDiffModel.status();
        if(status == 0)
        {
            path = pathDiffModel.srcFileInfo().absoluteFilePath().remove(0, srcBasePath.length());
            extension = pathDiffModel.srcFileInfo().completeSuffix();
        }
        else if(status == 2)
        {
            path = pathDiffModel.srcFileInfo().absoluteFilePath().remove(0, srcBasePath.length());
            extension = pathDiffModel.srcFileInfo().completeSuffix();
        }
        else if(status == 3)
        {
            path = pathDiffModel.dstFileInfo().absoluteFilePath().remove(0, dstBasePath.length());
            extension = pathDiffModel.dstFileInfo().completeSuffix();
        }
        else
        {

        }


        QModelIndex index = mTreeView->model()->index(i - 1, 0);
        QAbstractItemModel *model = mTreeView->model();

        if (!model->insertRow(index.row() + 1, index.parent()))
        {
            return;
        }
        QModelIndex child1 = model->index(index.row() + 1, 0, index.parent());
        model->setData(child1, QVariant(no), Qt::DisplayRole);

        QModelIndex child2 = model->index(index.row() + 1, 1, index.parent());
        model->setData(child2, QVariant(path), Qt::DisplayRole);

        QModelIndex child3 = model->index(index.row() + 1, 2, index.parent());
        model->setData(child3, QVariant(extension), Qt::DisplayRole);

        QModelIndex child4 = model->index(index.row() + 1, 3, index.parent());
        model->setData(child4, QVariant(status), Qt::DisplayRole);
    }
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
    mPathEditSrc = new QLineEdit;
    mPathEditSrc->setPlaceholderText("Folder");
    mSearchButtonSrc = new QPushButton;
    mSearchButtonSrc->setIcon(QIcon(":/icon/search.png"));
    mSearchButtonSrc->setToolTip("search");

    mPathEditDst = new QLineEdit;
    mPathEditDst->setPlaceholderText("Folder");
    mSearchButtonDst = new QPushButton;
    mSearchButtonDst->setIcon(QIcon(":/icon/search.png"));
    mSearchButtonDst->setToolTip("search");

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(mPathEditSrc, 1);
    searchLayout->addWidget(mSearchButtonSrc);
    searchLayout->addSpacing(10);
    searchLayout->addWidget(mPathEditDst, 1);
    searchLayout->addWidget(mSearchButtonDst);

    mTreeView = new QTreeView;
    ZTreeModel *model = new ZTreeModel(mHeader, "");
    mTreeView->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column){
        mTreeView->resizeColumnToContents(column);
    }
    mTreeView->setAlternatingRowColors(true);

    QVBoxLayout *folderLayout = new QVBoxLayout;
    folderLayout->addLayout(searchLayout);
    folderLayout->addWidget(mTreeView, 1);
    setLayout(folderLayout);
}

void ZFolderWidget::initConnect()
{
    connect(mSearchButtonSrc, SIGNAL(clicked()), this, SLOT(searchClicked()));
    connect(mSearchButtonDst, SIGNAL(clicked()), this, SLOT(searchClicked()));
}

void ZFolderWidget::clearAll()
{
    QAbstractItemModel *model = mTreeView->model();
    if(model->rowCount() > 0){
        model->removeRows(0, model->rowCount());
    }
}

void ZFolderWidget::searchClicked()
{
    QObject *object = this->sender();
    if(object == mSearchButtonSrc)
    {
        QString currentDir = mPathEditSrc->text().isEmpty() ? QDir::currentPath() : mPathEditSrc->text();
        QString directory = QFileDialog::getExistingDirectory(this,
                                   tr("search"), currentDir);
        mPathEditSrc->setText(directory);
    }
    else
    {
        QString currentDir = mPathEditDst->text().isEmpty() ? QDir::currentPath() : mPathEditDst->text();
        QString directory = QFileDialog::getExistingDirectory(this,
                                   tr("search"), currentDir);
        mPathEditDst->setText(directory);
    }
}