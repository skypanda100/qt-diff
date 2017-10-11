#include "zdiffmodel.h"

ZDiffModel::ZDiffModel(){

}

ZDiffModel::~ZDiffModel(){

}

QString ZDiffModel::srcLine() const
{
    return mSrcLine;
}

void ZDiffModel::setSrcLine(const QString &srcLine)
{
    mSrcLine = srcLine;
}

QString ZDiffModel::dstLine() const
{
    return mDstLine;
}

void ZDiffModel::setDstLine(const QString &dstLine)
{
    mDstLine = dstLine;
}

int ZDiffModel::status() const
{
    return mStatus;
}

void ZDiffModel::setStatus(int status)
{
    mStatus = status;
}
