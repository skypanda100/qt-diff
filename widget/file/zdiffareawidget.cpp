#include "zdiffareawidget.h"

ZDiffAreaWidget::ZDiffAreaWidget(ZTextWidget *textWidget, QWidget *parent)
    : QWidget(parent)
    , mTextWidget(textWidget)
{
    initData();
    initUI();
    initConnect();
}

ZDiffAreaWidget::~ZDiffAreaWidget()
{

}

void ZDiffAreaWidget::setDiffList(QList<ZDiffInfo> diffLst)
{
    mDiffLst = diffLst;
    update();
}

void ZDiffAreaWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);
    if(mTextWidget != NULL)
    {
        int diffCount = mDiffLst.size();
        for(int i = 0;i < diffCount;i++)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::NoBrush);
            ZDiffInfo diffInfo = mDiffLst[i];
            QColor color = STATUS_CLR[(int)diffInfo.status()];
            if(mTextWidget->isBlockContained(diffInfo))
            {
                QRectF rectf = mTextWidget->blockArea(diffInfo);
                QPoint startPoint = QPoint((int)rectf.x(), (int)rectf.y());
                startPoint = mTextWidget->mapToParent(startPoint);
                startPoint = this->mapFromParent(startPoint);
                if(diffInfo.isLine())
                {
                    painter.setPen(color);
                    painter.drawLine(startPoint, QPointF(startPoint.x() + rectf.width(), startPoint.y()));
                }
                else
                {
                    painter.setPen(color);
                    color.setAlpha(STATUS_CLR_ALPHA);
                    painter.setBrush(QBrush(color));
                    painter.drawRect(QRectF(startPoint, QSizeF(rectf.width(), rectf.height())));
                }
            }
        }
    }
}

void ZDiffAreaWidget::initData()
{

}

void ZDiffAreaWidget::initUI()
{

}

void ZDiffAreaWidget::initConnect()
{

}
