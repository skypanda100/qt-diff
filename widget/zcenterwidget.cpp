#include "zcenterwidget.h"
#include "widget/folder/zfolderwidget.h"
#include "widget/file/zfilewidget.h"

ZCenterWidget::ZCenterWidget(QWidget *parent)
    :QWidget(parent)
{
    initData();
    initUI();
    initConnect();
}

ZCenterWidget::~ZCenterWidget()
{
    delete mTabWidget;
}

void ZCenterWidget::closeTab(int index)
{
    if(index < 0)
    {
        index = mTabWidget->currentIndex();
    }

    if(index < 0 || index >= mTabWidget->count())
    {
        return;
    }

    if(QString::compare(mTabWidget->widget(index)->objectName(), OBJECT_FOLDER_COMPARISON) == 0)
    {
        ZFolderWidget *widget = (ZFolderWidget *)(mTabWidget->widget(index));
        mTabWidget->removeTab(index);
        delete widget;
    }
    else
    {
        ZFileWidget *widget = (ZFileWidget *)(mTabWidget->widget(index));
        mTabWidget->removeTab(index);
        delete widget;
    }
}

void ZCenterWidget::folderComparison()
{
    QString title = QString("Folder comparison:%1").arg(mFolderCount);
    mFolderCount++;
    ZFolderWidget *folderWidget = new ZFolderWidget;
    folderWidget->setObjectName(OBJECT_FOLDER_COMPARISON);
    connect(folderWidget, SIGNAL(fileCompare(ZPathDiffModel)), this, SLOT(fileCompare(ZPathDiffModel)));
    int index = mTabWidget->addTab(folderWidget, QIcon(":/icon/folder.png"), title);
    mTabWidget->setCurrentIndex(index);
}

void ZCenterWidget::fileComparison()
{
    ZPathDiffModel pathDiffModel;
    pathDiffModel.setStatus(Same);
    fileCompare(pathDiffModel);
}

void ZCenterWidget::startOrRecompare()
{
    if(mTabWidget->currentWidget() == NULL)
    {
        return;
    }
    if(QString::compare(mTabWidget->currentWidget()->objectName(), OBJECT_FOLDER_COMPARISON) == 0)
    {
        ZFolderWidget *widget = (ZFolderWidget *)(mTabWidget->currentWidget());
        if(widget != NULL)
        {
            widget->compare();
        }
    }
    else if(QString::compare(mTabWidget->currentWidget()->objectName(), OBJECT_FILE_COMPARISON) == 0)
    {
        ZFileWidget *widget = (ZFileWidget *)(mTabWidget->currentWidget());
        if(widget != NULL)
        {
            widget->compare();
        }
    }
}

void ZCenterWidget::stopCompare()
{
    if(mTabWidget->currentWidget() == NULL)
    {
        return;
    }
    if(QString::compare(mTabWidget->currentWidget()->objectName(), OBJECT_FOLDER_COMPARISON) == 0)
    {
        ZFolderWidget *widget = (ZFolderWidget *)(mTabWidget->currentWidget());
        if(widget != NULL)
        {
            widget->stopCompare();
        }
    }
}

void ZCenterWidget::paintEvent(QPaintEvent *event){
    QWidget::paintEvent(event);
}

void ZCenterWidget::fileCompare(ZPathDiffModel pathDiffModel)
{
    ZFileWidget *fileWidget = new ZFileWidget(pathDiffModel);
    fileWidget->setObjectName(OBJECT_FILE_COMPARISON);
    QString title;
    QString srcFileName = pathDiffModel.srcFileInfo().fileName();
    QString dstFileName = pathDiffModel.dstFileInfo().fileName();
    if(srcFileName.isEmpty() && dstFileName.isEmpty())
    {
        title = QString("Text comparison:%1").arg(mTextCount);
        mTextCount++;
    }
    else if(srcFileName.isEmpty() && !dstFileName.isEmpty())
    {
        title = dstFileName;
    }
    else
    {
        title = srcFileName;
    }
    int index = mTabWidget->addTab(fileWidget, QIcon(":/icon/file.png"), title);
    mTabWidget->setCurrentIndex(index);
}

void ZCenterWidget::initData()
{
    mTextCount = 1;
    mFolderCount = 1;
}

void ZCenterWidget::initUI()
{
    mTabWidget = new QTabWidget;
    mTabWidget->setElideMode(Qt::ElideRight);
    mTabWidget->setDocumentMode(true);
    mTabWidget->setTabsClosable(true);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->setContentsMargins(0, STANDARD_MARGIN, 0, 0);
    tabLayout->addWidget(mTabWidget);
    this->setLayout(tabLayout);
}

void ZCenterWidget::initConnect()
{
    connect(mTabWidget->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

