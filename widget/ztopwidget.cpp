#include "ztopwidget.h"

ZTopWidget::ZTopWidget(QWidget *parent)
    :QToolBar(parent)
{
    initData();
    initUI();
    initConnect();
}

ZTopWidget::~ZTopWidget()
{
    delete mFolderButton;
    delete mRefreshButton;
}

void ZTopWidget::initData()
{

}

void ZTopWidget::initUI()
{
    mFolderButton = new QToolButton;
    QIcon folderIcon(":/icon/folder.png");
    mFolderButton->setIcon(folderIcon);
    mFolderButton->setText("New folder\ncomparison");
    mFolderButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mFolderButton->setFixedSize(QSize(80, 100));

    mRefreshButton = new QToolButton;
    QIcon refreshIcon(":/icon/refresh.png");
    mRefreshButton->setIcon(refreshIcon);
    mRefreshButton->setText("Start or\nrecompare");
    mRefreshButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mRefreshButton->setFixedSize(QSize(80, 100));

    this->addWidget(mFolderButton);
    this->addWidget(mRefreshButton);
    this->setIconSize(QSize(60, 60));
}

void ZTopWidget::initConnect()
{
    connect(mFolderButton, SIGNAL(clicked()), this, SIGNAL(folderComparison()));
}
