#ifndef ZPATHDIFFMODEL
#define ZPATHDIFFMODEL

#include <QFileInfo>
#include "env/zcons.h"

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

    QFileInfo srcTempFileInfo() const;
    void setSrcTempFileInfo(const QFileInfo &srcTempFileInfo);

    QFileInfo dstTempFileInfo() const;
    void setDstTempFileInfo(const QFileInfo &dstTempFileInfo);

    void deleteTempFile();

private:
    QFileInfo mSrcFileInfo;
    QFileInfo mDstFileInfo;
    QFileInfo mSrcTempFileInfo;
    QFileInfo mDstTempFileInfo;
    Status mStatus; //0:same 1:modify 2:delete 3:add
};

#endif // ZPATHDIFFMODEL

