#include "zfilediff.h"
#include "util/zfile.h"
#include <QDebug>
#include <QtMath>
#include "env/zcons.h"

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
        for(int i = 0;i < mDiffRectRows;i++)
        {
            delete mDiffRect[i];
            mDiffRect[i] = NULL;
        }
        delete mDiffRect;
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

QList<ZFileDiffModel> ZFileDiff::execute()
{
    if(hasDifference())
    {
        if(initRect())
        {
            if(makeRect())
            {
                recallRect();
            }
        }
    }
    return mModelLst;
}

bool ZFileDiff::hasDifference()
{
    mDiffRectRows = ZFile::linesWithHash(mFileSrc, mHashSrcLst);
    mDiffRectCols = ZFile::linesWithHash(mFileDst, mHashDstLst);

    if(mDiffRectRows == -1 || mDiffRectCols == -1)
    {
        return true;
    }
    else if(mDiffRectRows != mDiffRectCols)
    {
        return true;
    }
    else
    {
        int lineCount = mDiffRectRows;
        int i = 0;
        for(;i < lineCount;i++)
        {
            unsigned int srcHash = mHashSrcLst[i];
            unsigned int dstHash = mHashDstLst[i];
            if(srcHash != dstHash)
            {
                break;
            }
            else
            {
                ZFileDiffModel model;
                model.setSrcHash(srcHash);
                model.setDstHash(dstHash);
                model.setStatus(Same);

                mModelLst.append(model);
            }
        }
        if(i != lineCount)
        {
            mModelLst.clear();
            return true;
        }
    }
    return false;
}

bool ZFileDiff::initRect()
{
    if(mDiffRectRows == -1 || mDiffRectCols == -1)
    {
        return false;
    }
    else
    {
        mDiffRectRows += 1;
        mDiffRectCols += 1;
    }

    mDiffRect = new int*[mDiffRectRows];
    for(int i = 0;i < mDiffRectRows;i++)
    {
        mDiffRect[i] = new int[mDiffRectCols];
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
    int hashSrcCount = mHashSrcLst.size();
    int hashDstCount = mHashDstLst.size();

    for(int i = 0;i < hashSrcCount;i++)
    {
        unsigned int hashSrc = mHashSrcLst[i];
        for(int j = 0;j < hashDstCount;j++)
        {
            unsigned int hashDst = mHashDstLst[j];

            int top = mDiffRect[i][j + 1];
            int topLeft = mDiffRect[i][j];
            int left = mDiffRect[i + 1][j];

            if(hashSrc == hashDst)
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
        unsigned int hashSrc = 0;
        unsigned int hashDst = 0;
        if(i - 1 >= 0)
        {
            hashSrc = mHashSrcLst[i - 1];
        }
        if(j - 1 >= 0)
        {
            hashDst = mHashDstLst[j - 1];
        }

        int top = 0;
        int topLeft = 0;
        int left = 0;

        int valArr[3] = {topLeft, top, left};
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
            if(hashSrc == hashDst)
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

        for(int k = 0;k < (int)(sizeof(valArr) / sizeof(int) - 1);k++)
        {
            int valCur = valArr[k];
            int valNxt = valArr[k + 1];

            int idxCur = idxArr[k];
            int idxNxt = idxArr[k + 1];

            if(valCur <= valNxt)
            {
                int valTmp = valCur;
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
                model.setSrcHash(hashSrc);
                model.setDstHash(hashDst);
                if(hashSrc == hashDst)
                {
                    model.setStatus(Same);
                }
                else
                {
                    model.setStatus(Modified);
                }
                i -= 1;
                j -= 1;
                break;
            case 1:
                model.setSrcHash(hashSrc);
                model.setDstHash(0);
                model.setStatus(Removed);
                i -= 1;
                break;
            case 2:
                model.setSrcHash(0);
                model.setDstHash(hashDst);
                model.setStatus(Added);
                j -= 1;
                break;
        }

        mModelLst.append(model);
    }
}

