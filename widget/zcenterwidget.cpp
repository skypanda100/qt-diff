#include "zcenterwidget.h"
#include "zfolderwidget.h"

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

void ZTabBar::paintEvent(QPaintEvent *event)
{
    QTabBar::paintEvent(event);

    float progress = (float)((mValue * 1.0f) / (mMaxValue * 1.0f));
    if(progress < 1.0f)
    {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(0, 128, 0, 100)));

        int width = this->width() * progress;
        int height = this->height();

        painter.drawRect(0, 0, width, height);
    }
}

void ZTabBar::onProgress(int value, int maxValue)
{
    if(maxValue == 0)
    {
        mValue = 0;
        mMaxValue = 1;
    }
    else if(value > maxValue)
    {
        mValue = maxValue;
        mMaxValue = maxValue;
    }
    else
    {
        mValue = value;
        mMaxValue = maxValue;
    }
    update();
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
    connect(folderWidget, SIGNAL(progress(int,int)), this, SIGNAL(progress(int,int)));
    addTab(folderWidget, QIcon(":/icon/folder.png"), "Folder comparison");
    connect(folderWidget, SIGNAL(progress(int,int)), (ZTabBar *)(this->tabBar()), SLOT(onProgress(int,int)));
}

void ZCenterWidget::startOrRecompare()
{
    ZFolderWidget *widget = (ZFolderWidget *)(this->currentWidget());
    widget->compare();
}

void ZCenterWidget::stopCompare()
{
    ZFolderWidget *widget = (ZFolderWidget *)(this->currentWidget());
    widget->stopCompare();
}

void ZCenterWidget::paintEvent(QPaintEvent *event){
    QTabWidget::paintEvent(event);
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

