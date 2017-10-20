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
    if(minBlockNo <= mLastVisibleBlockNo && maxBlockNo >= mFirstVisibleBlockNo)
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

void ZTextWidget::setDiffList(QList<QList<int> > diffLst)
{
    mDiffLst = diffLst;
}

void ZTextWidget::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    mLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void ZTextWidget::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);
    QPainter painter_p(this->viewport());
    painter_p.setPen(Qt::NoPen);
    painter_p.setBrush(QBrush(DIFF_CLR));
    int diffCount = mDiffLst.size();
    for(int i = 0;i < diffCount;i++)
    {
        QList<int> diffLst = mDiffLst[i];
        if(isBlockContained(diffLst))
        {
            QRect rect = blockArea(diffLst);
            painter_p.drawRect(rect);
        }
    }
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

void ZScrollTextWidget::setDiffList(QList<QList<int> > diffLst)
{
    mTextWidget->setDiffList(diffLst);
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

        QList<int> srcDiffLst;
        QList<int> dstDiffLst;

        int srcLineCount = mSrcLineLst.size();
        for(int i = 0;i < srcLineCount;i++)
        {
            srcDiffLst.append(i);
        }
        dstDiffLst.append(0);

        mSrcDiffLst.append(srcDiffLst);
        mDstDiffLst.append(dstDiffLst);
    }
    else if(status == Added)
    {
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        QFile dstFile(dstPath);

        ZFile::linesWithLine(&dstFile, mDstLineLst);

        QList<int> srcDiffLst;
        QList<int> dstDiffLst;

        srcDiffLst.append(0);
        int dstLineCount = mDstLineLst.size();
        for(int i = 0;i < dstLineCount;i++)
        {
            dstDiffLst.append(i);
        }

        mSrcDiffLst.append(srcDiffLst);
        mDstDiffLst.append(dstDiffLst);
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


        int modelCount = mModelLst.size();
        int srcIndex = 0;
        int dstIndex = 0;
        QList<int> srcDiffLst;
        QList<int> dstDiffLst;
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
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(!isModifiedBlockStart)
                {
                    isModifiedBlockStart = !isModifiedBlockStart;
                }
                srcDiffLst.append(srcIndex);
                dstDiffLst.append(dstIndex);
                srcIndex++;
                dstIndex++;
            }
            else if(status == Removed)
            {
                if(isModifiedBlockStart)
                {
                    isModifiedBlockStart = !isModifiedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(!isRemovedBlockStart)
                {
                    isRemovedBlockStart = !isRemovedBlockStart;
                }
                srcDiffLst.append(srcIndex);
                dstDiffLst.append(dstIndex);
                srcIndex++;
            }
            else if(status == Added)
            {
                if(isModifiedBlockStart)
                {
                    isModifiedBlockStart = !isModifiedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isRemovedBlockStart)
                {
                    isRemovedBlockStart = !isRemovedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(!isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                }
                srcDiffLst.append(srcIndex);
                dstDiffLst.append(dstIndex);
                dstIndex++;
            }
            else
            {
                if(isModifiedBlockStart)
                {
                    isModifiedBlockStart = !isModifiedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isRemovedBlockStart)
                {
                    isRemovedBlockStart = !isRemovedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                    mSrcDiffLst.append(srcDiffLst);
                    mDstDiffLst.append(dstDiffLst);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }

                srcIndex++;
                dstIndex++;
            }
        }
        if(isModifiedBlockStart)
        {
            isModifiedBlockStart = !isModifiedBlockStart;
            mSrcDiffLst.append(srcDiffLst);
            mDstDiffLst.append(dstDiffLst);
            srcDiffLst.clear();
            dstDiffLst.clear();
        }
        if(isRemovedBlockStart)
        {
            isRemovedBlockStart = !isRemovedBlockStart;
            mSrcDiffLst.append(srcDiffLst);
            mDstDiffLst.append(dstDiffLst);
            srcDiffLst.clear();
            dstDiffLst.clear();
        }
        if(isAddedBlockStart)
        {
            isAddedBlockStart = !isAddedBlockStart;
            mSrcDiffLst.append(srcDiffLst);
            mDstDiffLst.append(dstDiffLst);
            srcDiffLst.clear();
            dstDiffLst.clear();
        }
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

    mSrcScrollTextWidget->setDiffList(mSrcDiffLst);
    mDstScrollTextWidget->setDiffList(mDstDiffLst);

}

void ZFileWidget::initConnect()
{

}
