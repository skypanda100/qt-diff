#include "zpathdiffmodel.h"

ZPathDiffModel::ZPathDiffModel()
{

}

ZPathDiffModel::~ZPathDiffModel()
{

}

QFileInfo ZPathDiffModel::srcFileInfo() const
{
    return mSrcFileInfo;
}

void ZPathDiffModel::setSrcFileInfo(const QFileInfo &srcFileInfo)
{
    mSrcFileInfo = srcFileInfo;
}

QFileInfo ZPathDiffModel::dstFileInfo() const
{
    return mDstFileInfo;
}

void ZPathDiffModel::setDstFileInfo(const QFileInfo &dstFileInfo)
{
    mDstFileInfo = dstFileInfo;
}

Status ZPathDiffModel::status() const
{
    return mStatus;
}

void ZPathDiffModel::setStatus(Status status)
{
    mStatus = status;
}

QFileInfo ZPathDiffModel::srcTempFileInfo() const
{
    return mSrcTempFileInfo;
}

void ZPathDiffModel::setSrcTempFileInfo(const QFileInfo &srcTempFileInfo)
{
    mSrcTempFileInfo = srcTempFileInfo;
}

QFileInfo ZPathDiffModel::dstTempFileInfo() const
{
    return mDstTempFileInfo;
}

void ZPathDiffModel::setDstTempFileInfo(const QFileInfo &dstTempFileInfo)
{
    mDstTempFileInfo = dstTempFileInfo;
}
