#include "zfilediffmodel.h"

ZFileDiffModel::ZFileDiffModel(){

}

ZFileDiffModel::~ZFileDiffModel(){

}

QString ZFileDiffModel::srcLine() const
{
    return mSrcLine;
}

void ZFileDiffModel::setSrcLine(const QString &srcLine)
{
    mSrcLine = srcLine;
}

QString ZFileDiffModel::dstLine() const
{
    return mDstLine;
}

void ZFileDiffModel::setDstLine(const QString &dstLine)
{
    mDstLine = dstLine;
}

int ZFileDiffModel::status() const
{
    return mStatus;
}

void ZFileDiffModel::setStatus(int status)
{
    mStatus = status;
}
