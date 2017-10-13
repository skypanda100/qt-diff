#ifndef ZFILEDIFFMODEL
#define ZFILEDIFFMODEL

#include <QString>
#include "env/zcons.h"

class ZFileDiffModel{

public:
    ZFileDiffModel();
    ~ZFileDiffModel();

    QString line() const;
    void setLine(const QString &line);

    Status status() const;
    void setStatus(Status status);

    QString srcLine() const;
    void setSrcLine(const QString &srcLine);

    QString dstLine() const;
    void setDstLine(const QString &dstLine);

    unsigned int srcHash() const;
    void setSrcHash(unsigned int srcHash);

    unsigned int dstHash() const;
    void setDstHash(unsigned int dstHash);

private:
    unsigned int mSrcHash;
    unsigned int mDstHash;
    QString mSrcLine;
    QString mDstLine;
    Status mStatus; //0:same 1:modify 2:delete 3:add
};

#endif // ZFILEDIFFMODEL

