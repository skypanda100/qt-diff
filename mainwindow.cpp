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
}

void MainWindow::initConnect()
{
    connect(mTopWidget, SIGNAL(folderComparison()), mCenterWidget, SLOT(folderComparison()));
    connect(mTopWidget, SIGNAL(startOrRecompare()), mCenterWidget, SLOT(startOrRecompare()));

}
