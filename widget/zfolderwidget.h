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
    void insert(const QList<ZTreeItemModel> &itemModelList);

private slots:
    void searchClicked();
    void onDiffMessage(const QList<ZTreeItemModel> &itemModelList);
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

