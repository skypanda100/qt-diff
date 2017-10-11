#ifndef ZDIFFERENCE
#define ZDIFFERENCE

#include <QString>
#include <QFile>
#include "zdiffmodel.h"

class ZDifference {

public:
    ZDifference();
    ZDifference(const QString &fileSrc, const QString &fileDst);
    ~ZDifference();

    void setFileSrc(const QString &fileSrc);
    void setFileDst(const QString &fileDst);

    void execute();

private:
    bool initRect();
    bool makeRect();
    void recallRect();

private:
    QFile *mFileSrc;
    QFile *mFileDst;

    short **mDiffRect;
    int mDiffRectRows;
    int mDiffRectCols;

    QList<QString> mLineSrcLst;
    QList<QString> mLineDstLst;
    QList<ZDiffModel> mModelLst;
};

#endif // ZDIFFERENCE

