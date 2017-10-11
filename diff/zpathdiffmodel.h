#ifndef ZPATHDIFFMODEL
#define ZPATHDIFFMODEL

#include <QFileInfo>

class ZPathDiffModel{

public:
    ZPathDiffModel();
    ~ZPathDiffModel();

    QFileInfo srcFileInfo() const;
    void setSrcFileInfo(const QFileInfo &srcFileInfo);

    QFileInfo dstFileInfo() const;
    void setDstFileInfo(const QFileInfo &dstFileInfo);

    int status() const;
    void setStatus(int status);

private:
    QFileInfo mSrcFileInfo;
    QFileInfo mDstFileInfo;
    int mStatus; //0:same 1:modify 2:delete 3:add
};

#endif // ZPATHDIFFMODEL

