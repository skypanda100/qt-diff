#ifndef ZFOLDERCTL
#define ZFOLDERCTL

#include <QThread>
#include "diff/zpathdiffmodel.h"

class ZFolderCtl : public QThread{
    Q_OBJECT

public:
    ZFolderCtl(QString srcBasePath, QString dstBasePath, QList<ZPathDiffModel> &pathModelLst, QObject *parent = 0);
    ~ZFolderCtl();

    void run() Q_DECL_OVERRIDE;

signals:
    void diffMessage(int no, const QString &path
                     , const QString &extension, const QString &status
                     , int lineAdded, int lineRemoved
                     , int lineModified);
    void diffEnd();

private:
    QString mSrcBasePath;
    QString mDstBasePath;
    QList<ZPathDiffModel> &mPathModelLst;
};

#endif // ZFOLDERCTL

