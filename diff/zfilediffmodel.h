#ifndef ZFILEDIFFMODEL
#define ZFILEDIFFMODEL

#include <QString>

class ZFileDiffModel{

public:
    ZFileDiffModel();
    ~ZFileDiffModel();

    QString line() const;
    void setLine(const QString &line);

    int status() const;
    void setStatus(int status);

    QString srcLine() const;
    void setSrcLine(const QString &srcLine);

    QString dstLine() const;
    void setDstLine(const QString &dstLine);

private:
    QString mSrcLine;
    QString mDstLine;
    int mStatus; //0:same 1:modify 2:delete 3:add
};

#endif // ZFILEDIFFMODEL

