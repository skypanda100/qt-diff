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

Status ZFileDiffModel::status() const
{
    return mStatus;
}

void ZFileDiffModel::setStatus(Status status)
{
    mStatus = status;
}
