#include "zfolderwidget.h"
#include "tree/ztreemodel.h"

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
