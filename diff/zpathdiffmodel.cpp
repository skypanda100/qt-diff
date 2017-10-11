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

int ZPathDiffModel::status() const
{
    return mStatus;
}

void ZPathDiffModel::setStatus(int status)
{
    mStatus = status;
}
