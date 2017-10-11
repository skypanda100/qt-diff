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
    delete m_folder_button;
    delete m_refresh_button;
}

void ZTopWidget::initData()
{

}

void ZTopWidget::initUI()
{
    m_folder_button = new QToolButton;
    QIcon folder_icon(":/icon/folder.png");
    m_folder_button->setIcon(folder_icon);
    m_folder_button->setText("New folder\ncomparison");
    m_folder_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_folder_button->setFixedSize(QSize(80, 100));

    m_refresh_button = new QToolButton;
    QIcon refresh_icon(":/icon/refresh.png");
    m_refresh_button->setIcon(refresh_icon);
    m_refresh_button->setText("Start or\nrecompare");
    m_refresh_button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_refresh_button->setFixedSize(QSize(80, 100));

    this->addWidget(m_folder_button);
    this->addWidget(m_refresh_button);
    this->setIconSize(QSize(60, 60));
}

void ZTopWidget::initConnect()
{

}
