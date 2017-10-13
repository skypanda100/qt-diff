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

unsigned int ZFileDiffModel::srcHash() const
{
    return mSrcHash;
}

void ZFileDiffModel::setSrcHash(unsigned int srcHash)
{
    mSrcHash = srcHash;
}

unsigned int ZFileDiffModel::dstHash() const
{
    return mDstHash;
}

void ZFileDiffModel::setDstHash(unsigned int dstHash)
{
    mDstHash = dstHash;
}

Status ZFileDiffModel::status() const
{
    return mStatus;
}

void ZFileDiffModel::setStatus(Status status)
{
    mStatus = status;
}
