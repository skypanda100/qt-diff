#include "zfolderctl.h"
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"
#include "util/zfile.h"
#include "env/zcons.h"

ZFolderCtl::ZFolderCtl(QString srcBasePath, QString dstBasePath, QList<ZPathDiffModel> &mPathModelLst, QObject *parent)
    : QThread(parent)
    , mPathModelLst(mPathModelLst)
{
    mSrcBasePath = srcBasePath;
    mDstBasePath = dstBasePath;
    mIsRunning = false;
}

ZFolderCtl::~ZFolderCtl()
{

}

void ZFolderCtl::run()
{
    mIsRunning = true;
    int modelCount = mPathModelLst.size();
    for(int i = 0;i < modelCount;i++)
    {
        if(!mIsRunning)
        {
            break;
        }
        ZPathDiffModel pathDiffModel = mPathModelLst[i];
        int no = i + 1;
        QString path;
        QIcon icon;
        QString extension;
        Status status = pathDiffModel.status();
        QString sts;
        int lineAdded = 0;
        int lineRemoved = 0;
        int lineModified = 0;
        QColor color = STATUS_CLR[0];

        if(status == Same)
        {
            path = pathDiffModel.srcFileInfo().absoluteFilePath().remove(0, mSrcBasePath.length());
            extension = pathDiffModel.srcFileInfo().suffix();
            icon = ZFile::icon(pathDiffModel.srcFileInfo().absoluteFilePath());

            ZFileDiff fileDiff(pathDiffModel.srcFileInfo().absoluteFilePath(), pathDiffModel.dstFileInfo().absoluteFilePath());
            QList<ZFileDiffModel> modelLst = fileDiff.execute();
            for(int j = modelLst.size() - 2;j >= 0;j--)
            {
                ZFileDiffModel model = modelLst[j];
                if(model.status() == Same)
                {

                }
                else if(model.status() == Modified)
                {
                    lineModified++;
                }
                else if(model.status() == Removed)
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
                status = Modified;
                pathDiffModel.setStatus(status);
                mPathModelLst[i] = pathDiffModel;
            }
        }
        else if(status == Removed)
        {
            path = pathDiffModel.srcFileInfo().absoluteFilePath().remove(0, mSrcBasePath.length());
            extension = pathDiffModel.srcFileInfo().suffix();
            icon = ZFile::icon(pathDiffModel.srcFileInfo().absoluteFilePath());

            QFile file(pathDiffModel.srcFileInfo().absoluteFilePath());
            QList<unsigned int> hashLst;
            lineRemoved = ZFile::linesWithHash(&file, hashLst);
        }
        else if(status == Added)
        {
            path = pathDiffModel.dstFileInfo().absoluteFilePath().remove(0, mDstBasePath.length());
            extension = pathDiffModel.dstFileInfo().suffix();
            icon = ZFile::icon(pathDiffModel.dstFileInfo().absoluteFilePath());

            QFile file(pathDiffModel.dstFileInfo().absoluteFilePath());
            QList<unsigned int> hashLst;
            lineAdded = ZFile::linesWithHash(&file, hashLst);
        }
        else
        {

        }

        sts = STATUS_STR[(int)status];
        color = STATUS_CLR[(int)status];

        QList<ZTreeItemModel> itemModelList;

        ZTreeItemModel noItemModel;
        noItemModel.setHasIcon(false);
        noItemModel.setValue(no);
        noItemModel.setColor(color);
        itemModelList.append(noItemModel);

        ZTreeItemModel pathItemModel;
        pathItemModel.setHasIcon(true);
        pathItemModel.setIcon(icon);
        pathItemModel.setValue(path);
        pathItemModel.setColor(color);
        itemModelList.append(pathItemModel);

        ZTreeItemModel extItemModel;
        extItemModel.setHasIcon(false);
        extItemModel.setValue(extension);
        extItemModel.setColor(color);
        itemModelList.append(extItemModel);

        ZTreeItemModel stsItemModel;
        stsItemModel.setHasIcon(false);
        stsItemModel.setValue(sts);
        stsItemModel.setColor(color);
        itemModelList.append(stsItemModel);

        ZTreeItemModel laItemModel;
        laItemModel.setHasIcon(false);
        laItemModel.setValue(lineAdded);
        laItemModel.setColor(color);
        itemModelList.append(laItemModel);

        ZTreeItemModel lrItemModel;
        lrItemModel.setHasIcon(false);
        lrItemModel.setValue(lineRemoved);
        lrItemModel.setColor(color);
        itemModelList.append(lrItemModel);

        ZTreeItemModel lmItemModel;
        lmItemModel.setHasIcon(false);
        lmItemModel.setValue(lineModified);
        lmItemModel.setColor(color);
        itemModelList.append(lmItemModel);

        emit diffMessage(itemModelList);
        emit progress(i + 1, modelCount);
    }
    stopRunAndDelete();
}

void ZFolderCtl::stopRunAndDelete()
{
    mIsRunning = false;
    quit();
    wait();

    emit progress(100, 100);
    emit diffEnd();
}
