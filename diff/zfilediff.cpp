#include "zfilediff.h"
#include "util/zfile.h"
#include <QDebug>
#include <QtMath>

ZFileDiff::ZFileDiff()
{
    mDiffRect = NULL;
    mFileSrc = NULL;
    mFileDst = NULL;
}

ZFileDiff::ZFileDiff(const QString &fileSrc, const QString &fileDst)
{
    mDiffRect = NULL;
    mFileSrc = new QFile(fileSrc);
    mFileDst = new QFile(fileDst);
}

ZFileDiff::~ZFileDiff()
{
    if(mDiffRect != NULL)
    {
        delete[] mDiffRect;
        mDiffRect = NULL;
    }

    if(mFileSrc != NULL)
    {
        delete mFileSrc;
        mFileSrc = NULL;
    }

    if(mFileDst != NULL)
    {
        delete mFileDst;
        mFileDst = NULL;
    }
}

void ZFileDiff::setFileSrc(const QString &fileSrc)
{
    mFileSrc = new QFile(fileSrc);
}

void ZFileDiff::setFileDst(const QString &fileDst)
{
    mFileDst = new QFile(fileDst);
}

void ZFileDiff::execute()
{
    if(initRect())
    {
        if(makeRect())
        {
            recallRect();
            //test
            for(int i = mModelLst.size() - 2;i >= 0;i--)
            {
                ZFileDiffModel model = mModelLst[i];
                qDebug() << model.srcLine() << model.dstLine() << model.status();
            }
        }
    }
}

bool ZFileDiff::initRect()
{
    mDiffRectRows = ZFile::lines(mFileSrc, mLineSrcLst);
    mDiffRectCols = ZFile::lines(mFileDst, mLineDstLst);

    if(mDiffRectRows == -1 || mDiffRectCols == -1)
    {
        return false;
    }
    else
    {
        mDiffRectRows += 1;
        mDiffRectCols += 1;
    }

    mDiffRect = new short*[mDiffRectRows];
    for(int i = 0;i < mDiffRectRows;i++)
    {
        mDiffRect[i] = new short[mDiffRectCols];
        memset(mDiffRect[i], 0, mDiffRectCols);
    }

    for(int i = 1;i < mDiffRectRows;i++)
    {
        mDiffRect[i][0] = i;
    }

    for(int i = 0;i < mDiffRectCols;i++)
    {
        mDiffRect[0][i] = i;
    }

    return true;
}

bool ZFileDiff::makeRect()
{
    int lineSrcCount = mLineSrcLst.size();
    int lineDstCount = mLineDstLst.size();

    for(int i = 0;i < lineSrcCount;i++)
    {
        QString lineSrc = mLineSrcLst[i];
        for(int j = 0;j < lineDstCount;j++)
        {
            QString lineDst = mLineDstLst[j];

            short top = mDiffRect[i][j + 1];
            short topLeft = mDiffRect[i][j];
            short left = mDiffRect[i + 1][j];

            if(QString::compare(lineSrc, lineDst) == 0)
            {
                mDiffRect[i + 1][j + 1] = topLeft;
            }
            else
            {
                mDiffRect[i + 1][j + 1] = qMin(top, qMin(topLeft, left)) + 1;
            }
        }
    }

    return true;
}

void ZFileDiff::recallRect()
{
    int row = mDiffRectRows;
    int col = mDiffRectCols;

    int i = row - 1;
    int j = col - 1;

    while(i >= 0 && j >= 0)
    {
        QString lineSrc = NULL;
        QString lineDst = NULL;
        if(i - 1 >= 0)
        {
            lineSrc = mLineSrcLst[i - 1];
        }
        if(j - 1 >= 0)
        {
            lineDst = mLineDstLst[j - 1];
        }

        short top = 0;
        short topLeft = 0;
        short left = 0;

        short valArr[3] = {topLeft, top, left};
        int idxArr[3] = {0, 1, 2};

        if(i == 0 && j > 0)
        {
            valArr[0] = 1;
            valArr[1] = 1;
            valArr[2] = 0;
        }
        else if(i > 0 && j == 0)
        {
            valArr[0] = 1;
            valArr[1] = 0;
            valArr[2] = 1;
        }
        else if(i == 0 && j == 0)
        {
            valArr[0] = 0;
            valArr[1] = 0;
            valArr[2] = 0;
        }
        else
        {
            if(QString::compare(lineSrc, lineDst) == 0)
            {
                valArr[0] = 0;
                valArr[1] = 1;
                valArr[2] = 1;
            }
            else
            {
                valArr[0] = mDiffRect[i - 1][j - 1];
                valArr[1] = mDiffRect[i - 1][j];
                valArr[2] = mDiffRect[i][j - 1];
            }
        }

        for(int k = 0;k < (int)(sizeof(valArr) / sizeof(short) - 1);k++)
        {
            short valCur = valArr[k];
            short valNxt = valArr[k + 1];

            int idxCur = idxArr[k];
            int idxNxt = idxArr[k + 1];

            if(valCur <= valNxt)
            {
                short valTmp = valCur;
                valArr[k] = valNxt;
                valArr[k + 1] = valTmp;

                int idxTmp = idxCur;
                idxArr[k] = idxNxt;
                idxArr[k + 1] = idxTmp;
            }
        }

        ZFileDiffModel model;
        int idxLast = idxArr[sizeof(idxArr) / sizeof(int) - 1];
        switch(idxLast)
        {
            case 0:
                model.setSrcLine(lineSrc);
                model.setDstLine(lineDst);
                if(QString::compare(lineSrc, lineDst) == 0)
                {
                    model.setStatus(0);
                }
                else
                {
                    model.setStatus(1);
                }
                i -= 1;
                j -= 1;
                break;
            case 1:
                model.setSrcLine(lineSrc);
                model.setDstLine(NULL);
                model.setStatus(2);
                i -= 1;
                break;
            case 2:
                model.setSrcLine(NULL);
                model.setDstLine(lineDst);
                model.setStatus(3);
                j -= 1;
                break;
        }

        mModelLst.append(model);
    }
}

