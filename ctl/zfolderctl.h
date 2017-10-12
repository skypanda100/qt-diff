#ifndef ZFOLDERCTL
#define ZFOLDERCTL

#include <QThread>
#include "diff/zpathdiffmodel.h"
#include "widget/tree/ztreeitemmodel.h"

class ZFolderCtl : public QThread{
    Q_OBJECT

public:
    ZFolderCtl(QString srcBasePath, QString dstBasePath, QList<ZPathDiffModel> &pathModelLst, QObject *parent = 0);
    ~ZFolderCtl();

    void run() Q_DECL_OVERRIDE;

signals:
    void diffMessage(const QList<ZTreeItemModel> &itemModelList);
    void diffEnd();

private:
    QString mSrcBasePath;
    QString mDstBasePath;
    QList<ZPathDiffModel> &mPathModelLst;
};

#endif // ZFOLDERCTL

