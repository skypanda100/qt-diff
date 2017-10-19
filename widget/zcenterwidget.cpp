#include "zcenterwidget.h"
#include "zfolderwidget.h"
#include "zfilewidget.h"

ZTabBar::ZTabBar(QWidget *parent)
    : QTabBar(parent)
{
    setAcceptDrops(true);
    setMovable(true);
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SIGNAL(closeTab(int)));
}

ZTabBar::~ZTabBar(){

}

void ZTabBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mDragStartPos = event->pos();
    QTabBar::mousePressEvent(event);
}

void ZTabBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        int diffX = event->pos().x() - mDragStartPos.x();
        int diffY = event->pos().y() - mDragStartPos.y();
        if ((event->pos() - mDragStartPos).manhattanLength() > QApplication::startDragDistance()
            && diffX < 3 && diffX > -3
            && diffY < -10) {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            QList<QUrl> urls;
            int index = tabAt(event->pos());
            QUrl url = tabData(index).toUrl();
            urls.append(url);
            mimeData->setUrls(urls);
            mimeData->setText(tabText(index));
            mimeData->setData(QLatin1String("action"), "tab-reordering");
            drag->setMimeData(mimeData);
            drag->exec();
        }
    }
    QTabBar::mouseMoveEvent(event);
}

ZCenterWidget::ZCenterWidget(QWidget *parent)
    :QTabWidget(parent)
{
    initData();
    initUI();
    initConnect();
}

ZCenterWidget::~ZCenterWidget()
{
    delete mTabBar;
}

void ZCenterWidget::closeTab(int index)
{
    if(index < 0)
    {
        index = currentIndex();
    }

    if(index < 0 || index >= count())
    {
        return;
    }

    ZFolderWidget *widget = (ZFolderWidget *)(this->widget(index));
    removeTab(index);
    delete widget;
}

void ZCenterWidget::folderComparison()
{
    ZFolderWidget *folderWidget = new ZFolderWidget;
    folderWidget->setObjectName(OBJECT_FOLDER_COMPARISON);
    connect(folderWidget, SIGNAL(fileCompare(ZPathDiffModel)), this, SLOT(fileCompare(ZPathDiffModel)));
    int index = addTab(folderWidget, QIcon(":/icon/folder.png"), "Folder comparison");
    this->setCurrentIndex(index);
}

void ZCenterWidget::startOrRecompare()
{
    ZFolderWidget *widget = (ZFolderWidget *)(this->currentWidget());
    if(widget != NULL)
    {
        widget->compare();
    }
}

void ZCenterWidget::stopCompare()
{
    ZFolderWidget *widget = (ZFolderWidget *)(this->currentWidget());
    if(widget != NULL)
    {
        widget->stopCompare();
    }
}

void ZCenterWidget::paintEvent(QPaintEvent *event){
    QTabWidget::paintEvent(event);
}

void ZCenterWidget::fileCompare(ZPathDiffModel pathDiffModel)
{
    ZFileWidget *fileWidget = new ZFileWidget(pathDiffModel);
    fileWidget->setObjectName(OBJECT_FILE_COMPARISON);
    int index = addTab(fileWidget, QIcon(":/icon/file.png"), "File comparison");
    this->setCurrentIndex(index);
}

void ZCenterWidget::initData()
{

}

void ZCenterWidget::initUI()
{
    setElideMode(Qt::ElideRight);
    mTabBar = new ZTabBar(this);
    setTabBar(mTabBar);
    setDocumentMode(true);
}

void ZCenterWidget::initConnect()
{
    connect(mTabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
}

