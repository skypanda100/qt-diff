#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initData();
    initUI();
    initConnect();
}

MainWindow::~MainWindow()
{
    delete mTopWidget;
    delete mCenterWidget;
    delete mBottomWidget;
}

void MainWindow::initData()
{

}

void MainWindow::initUI()
{
    mTopWidget = new ZTopWidget;
    this->addToolBar(mTopWidget);

    mCenterWidget = new ZCenterWidget;
    this->setCentralWidget(mCenterWidget);

    mBottomWidget = new ZBottomWidget;
    this->setStatusBar(mBottomWidget);
}

void MainWindow::initConnect()
{
    connect(mTopWidget, SIGNAL(folderComparison()), mCenterWidget, SLOT(folderComparison()));
    connect(mTopWidget, SIGNAL(startOrRecompare()), mCenterWidget, SLOT(startOrRecompare()));
    connect(mTopWidget, SIGNAL(stopCompare()), mCenterWidget, SLOT(stopCompare()));
}
