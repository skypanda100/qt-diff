#include "ztextwidget.h"
#include "zlinenumberwidget.h"

ZTextWidget::ZTextWidget(QWidget *parent)
    : QPlainTextEdit(parent)
{
    initData();
    initUI();
    initConnect();
}

ZTextWidget::~ZTextWidget()
{
    delete mLineNumberArea;
}

void ZTextWidget::lineNumberAreaPaintEvent(QPaintEvent */*event*/)
{
    QPainter painter(mLineNumberArea);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(LINE_NUMBER_AREA);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal bottom = top + blockBoundingRect(block).height();
    mFirstVisibleBlockNo = blockNumber;

    while(block.isValid() && top <= this->rect().bottom())
    {
        if(block.isVisible() && bottom >= this->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(10, top, mLineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignLeft, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
    mLastVisibleBlockNo = blockNumber - 1;
}

int ZTextWidget::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while(max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 20 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

bool ZTextWidget::isBlockContained(ZDiffInfo diffInfo)
{
    updateBlockNo();

    QList<int> blockNoLst = diffInfo.diffLst();
    int minBlockNo = blockNoLst[0];
    int maxBlockNo = blockNoLst[blockNoLst.size() - 1];
    if(minBlockNo <= mLastVisibleBlockNo && maxBlockNo >= mFirstVisibleBlockNo)
    {
        return true;
    }
    return false;
}

QRectF ZTextWidget::blockArea(ZDiffInfo diffInfo)
{
    updateBlockNo();

    QList<int> blockNoLst = diffInfo.diffLst();
    bool isLine = diffInfo.isLine();
    int minBlockNo = blockNoLst[0];
    int maxBlockNo = blockNoLst[blockNoLst.size() - 1];
    qreal y1 = 0;
    qreal y2 = 0;

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal bottom = top + blockBoundingRect(block).height();

    if(minBlockNo < mFirstVisibleBlockNo)
    {
        minBlockNo = mFirstVisibleBlockNo;
    }
    else if(minBlockNo > mLastVisibleBlockNo)
    {
        minBlockNo = mLastVisibleBlockNo;
    }

    if(maxBlockNo > mLastVisibleBlockNo)
    {
        maxBlockNo = mLastVisibleBlockNo;
    }
    else if(maxBlockNo < mFirstVisibleBlockNo)
    {
        maxBlockNo = mFirstVisibleBlockNo;
    }

    while(block.isValid())
    {
        if(minBlockNo == blockNumber)
        {
            y1 = blockBoundingGeometry(block).translated(contentOffset()).top();
            if(isLine)
            {
                y2 = y1 + 1;
                break;
            }
        }

        if(maxBlockNo == blockNumber)
        {
            y2 = blockBoundingGeometry(block).translated(contentOffset()).top() + blockBoundingRect(block).height();
            break;
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }

    QPoint point(this->viewport()->rect().x(), y1);

    return QRectF(point, QSize(this->width(), y2 - y1));
}

int ZTextWidget::visibleBlockCount()
{
    return mLastVisibleBlockNo - mFirstVisibleBlockNo;
}

void ZTextWidget::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    mLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void ZTextWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = qAbs(numDegrees / 15);
    int value = 0;
    if(numDegrees > 0)
    {
        value = this->verticalScrollBar()->value() - numSteps;
    }
    else
    {
        value = this->verticalScrollBar()->value() + numSteps;
    }
    value = value < 0 ? 0 : value;
    emit scrollValueChanged(value);

    event->accept();
}

void ZTextWidget::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void ZTextWidget::updateLineNumberArea(const QRect &rect, int dy)
{
    if(dy)
    {
        mLineNumberArea->scroll(0, dy);
    }
    else
    {
        mLineNumberArea->update(0, rect.y(), mLineNumberArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void ZTextWidget::initData()
{
    mFirstVisibleBlockNo = 0;
    mLastVisibleBlockNo = 0;
}

void ZTextWidget::initUI()
{
    mLineNumberArea = new ZLineNumberWidget(this);
    verticalScrollBar()->setTracking(true);

    updateLineNumberAreaWidth(0);
}

void ZTextWidget::initConnect()
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
}

void ZTextWidget::updateBlockNo()
{
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal bottom = top + blockBoundingRect(block).height();
    mFirstVisibleBlockNo = blockNumber;

    while(block.isValid() && top <= this->rect().bottom())
    {
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
    mLastVisibleBlockNo = blockNumber - 1;
}

