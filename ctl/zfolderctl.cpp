#include "zfolderctl.h"
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"
#include "util/zfile.h"

ZFolderCtl::ZFolderCtl(QString srcBasePath, QString dstBasePath, QList<ZPathDiffModel> &mPathModelLst, QObject *parent)
    : QThread(parent)
    , mPathModelLst(mPathModelLst)
{
    mSrcBasePath = srcBasePath;
    mDstBasePath = dstBasePath;
}

ZFolderCtl::~ZFolderCtl()
{

}

void ZFolderCtl::run()
{
    int modelCount = mPathModelLst.size();
    for(int i = 0;i < modelCount;i++)
    {
        ZPathDiffModel pathDiffModel = mPathModelLst[i];
        int no = i + 1;
        QString path;
        QString extension;
        int status = pathDiffModel.status();
        QString sts;
        int lineAdded = 0;
        int lineRemoved = 0;
        int lineModified = 0;

        if(status == 0)
        {
            path = pathDiffModel.srcFileInfo().absoluteFilePath().remove(0, mSrcBasePath.length());
            extension = pathDiffModel.srcFileInfo().completeSuffix();

            ZFileDiff fileDiff(pathDiffModel.srcFileInfo().absoluteFilePath(), pathDiffModel.dstFileInfo().absoluteFilePath());
            QList<ZFileDiffModel> modelLst = fileDiff.execute();
            for(int j = modelLst.size() - 2;j >= 0;j--)
            {
                ZFileDiffModel model = modelLst[j];
                if(model.status() == 0)
                {

                }
                else if(model.status() == 1)
                {
                    lineModified++;
                }
                else if(model.status() == 2)
                {
                    lineRemoved++;
                }
                else
                {
                    lineAdded++;
                }
            }
            if(lineModified != 0 || lineRemoved != 0 || lineAdded != 0)
            {
                status = 1;
                pathDiffModel.setStatus(status);
                mPathModelLst[i] = pathDiffModel;
            }
        }
        else if(status == 2)
        {
            path = pathDiffModel.srcFileInfo().absoluteFilePath().remove(0, mSrcBasePath.length());
            extension = pathDiffModel.srcFileInfo().completeSuffix();
            QFile file(pathDiffModel.srcFileInfo().absoluteFilePath());
            QList<QString> lineLst;
            lineRemoved = ZFile::lines(&file, lineLst);
        }
        else if(status == 3)
        {
            path = pathDiffModel.dstFileInfo().absoluteFilePath().remove(0, mDstBasePath.length());
            extension = pathDiffModel.dstFileInfo().completeSuffix();
            QFile file(pathDiffModel.dstFileInfo().absoluteFilePath());
            QList<QString> lineLst;
            lineAdded = ZFile::lines(&file, lineLst);
        }
        else
        {

        }

        switch(status)
        {
        case 0:
            sts = "Same";
            break;
        case 1:
            sts = "Modified";
            break;
        case 2:
            sts = "Removed";
            break;
        case 3:
            sts = "Added";
            break;
        }

        emit diffMessage(no, path, extension, sts, lineAdded, lineRemoved, lineModified);
    }
    emit diffEnd();
}
