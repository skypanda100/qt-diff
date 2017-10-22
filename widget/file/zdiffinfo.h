#ifndef ZDIFFINFO
#define ZDIFFINFO

#include <QList>
#include "env/zcons.h"

class ZDiffInfo
{
public:
    ZDiffInfo();
    ~ZDiffInfo();

    void setDiffLst(QList<int> diffLst);
    QList<int> diffLst() const;

    void setLine(bool isLine);
    bool isLine() const;

    void setStatus(Status status);
    Status status() const;

private:
    QList<int> mDiffLst;
    bool mIsLine;
    Status mStatus;
};


#endif // ZDIFFINFO

