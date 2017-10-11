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
    delete m_top_widget;
    delete m_center_widget;
}

void MainWindow::initData()
{

}

void MainWindow::initUI()
{
    m_top_widget = new ZTopWidget;
    this->addToolBar(m_top_widget);

    m_center_widget = new ZCenterWidget;
    this->setCentralWidget(m_center_widget);
}

void MainWindow::initConnect()
{

}
