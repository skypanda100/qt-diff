#include "zcenterwidget.h"

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
        m_drag_start_pos = event->pos();
    QTabBar::mousePressEvent(event);
}

void ZTabBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        int diffX = event->pos().x() - m_drag_start_pos.x();
        int diffY = event->pos().y() - m_drag_start_pos.y();
        if ((event->pos() - m_drag_start_pos).manhattanLength() > QApplication::startDragDistance()
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
    delete m_tab_bar;
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

    QWidget *widget = this->widget(index);
    removeTab(index);
    delete widget;
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
    m_tab_bar = new ZTabBar(this);
    setTabBar(m_tab_bar);
    setDocumentMode(true);
}

void ZCenterWidget::initConnect()
{
    connect(m_tab_bar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));
}

