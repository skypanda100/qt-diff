#include "zpathdiff.h"
#include <QFileInfo>
#include "util/zfile.h"
#include "env/zcons.h"

ZPathDiff::ZPathDiff()
{

}

ZPathDiff::ZPathDiff(const QString &pathSrc, const QString &pathDst)
{
    mPathSrc = pathSrc;
    mPathDst = pathDst;
}

ZPathDiff::~ZPathDiff()
{

}

void ZPathDiff::setPathSrc(const QString &pathSrc)
{
    mPathSrc = pathSrc;
}

void ZPathDiff::setPathDst(const QString &pathDst)
{
    mPathDst = pathDst;
}

QList<ZPathDiffModel> ZPathDiff::execute()
{
    QList<ZPathDiffModel> pathModelLst;
    QFileInfoList srcFileInfoList = ZFile::files(mPathSrc);
    QFileInfoList dstFileInfoList = ZFile::files(mPathDst);

    int srcFileCount = srcFileInfoList.size();
    int dstFileCount = dstFileInfoList.size();

    if(srcFileCount == 0 && dstFileCount == 0)
    {

    }
    else if(srcFileCount == 0 && dstFileCount > 0)
    {
        for(int i = 0;i < dstFileCount;i++)
        {
            QFileInfo dstFileInfo = dstFileInfoList[i];

            ZPathDiffModel model;
            model.setDstFileInfo(dstFileInfo);
            model.setStatus(Added);

            pathModelLst.append(model);
        }
    }
    else if(srcFileCount > 0 && dstFileCount == 0)
    {
        for(int i = 0;i < srcFileCount;i++)
        {
            QFileInfo srcFileInfo = srcFileInfoList[i];

            ZPathDiffModel model;
            model.setSrcFileInfo(srcFileInfo);
            model.setStatus(Removed);

            pathModelLst.append(model);
        }
    }
    else
    {
        for(int i = 0;i < srcFileCount;i++)
        {
            QFileInfo srcFileInfo = srcFileInfoList[i];
            QString srcPathWithoutBase = srcFileInfo.absoluteFilePath().remove(0, mPathSrc.length());
            int j = 0;
            dstFileCount = dstFileInfoList.size();
            for(;j < dstFileCount;j++)
            {
                QFileInfo dstFileInfo = dstFileInfoList[j];
                QString dstPathWithoutBase = dstFileInfo.absoluteFilePath().remove(0, mPathDst.length());

                if(QString::compare(srcPathWithoutBase, dstPathWithoutBase) == 0)
                {
                    ZPathDiffModel model;
                    model.setSrcFileInfo(srcFileInfo);
                    model.setDstFileInfo(dstFileInfo);
                    model.setStatus(Same);

                    pathModelLst.append(model);
                    dstFileInfoList.removeOne(dstFileInfo);
                    break;
                }
            }
            if(j == dstFileCount)
            {
                ZPathDiffModel model;
                model.setSrcFileInfo(srcFileInfo);
                model.setStatus(Removed);

                pathModelLst.append(model);
            }
        }

        dstFileCount = dstFileInfoList.size();
        for(int i = 0;i < dstFileCount;i++)
        {
            QFileInfo dstFileInfo = dstFileInfoList[i];

            ZPathDiffModel model;
            model.setDstFileInfo(dstFileInfo);
            model.setStatus(Added);

            pathModelLst.append(model);
        }
    }

    return pathModelLst;
}
