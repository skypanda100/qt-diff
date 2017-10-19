#include "zfilewidget.h"
#include "util/zfile.h"
#include <QFile>

ZLineNumberWidget::ZLineNumberWidget(ZTextWidget *parent)
    : QWidget(parent)
    , mTextWidget(parent)
{
    initData();
    initUI();
    initConnect();
}

ZLineNumberWidget::~ZLineNumberWidget()
{

}

QSize ZLineNumberWidget::sizeHint() const
{
    return QSize(mTextWidget->lineNumberAreaWidth(), 0);
}

void ZLineNumberWidget::paintEvent(QPaintEvent *event)
{
    mTextWidget->lineNumberAreaPaintEvent(event);
}

void ZLineNumberWidget::initData()
{

}

void ZLineNumberWidget::initUI()
{

}

void ZLineNumberWidget::initConnect()
{

}

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

void ZTextWidget::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(mLineNumberArea);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    mFirstVisibleBlockNo = blockNumber;
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    mBlockHeight = (int) blockBoundingRect(block).height();

    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(10, top, mLineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignLeft, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
    mLastVisibleBlockNo = blockNumber;
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

bool ZTextWidget::isBlockContained(QList<int> blockNoLst)
{
    int minBlockNo = blockNoLst[0];
    int maxBlockNo = blockNoLst[blockNoLst.size() - 1];
    if(minBlockNo >= mLastVisibleBlockNo && maxBlockNo <= mFirstVisibleBlockNo)
    {
        return true;
    }
    return false;
}

QRect ZTextWidget::blockArea(QList<int> blockNoLst)
{
    int minBlockNo = blockNoLst[0];
    int maxBlockNo = blockNoLst[blockNoLst.size() - 1];
    int y = 0;
    int height = 0;

    if(minBlockNo < mFirstVisibleBlockNo)
    {
        y = this->rect().top();
    }
    else
    {
        y = (minBlockNo - mFirstVisibleBlockNo) * mBlockHeight + this->rect().top();
    }

    if(maxBlockNo > mLastVisibleBlockNo)
    {
        height = this->rect().bottom() - y;
    }
    else
    {
        height = this->rect().bottom() - (mLastVisibleBlockNo - maxBlockNo) * mBlockHeight;
    }

    return QRect(this->rect().x(), y, this->width(), height);
}

void ZTextWidget::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    mLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
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

ZScrollTextWidget::ZScrollTextWidget(Qt::Alignment verticalAlignment, QWidget *parent)
    : QWidget(parent)
    , mVerticalAlignment(verticalAlignment)
{
    initData();
    initUI();
    initConnect();
}

ZScrollTextWidget::~ZScrollTextWidget()
{
    delete mTextWidget;
    delete mVerticalBar;
    delete mHorizontalBar;
}

void ZScrollTextWidget::appendText(const QString &text)
{
    mTextWidget->appendPlainText(text);
}

void ZScrollTextWidget::setVerticalValue(int value)
{
}

void ZScrollTextWidget::setHorizontalValue(int value)
{
}

bool ZScrollTextWidget::isBlockContained(QList<int> blockNoLst)
{
    return mTextWidget->isBlockContained(blockNoLst);
}

QRect ZScrollTextWidget::blockArea(QList<int> blockNoLst)
{
    return mTextWidget->blockArea(blockNoLst);
}

void ZScrollTextWidget::initData()
{

}

void ZScrollTextWidget::initUI()
{
    mTextWidget = new ZTextWidget;
    mTextWidget->setLineWrapMode(QPlainTextEdit::NoWrap);
    mTextWidget->verticalScrollBar()->setTracking(true);
    mTextWidget->horizontalScrollBar()->setTracking(true);
    mTextWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mTextWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mVerticalBar = new QScrollBar(Qt::Vertical, this);
    mVerticalBar->setFixedWidth(20);
    mVerticalBar->setVisible(false);

    mHorizontalBar = new QScrollBar(Qt::Horizontal, this);
    mHorizontalBar->setFixedHeight(20);
    mHorizontalBar->setVisible(false);

    QGridLayout *mainLayout =new QGridLayout;
    mainLayout->setSpacing(0);
    if(mVerticalAlignment == Qt::AlignLeft)
    {
        mainLayout->addWidget(mVerticalBar, 0, 0);
        mainLayout->addWidget(mTextWidget, 0, 1);
        mainLayout->addWidget(mHorizontalBar, 1, 1);
    }
    else
    {
        mainLayout->addWidget(mTextWidget, 0, 0);
        mainLayout->addWidget(mVerticalBar, 0, 1);
        mainLayout->addWidget(mHorizontalBar, 1, 0);
    }
    this->setLayout(mainLayout);
}

void ZScrollTextWidget::initConnect()
{
    connect(mVerticalBar, SIGNAL(valueChanged(int)), mTextWidget->verticalScrollBar(), SLOT(setValue(int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setVerticalRange(int,int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(sliderMoved(int)), mVerticalBar, SLOT(setValue(int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), mVerticalBar, SLOT(setValue(int)));
    connect(mHorizontalBar, SIGNAL(valueChanged(int)), mTextWidget->horizontalScrollBar(), SLOT(setValue(int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setHorizontalRange(int,int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(sliderMoved(int)), mHorizontalBar, SLOT(setValue(int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(valueChanged(int)), mHorizontalBar, SLOT(setValue(int)));

}

void ZScrollTextWidget::setVerticalRange(int min, int max)
{
    if(!mVerticalBar->isVisible())
    {
        mVerticalBar->setVisible(true);
    }
    mVerticalBar->setValue(max);
    mVerticalBar->setRange(min, max);
}

void ZScrollTextWidget::setHorizontalRange(int min, int max)
{
    if(!mHorizontalBar->isVisible())
    {
        mHorizontalBar->setVisible(true);
    }
    mHorizontalBar->setValue(max);
    mHorizontalBar->setRange(min, max);
}

ZFileWidget::ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent)
    : QWidget(parent)
    , mPathDiffModel(pathDiffModel)
{
    initData();
    initUI();
    initConnect();
}

ZFileWidget::~ZFileWidget()
{
    delete mSrcScrollTextWidget;
    delete mDstScrollTextWidget;
}

void ZFileWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter;

    int modelCount = mModelLst.size();
    int srcIndex = 0;
    int dstIndex = 0;
    QList<int> srcBlockNoLst;
    QList<int> dstBlockNoLst;
    bool isModifiedBlockStart = false;
    bool isRemovedBlockStart = false;
    bool isAddedBlockStart = false;

    for(int i = 0;i < modelCount;i++)
    {
        ZFileDiffModel model = mModelLst[i];
        Status status = model.status();
        if(status == Modified)
        {
            if(isRemovedBlockStart)
            {
                isRemovedBlockStart = !isRemovedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "1" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "2" << rect;

                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(isAddedBlockStart)
            {
                isAddedBlockStart = !isAddedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "3" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "4" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(!isModifiedBlockStart)
            {
                isModifiedBlockStart = !isModifiedBlockStart;
            }
            srcBlockNoLst.append(srcIndex);
            dstBlockNoLst.append(dstIndex);
            srcIndex++;
            dstIndex++;
        }
        else if(status == Removed)
        {
            if(isModifiedBlockStart)
            {
                isModifiedBlockStart = !isModifiedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "5" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "6" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(isAddedBlockStart)
            {
                isAddedBlockStart = !isAddedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "7" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "8" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(!isRemovedBlockStart)
            {
                isRemovedBlockStart = !isRemovedBlockStart;
            }
            srcBlockNoLst.append(srcIndex);
            dstBlockNoLst.append(dstIndex);
            srcIndex++;
        }
        else if(status == Added)
        {
            if(isModifiedBlockStart)
            {
                isModifiedBlockStart = !isModifiedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "9" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "10" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(isRemovedBlockStart)
            {
                isRemovedBlockStart = !isRemovedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "11" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "12" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(!isAddedBlockStart)
            {
                isAddedBlockStart = !isAddedBlockStart;
            }
            srcBlockNoLst.append(srcIndex);
            dstBlockNoLst.append(dstIndex);
            dstIndex++;
        }
        else
        {
            if(isModifiedBlockStart)
            {
                isModifiedBlockStart = !isModifiedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "13" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "14" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(isRemovedBlockStart)
            {
                isRemovedBlockStart = !isRemovedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "15" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "6" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }
            if(isAddedBlockStart)
            {
                isAddedBlockStart = !isAddedBlockStart;
                if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
                {
                    QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "17" << rect;
                }
                if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
                {
                    QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
                    painter.fillRect(rect, QBrush(DIFF_CLR));
                    qDebug() << "18" << rect;
                }
                srcBlockNoLst.clear();
                dstBlockNoLst.clear();
            }

            srcIndex++;
            dstIndex++;
        }
    }
    if(isModifiedBlockStart)
    {
        isModifiedBlockStart = !isModifiedBlockStart;
        if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
        {
            QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
            painter.fillRect(rect, QBrush(DIFF_CLR));
            qDebug() << "19" << rect;
        }
        if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
        {
            QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
            painter.fillRect(rect, QBrush(DIFF_CLR));
            qDebug() << "20" << rect;
        }
        srcBlockNoLst.clear();
        dstBlockNoLst.clear();
    }
    if(isRemovedBlockStart)
    {
        isRemovedBlockStart = !isRemovedBlockStart;
        if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
        {
            QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
            painter.fillRect(rect, QBrush(DIFF_CLR));
            qDebug() << "21" << rect;
        }
        if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
        {
            QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
            painter.fillRect(rect, QBrush(DIFF_CLR));
            qDebug() << "22" << rect;
        }
        srcBlockNoLst.clear();
        dstBlockNoLst.clear();
    }
    if(isAddedBlockStart)
    {
        isAddedBlockStart = !isAddedBlockStart;
        if(mSrcScrollTextWidget->isBlockContained(srcBlockNoLst))
        {
            QRect rect = mSrcScrollTextWidget->blockArea(srcBlockNoLst);
            painter.fillRect(rect, QBrush(DIFF_CLR));
            qDebug() << "23" << rect;
        }
        if(mDstScrollTextWidget->isBlockContained(dstBlockNoLst))
        {
            QRect rect = mDstScrollTextWidget->blockArea(dstBlockNoLst);
            painter.fillRect(rect, QBrush(DIFF_CLR));
            qDebug() << "24" << rect;
        }
        srcBlockNoLst.clear();
        dstBlockNoLst.clear();
    }
}

void ZFileWidget::initData()
{
    Status status = mPathDiffModel.status();
    if(status == Same)
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        QFile srcFile(srcPath);
        QFile dstFile(dstPath);

        ZFile::linesWithLine(&srcFile, mSrcLineLst);
        ZFile::linesWithLine(&dstFile, mDstLineLst);
    }
    else if(status == Removed)
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        QFile srcFile(srcPath);

        ZFile::linesWithLine(&srcFile, mSrcLineLst);
    }
    else if(status == Added)
    {
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        QFile dstFile(dstPath);

        ZFile::linesWithLine(&dstFile, mDstLineLst);
    }
    else
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        QFile srcFile(srcPath);
        QFile dstFile(dstPath);

        ZFileDiff fileDiff(srcPath, dstPath);
        mModelLst = fileDiff.execute();
        ZFile::linesWithLine(&srcFile, mSrcLineLst);
        ZFile::linesWithLine(&dstFile, mDstLineLst);
    }
}

void ZFileWidget::initUI()
{
    mSrcScrollTextWidget = new ZScrollTextWidget(Qt::AlignLeft);
    mDstScrollTextWidget = new ZScrollTextWidget(Qt::AlignRight);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mSrcScrollTextWidget);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(mDstScrollTextWidget);

    this->setLayout(mainLayout);

    int srcLineCount = mSrcLineLst.size();
    for(int i = 0;i < srcLineCount;i++)
    {
        mSrcScrollTextWidget->appendText(mSrcLineLst[i]);
    }

    int dstLineCount = mDstLineLst.size();
    for(int i = 0;i < dstLineCount;i++)
    {
        mDstScrollTextWidget->appendText(mDstLineLst[i]);
    }
}

void ZFileWidget::initConnect()
{

}
