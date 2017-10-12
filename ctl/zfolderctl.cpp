#include "zfolderctl.h"
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"
#include "util/zfile.h"
#include "env/cons.h"

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
        QIcon icon;
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
            icon = ZFile::icon(pathDiffModel.srcFileInfo().absoluteFilePath());

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
            icon = ZFile::icon(pathDiffModel.srcFileInfo().absoluteFilePath());

            QFile file(pathDiffModel.srcFileInfo().absoluteFilePath());
            QList<QString> lineLst;
            lineRemoved = ZFile::lines(&file, lineLst);
        }
        else if(status == 3)
        {
            path = pathDiffModel.dstFileInfo().absoluteFilePath().remove(0, mDstBasePath.length());
            extension = pathDiffModel.dstFileInfo().completeSuffix();
            icon = ZFile::icon(pathDiffModel.dstFileInfo().absoluteFilePath());

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

        QList<ZTreeItemModel> itemModelList;

        ZTreeItemModel noItemModel;
        noItemModel.setHasIcon(false);
        noItemModel.setValue(no);
        itemModelList.append(noItemModel);

        ZTreeItemModel pathItemModel;
        pathItemModel.setHasIcon(true);
        pathItemModel.setIcon(icon);
        pathItemModel.setValue(path);
        itemModelList.append(pathItemModel);

        ZTreeItemModel extItemModel;
        extItemModel.setHasIcon(false);
        extItemModel.setValue(extension);
        itemModelList.append(extItemModel);

        ZTreeItemModel stsItemModel;
        stsItemModel.setHasIcon(false);
        stsItemModel.setValue(sts);
        itemModelList.append(stsItemModel);

        ZTreeItemModel laItemModel;
        laItemModel.setHasIcon(false);
        laItemModel.setValue(lineAdded);
        itemModelList.append(laItemModel);

        ZTreeItemModel lrItemModel;
        lrItemModel.setHasIcon(false);
        lrItemModel.setValue(lineRemoved);
        itemModelList.append(lrItemModel);

        ZTreeItemModel lmItemModel;
        lmItemModel.setHasIcon(false);
        lmItemModel.setValue(lineModified);
        itemModelList.append(lmItemModel);

        emit diffMessage(itemModelList);
    }
    emit diffEnd();
}
