#include "zfilewidget.h"
#include "util/zfile.h"
#include <QFile>

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
    delete mPathEditSrc;
    delete mSearchButtonSrc;
    delete mPathEditDst;
    delete mSearchButtonDst;
    delete mSrcScrollTextWidget;
    delete mDstScrollTextWidget;
}

void ZFileWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    int srcDiffCount = mSrcDiffLst.size();
    int dstDiffCount = mDstDiffLst.size();
    if(srcDiffCount != dstDiffCount)
    {
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for(int i = 0;i < srcDiffCount;i++)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        ZDiffInfo srcDiffInfo = mSrcDiffLst[i];
        ZDiffInfo dstDiffInfo = mDstDiffLst[i];
        QColor color = STATUS_CLR[(int)srcDiffInfo.status()];
        if(mSrcScrollTextWidget->isBlockContained(srcDiffInfo)
                || mDstScrollTextWidget->isBlockContained(dstDiffInfo))
        {
            QRectF srcRectf = mSrcScrollTextWidget->blockArea(srcDiffInfo);
            QRectF dstRectf = mDstScrollTextWidget->blockArea(dstDiffInfo);

            QPoint srcStartPoint = QPoint(srcRectf.x(), srcRectf.y());
            QPoint dstStartPoint = QPoint(dstRectf.x(), dstRectf.y());
            srcStartPoint = mSrcScrollTextWidget->mapToParent(srcStartPoint);
            dstStartPoint = mDstScrollTextWidget->mapToParent(dstStartPoint);
            if(srcDiffInfo.isLine())
            {
                srcStartPoint = QPoint(srcStartPoint.x() + srcRectf.width()
                                       , srcStartPoint.y());
            }
            else
            {
                srcStartPoint = QPoint(srcStartPoint.x() + srcRectf.width()
                                       , srcStartPoint.y() + srcRectf.height() / 2);
            }
            if(dstDiffInfo.isLine())
            {
                dstStartPoint = QPoint(dstStartPoint.x()
                                       , dstStartPoint.y());
            }
            else
            {
                dstStartPoint = QPoint(dstStartPoint.x()
                                       , dstStartPoint.y() + dstRectf.height() / 2);
            }
            painter.setPen(color);
            painter.drawLine(srcStartPoint, dstStartPoint);
        }
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(OVERVIEW_DIFF_AREA_CLR);

    int srcDiffAreaCount = mSrcDiffAreaLst.size();
    for(int i = 0;i < srcDiffAreaCount;i++)
    {
        ZDiffArea diffArea = mSrcDiffAreaLst[i];
        int startX = diffArea.startX();
        int startY = diffArea.startY() * (mSrcScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mSrcScrollTextWidget->y();
        int endX = diffArea.endX();
        int endY = diffArea.endY() * (mSrcScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mSrcScrollTextWidget->y();
        painter.drawRect(QRect(QPoint(startX, startY), QPoint(endX, endY)));
    }

    int dstDiffAreaCount = mDstDiffAreaLst.size();
    for(int i = 0;i < dstDiffAreaCount;i++)
    {
        ZDiffArea diffArea = mDstDiffAreaLst[i];
        int startX = diffArea.startX() + this->width();
        int startY = diffArea.startY() * (mDstScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mDstScrollTextWidget->y();
        int endX = diffArea.endX() + this->width();
        int endY = diffArea.endY() * (mDstScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mDstScrollTextWidget->y();
        painter.drawRect(QRect(QPoint(startX, startY), QPoint(endX, endY)));
    }
}

void ZFileWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos();
        int srcDiffAreaCount = mSrcDiffAreaLst.size();
        for(int i = 0;i < srcDiffAreaCount;i++)
        {
            ZDiffArea diffArea = mSrcDiffAreaLst[i];
            int startX = diffArea.startX();
            int startY = diffArea.startY() * (mSrcScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mSrcScrollTextWidget->y();
            int endX = diffArea.endX();
            int endY = diffArea.endY() * (mSrcScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mSrcScrollTextWidget->y();
            QRect rect(QPoint(startX, startY), QPoint(endX, endY));
            if(rect.contains(point))
            {
                int blockNo = (float)(point.y() - mSrcScrollTextWidget->y()) / (mSrcScrollTextWidget->height() - SCROLL_BAR_WIDTH)
                        * mSrcLineLst.size();
                mSrcScrollTextWidget->onScrollValueChange(blockNo);
                return;
            }
        }

        int dstDiffAreaCount = mDstDiffAreaLst.size();
        for(int i = 0;i < dstDiffAreaCount;i++)
        {
            ZDiffArea diffArea = mDstDiffAreaLst[i];
            int startX = diffArea.startX() + this->width();
            int startY = diffArea.startY() * (mDstScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mDstScrollTextWidget->y();
            int endX = diffArea.endX() + this->width();
            int endY = diffArea.endY() * (mDstScrollTextWidget->height() - SCROLL_BAR_WIDTH) + mDstScrollTextWidget->y();
            QRect rect(QPoint(startX, startY), QPoint(endX, endY));
            if(rect.contains(point))
            {
                int blockNo = (float)(point.y() - mDstScrollTextWidget->y()) / (mDstScrollTextWidget->height() - SCROLL_BAR_WIDTH)
                        * mDstLineLst.size();
                mDstScrollTextWidget->onScrollValueChange(blockNo);
                return;
            }
        }
    }
}

void ZFileWidget::initData()
{
    clearData();
    getLineFromFile();
    getDiffInfo();
    getDiffArea();
}

void ZFileWidget::initUI()
{
    this->setContentsMargins(OVERVIEW_DIFF_AREA_WIDTH, OVERVIEW_DIFF_AREA_WIDTH, OVERVIEW_DIFF_AREA_WIDTH, OVERVIEW_DIFF_AREA_WIDTH);

    mPathEditSrc = new QLineEdit;
    mPathEditSrc->setPlaceholderText("File");
    mSearchButtonSrc = new QPushButton;
    mSearchButtonSrc->setIcon(QIcon(":/icon/search.png"));
    mSearchButtonSrc->setToolTip("search");

    mPathEditDst = new QLineEdit;
    mPathEditDst->setPlaceholderText("File");
    mSearchButtonDst = new QPushButton;
    mSearchButtonDst->setIcon(QIcon(":/icon/search.png"));
    mSearchButtonDst->setToolTip("search");

    mSrcScrollTextWidget = new ZScrollTextWidget(Qt::AlignLeft);
    mDstScrollTextWidget = new ZScrollTextWidget(Qt::AlignRight);

    QHBoxLayout *srcHLayout = new QHBoxLayout;
    srcHLayout->addWidget(mPathEditSrc);
    srcHLayout->addWidget(mSearchButtonSrc);

    QHBoxLayout *dstHLayout = new QHBoxLayout;
    dstHLayout->addWidget(mPathEditDst);
    dstHLayout->addWidget(mSearchButtonDst);

    QVBoxLayout *srcVLayout = new QVBoxLayout;
    srcVLayout->setSpacing(5);
    srcVLayout->addLayout(srcHLayout);
    srcVLayout->addWidget(mSrcScrollTextWidget);

    QVBoxLayout *dstVLayout = new QVBoxLayout;
    dstVLayout->setSpacing(5);
    dstVLayout->addLayout(dstHLayout);
    dstVLayout->addWidget(mDstScrollTextWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(srcVLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(dstVLayout);

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

    setFilePath();
}

void ZFileWidget::initConnect()
{
    connect(mSearchButtonSrc, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
    connect(mSearchButtonDst, SIGNAL(clicked()), this, SLOT(onSearchClicked()));
    connect(mSrcScrollTextWidget, SIGNAL(scrollValueChange(int)), this, SLOT(onScrollValueChanged(int)));
    connect(mDstScrollTextWidget, SIGNAL(scrollValueChange(int)), this, SLOT(onScrollValueChanged(int)));
}

void ZFileWidget::clearData()
{
    mModelLst.clear();
    mSrcLineLst.clear();
    mDstLineLst.clear();
    mSrcDiffLst.clear();
    mDstDiffLst.clear();
    mSrcDiffAreaLst.clear();
    mDstDiffAreaLst.clear();
}

void ZFileWidget::setFilePath()
{
    Status status = mPathDiffModel.status();
    if(status == Same)
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        mPathEditSrc->setText(srcPath);
        mPathEditDst->setText(dstPath);
    }
    else if(status == Removed)
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        mPathEditSrc->setText(srcPath);
    }
    else if(status == Added)
    {
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        mPathEditDst->setText(dstPath);
    }
    else
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();
        mPathEditSrc->setText(srcPath);
        mPathEditDst->setText(dstPath);
    }
}

void ZFileWidget::getLineFromFile()
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

        ZFile::linesWithLine(&srcFile, mSrcLineLst);
        ZFile::linesWithLine(&dstFile, mDstLineLst);
    }
}

void ZFileWidget::getDiffInfo()
{
    Status status = mPathDiffModel.status();
    if(status == Same)
    {
    }
    else if(status == Removed)
    {
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
        srcDiffInfo.setStatus(Removed);
        dstDiffInfo.setDiffLst(dstDiffLst);
        dstDiffInfo.setLine(true);
        dstDiffInfo.setStatus(Removed);

        mSrcDiffLst.append(srcDiffInfo);
        mDstDiffLst.append(dstDiffInfo);
    }
    else if(status == Added)
    {
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
        srcDiffInfo.setStatus(Added);
        dstDiffInfo.setDiffLst(dstDiffLst);
        dstDiffInfo.setLine(false);
        dstDiffInfo.setStatus(Added);

        mSrcDiffLst.append(srcDiffInfo);
        mDstDiffLst.append(dstDiffInfo);
    }
    else
    {
        QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
        QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();

        ZFileDiff fileDiff(srcPath, dstPath);
        mModelLst = fileDiff.execute();

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

void ZFileWidget::getDiffArea()
{
    int srcLineCount = mSrcLineLst.size();
    int srcDiffCount = mSrcDiffLst.size();
    for(int i = 0;i < srcDiffCount;i++)
    {
        ZDiffInfo diffInfo = mSrcDiffLst[i];
        QList<int> diffLst = diffInfo.diffLst();
        int diffCount = diffLst.size();
        int startLineNo = 0;
        int endLineNo = 0;
        if(diffCount > 0)
        {
            startLineNo = diffLst[0] + 1;
            endLineNo = diffLst[diffCount - 1] + 1;
        }
        ZDiffArea diffArea;
        diffArea.setStartX(OVERVIEW_DIFF_AREA_MARGIN);
        diffArea.setStartY((float)startLineNo / srcLineCount);
        diffArea.setEndX(OVERVIEW_DIFF_AREA_WIDTH - OVERVIEW_DIFF_AREA_MARGIN);
        diffArea.setEndY((float)endLineNo / srcLineCount);
        mSrcDiffAreaLst.append(diffArea);
    }

    int dstLineCount = mDstLineLst.size();
    int dstDiffCount = mDstDiffLst.size();
    for(int i = 0;i < dstDiffCount;i++)
    {
        ZDiffInfo diffInfo = mDstDiffLst[i];
        QList<int> diffLst = diffInfo.diffLst();
        int diffCount = diffLst.size();
        int startLineNo = 0;
        int endLineNo = 0;
        if(diffCount > 0)
        {
            startLineNo = diffLst[0] + 1;
            endLineNo = diffLst[diffCount - 1] + 1;
        }
        ZDiffArea diffArea;
        diffArea.setStartX(-1 * (OVERVIEW_DIFF_AREA_WIDTH - OVERVIEW_DIFF_AREA_MARGIN));
        diffArea.setStartY((float)startLineNo / dstLineCount);
        diffArea.setEndX(-OVERVIEW_DIFF_AREA_MARGIN);
        diffArea.setEndY((float)endLineNo / dstLineCount);
        mDstDiffAreaLst.append(diffArea);
    }
}

void ZFileWidget::onScrollValueChanged(int value)
{
    QObject *sender = this->sender();
    Status status = mPathDiffModel.status();
    int srcLineCount = mSrcLineLst.size();
    int dstLineCount = mDstLineLst.size();
    int lineCount = srcLineCount;
    int oLineCount = dstLineCount;
    ZScrollTextWidget *scrollTextWidget = mDstScrollTextWidget;

    if(sender == mDstScrollTextWidget)
    {
        lineCount = dstLineCount;
        oLineCount = srcLineCount;
        scrollTextWidget = mSrcScrollTextWidget;
    }

    if(status == Modified)
    {
        qreal ratio = (qreal)value / lineCount;
        int value = (int)(oLineCount * ratio);
        scrollTextWidget->onScrollValueChange(value);
    }
    else if(status == Same)
    {
        scrollTextWidget->onScrollValueChange(value);

    }
    else
    {

    }
}

void ZFileWidget::onSearchClicked()
{
    QObject *object = this->sender();
    if(object == mSearchButtonSrc)
    {
        QString currentFile = mPathEditSrc->text().isEmpty() ? QDir::currentPath() : mPathEditSrc->text();
        QString file = QFileDialog::getOpenFileName(this,
                                   tr("search"), currentFile);
        if(!file.isEmpty())
        {
            mPathEditSrc->setText(file);
        }
    }
    else
    {
        QString currentFile = mPathEditDst->text().isEmpty() ? QDir::currentPath() : mPathEditDst->text();
        QString file = QFileDialog::getOpenFileName(this,
                                   tr("search"), currentFile);
        if(!file.isEmpty())
        {
            mPathEditDst->setText(file);
        }
    }
}
