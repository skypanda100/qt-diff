#ifndef ZFOLDERWIDGET
#define ZFOLDERWIDGET

#include <QtWidgets>
#include "ctl/zfolderctl.h"
#include "diff/zpathdiff.h"
#include "diff/zpathdiffmodel.h"
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"

class ZFolderWidget : public QWidget{
    Q_OBJECT

public:
    ZFolderWidget(QWidget *parent = 0);
    ~ZFolderWidget();

    void compare();

private:
    void initData();
    void initUI();
    void initConnect();
    void clearAll();
    void insert(int no, const QString &path
                , const QString &extension, const QString &status
                , int lineAdded, int lineRemoved
                , int lineModified);

private slots:
    void searchClicked();
    void onDiffMessage(int no, const QString &path
                     , const QString &extension, const QString &status
                     , int lineAdded, int lineRemoved
                     , int lineModified);
    void onDiffEnd();

private:
    QStringList mHeader;
    QTreeView *mTreeView;
    QLineEdit *mPathEditSrc;
    QPushButton *mSearchButtonSrc;
    QLineEdit *mPathEditDst;
    QPushButton *mSearchButtonDst;
    ZFolderCtl *mFolderCtl;
    QList<ZPathDiffModel> mPathModelLst;
};

#endif // ZFOLDERWIDGET

