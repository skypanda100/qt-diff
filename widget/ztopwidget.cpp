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
    delete mFileButton;
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
    mFolderButton->setText("New folder\ncomparison");
    mFolderButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mFolderButton->setFixedSize(QSize(95, 100));

    mFileButton = new QToolButton;
    QIcon fileIcon(":/icon/file.png");
    mFileButton->setIcon(fileIcon);
    mFileButton->setText("New text\ncomparison");
    mFileButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mFileButton->setFixedSize(QSize(95, 100));

    mRefreshButton = new QToolButton;
    QIcon refreshIcon(":/icon/refresh.png");
    mRefreshButton->setIcon(refreshIcon);
    mRefreshButton->setText("Start or\nrecompare");
    mRefreshButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mRefreshButton->setFixedSize(QSize(95, 100));

    mStopButton = new QToolButton;
    QIcon stopIcon(":/icon/stop.png");
    mStopButton->setIcon(stopIcon);
    mStopButton->setText("Stop");
    mStopButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mStopButton->setFixedSize(QSize(95, 100));

    this->addWidget(mFolderButton);
    this->addWidget(mFileButton);
    this->addWidget(mRefreshButton);
    this->addWidget(mStopButton);
    this->setIconSize(QSize(45, 45));
}

void ZTopWidget::initConnect()
{
    connect(mFolderButton, SIGNAL(clicked()), this, SIGNAL(folderComparison()));
    connect(mFileButton, SIGNAL(clicked()), this, SIGNAL(fileComparison()));
    connect(mRefreshButton, SIGNAL(clicked()), this, SIGNAL(startOrRecompare()));
    connect(mStopButton, SIGNAL(clicked()), this, SIGNAL(stopCompare()));
}
