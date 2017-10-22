#include "zdiffinfo.h"


ZDiffInfo::ZDiffInfo()
{
    this->setStatus(Same);
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

