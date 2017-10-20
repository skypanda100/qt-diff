#include "zfilewidget.h"
#include "util/zfile.h"
#include <QFile>

ZDiffInfo::ZDiffInfo()
{

}

ZDiffInfo::~ZDiffInfo()
{

}

void ZDiffInfo::setDiffLst(QList<int> diffLst)
{
    mDiffLst = diffLst;
}

QList<int> ZDiffInfo::diffLst() const
{
    return mDiffLst;
}

void ZDiffInfo::setLine(bool isLine)
{
    mIsLine = isLine;
}

bool ZDiffInfo::isLine() const
{
    return mIsLine;
}

void ZDiffInfo::setStatus(Status status)
{
    mStatus = status;
}

Status ZDiffInfo::status() const
{
    return mStatus;
}

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
}

void ZDiffAreaWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    if(mTextWidget != NULL)
    {
        int diffCount = mDiffLst.size();
        for(int i = 0;i < diffCount;i++)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(Qt::NoBrush);
            ZDiffInfo diffInfo = mDiffLst[i];
            QColor color = STATUS_CLR[(int)diffInfo.status()];
            color.setAlpha(50);
            if(mTextWidget->isBlockContained(diffInfo))
            {
                QRectF rectf = mTextWidget->blockArea(diffInfo);
                QPoint startPoint = QPoint((int)rectf.x(), (int)rectf.y());
                startPoint = mTextWidget->mapToParent(startPoint);
                if(diffInfo.isLine())
                {
                    painter.setPen(color);
                    painter.drawLine(startPoint, QPointF(startPoint.x() + rectf.width(), startPoint.y()));
                }
                else
                {
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
    qreal top = blockBoundingGeometry(block).translated(contentOffset()).top();
    qreal bottom = top + blockBoundingRect(block).height();
    mFirstVisibleBlockNo = blockNumber;

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

    if(maxBlockNo > mLastVisibleBlockNo)
    {
        maxBlockNo = mLastVisibleBlockNo;
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

    QPoint point(this->viewport()->rect().x() + mLineNumberArea->width(), y1);

    return QRectF(point, QSize(this->width() - mLineNumberArea->width(), y2 - y1));
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
    delete mBelowWidget;
    delete mAboveWidget;
}

void ZScrollTextWidget::appendText(const QString &text)
{
    mTextWidget->appendPlainText(text);
}

void ZScrollTextWidget::setVerticalValue(int /*value*/)
{
}

void ZScrollTextWidget::setHorizontalValue(int /*value*/)
{
}

void ZScrollTextWidget::setDiffList(QList<ZDiffInfo> diffLst)
{
    mAboveWidget->setDiffList(diffLst);
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

    QGridLayout *gridLayout =new QGridLayout;
    gridLayout->setSpacing(0);
    if(mVerticalAlignment == Qt::AlignLeft)
    {
        gridLayout->addWidget(mVerticalBar, 0, 0);
        gridLayout->addWidget(mTextWidget, 0, 1);
        gridLayout->addWidget(mHorizontalBar, 1, 1);
    }
    else
    {
        gridLayout->addWidget(mTextWidget, 0, 0);
        gridLayout->addWidget(mVerticalBar, 0, 1);
        gridLayout->addWidget(mHorizontalBar, 1, 0);
    }
    mBelowWidget = new QWidget;
    mBelowWidget->setLayout(gridLayout);

    mAboveWidget = new ZDiffAreaWidget(mTextWidget);
    mAboveWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->addWidget(mBelowWidget);
    stackedLayout->addWidget(mAboveWidget);
    this->setLayout(stackedLayout);
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

        ZDiffInfo srcDiffInfo;
        ZDiffInfo dstDiffInfo;
        srcDiffInfo.setDiffLst(srcDiffLst);
        srcDiffInfo.setLine(false);
        dstDiffInfo.setDiffLst(dstDiffLst);
        dstDiffInfo.setLine(true);

        mSrcDiffLst.append(srcDiffInfo);
        mDstDiffLst.append(dstDiffInfo);
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

        ZDiffInfo srcDiffInfo;
        ZDiffInfo dstDiffInfo;
        srcDiffInfo.setDiffLst(srcDiffLst);
        srcDiffInfo.setLine(true);
        dstDiffInfo.setDiffLst(dstDiffLst);
        dstDiffInfo.setLine(false);

        mSrcDiffLst.append(srcDiffInfo);
        mDstDiffLst.append(dstDiffInfo);
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

        for(int i = modelCount - 2;i >= 0;i--)
        {
            ZFileDiffModel model = mModelLst[i];
            Status status = model.status();
            if(status == Modified)
            {
                if(isRemovedBlockStart)
                {
                    isRemovedBlockStart = !isRemovedBlockStart;
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(false);
                    srcDiffInfo.setStatus(Removed);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(true);
                    dstDiffInfo.setStatus(Removed);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);

                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(true);
                    srcDiffInfo.setStatus(Added);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(false);
                    dstDiffInfo.setStatus(Added);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
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
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(false);
                    srcDiffInfo.setStatus(Modified);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(false);
                    dstDiffInfo.setStatus(Modified);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);

                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(true);
                    srcDiffInfo.setStatus(Added);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(false);
                    dstDiffInfo.setStatus(Added);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
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
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(false);
                    srcDiffInfo.setStatus(Modified);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(false);
                    dstDiffInfo.setStatus(Modified);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isRemovedBlockStart)
                {
                    isRemovedBlockStart = !isRemovedBlockStart;
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(false);
                    srcDiffInfo.setStatus(Removed);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(true);
                    dstDiffInfo.setStatus(Removed);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
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
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(false);
                    srcDiffInfo.setStatus(Modified);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(false);
                    dstDiffInfo.setStatus(Modified);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isRemovedBlockStart)
                {
                    isRemovedBlockStart = !isRemovedBlockStart;
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(false);
                    srcDiffInfo.setStatus(Removed);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(true);
                    dstDiffInfo.setStatus(Removed);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }
                if(isAddedBlockStart)
                {
                    isAddedBlockStart = !isAddedBlockStart;
                    ZDiffInfo srcDiffInfo;
                    ZDiffInfo dstDiffInfo;
                    srcDiffInfo.setDiffLst(srcDiffLst);
                    srcDiffInfo.setLine(true);
                    srcDiffInfo.setStatus(Added);
                    dstDiffInfo.setDiffLst(dstDiffLst);
                    dstDiffInfo.setLine(false);
                    dstDiffInfo.setStatus(Added);

                    mSrcDiffLst.append(srcDiffInfo);
                    mDstDiffLst.append(dstDiffInfo);
                    srcDiffLst.clear();
                    dstDiffLst.clear();
                }

                srcIndex++;
                dstIndex++;
            }
        }
        if(isModifiedBlockStart)
        {
            srcIndex -= 1;
            dstIndex -= 1;
            isModifiedBlockStart = !isModifiedBlockStart;
            ZDiffInfo srcDiffInfo;
            ZDiffInfo dstDiffInfo;
            srcDiffInfo.setDiffLst(srcDiffLst);
            srcDiffInfo.setLine(false);
            srcDiffInfo.setStatus(Modified);
            dstDiffInfo.setDiffLst(dstDiffLst);
            dstDiffInfo.setLine(false);
            dstDiffInfo.setStatus(Modified);

            mSrcDiffLst.append(srcDiffInfo);
            mDstDiffLst.append(dstDiffInfo);
            srcDiffLst.clear();
            dstDiffLst.clear();
        }
        if(isRemovedBlockStart)
        {
            srcIndex -= 1;
            isRemovedBlockStart = !isRemovedBlockStart;
            ZDiffInfo srcDiffInfo;
            ZDiffInfo dstDiffInfo;
            srcDiffInfo.setDiffLst(srcDiffLst);
            srcDiffInfo.setLine(false);
            srcDiffInfo.setStatus(Removed);
            dstDiffInfo.setDiffLst(dstDiffLst);
            dstDiffInfo.setLine(true);
            dstDiffInfo.setStatus(Removed);

            mSrcDiffLst.append(srcDiffInfo);
            mDstDiffLst.append(dstDiffInfo);
            srcDiffLst.clear();
            dstDiffLst.clear();
        }
        if(isAddedBlockStart)
        {
            dstIndex -= 1;
            isAddedBlockStart = !isAddedBlockStart;
            ZDiffInfo srcDiffInfo;
            ZDiffInfo dstDiffInfo;
            srcDiffInfo.setDiffLst(srcDiffLst);
            srcDiffInfo.setLine(true);
            srcDiffInfo.setStatus(Added);
            dstDiffInfo.setDiffLst(dstDiffLst);
            dstDiffInfo.setLine(false);
            dstDiffInfo.setStatus(Added);

            mSrcDiffLst.append(srcDiffInfo);
            mDstDiffLst.append(dstDiffInfo);
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
