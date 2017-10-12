#ifndef ZPATHDIFFMODEL
#define ZPATHDIFFMODEL

#include <QFileInfo>
#include "env/cons.h"

class ZPathDiffModel{

public:
    ZPathDiffModel();
    ~ZPathDiffModel();

    QFileInfo srcFileInfo() const;
    void setSrcFileInfo(const QFileInfo &srcFileInfo);

    QFileInfo dstFileInfo() const;
    void setDstFileInfo(const QFileInfo &dstFileInfo);

    Status status() const;
    void setStatus(Status status);

private:
    QFileInfo mSrcFileInfo;
    QFileInfo mDstFileInfo;
    Status mStatus; //0:same 1:modify 2:delete 3:add
};

#endif // ZPATHDIFFMODEL

