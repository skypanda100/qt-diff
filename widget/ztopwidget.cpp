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
    delete mStopButton;
}

void ZTopWidget::initData()
{

}

void ZTopWidget::initUI()
{
    mFolderButton = new QToolButton;
    QIcon folderIcon(":/icon/folder.png");
    mFolderButton->setIcon(folderIcon);
    mFolderButton->setToolTip("New folder comparison");
    mFolderButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
//    mFolderButton->setFixedSize(QSize(50, 50));

    mRefreshButton = new QToolButton;
    QIcon refreshIcon(":/icon/refresh.png");
    mRefreshButton->setIcon(refreshIcon);
    mRefreshButton->setToolTip("Start or recompare");
    mRefreshButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
//    mRefreshButton->setFixedSize(QSize(50, 50));

    mStopButton = new QToolButton;
    QIcon stopIcon(":/icon/stop.png");
    mStopButton->setIcon(stopIcon);
    mStopButton->setToolTip("Stop");
    mStopButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
//    mStopButton->setFixedSize(QSize(50, 50));

    this->addWidget(mFolderButton);
    this->addWidget(mRefreshButton);
    this->addWidget(mStopButton);
    this->setIconSize(QSize(50, 50));
}

void ZTopWidget::initConnect()
{
    connect(mFolderButton, SIGNAL(clicked()), this, SIGNAL(folderComparison()));
    connect(mRefreshButton, SIGNAL(clicked()), this, SIGNAL(startOrRecompare()));
    connect(mStopButton, SIGNAL(clicked()), this, SIGNAL(stopCompare()));
}
