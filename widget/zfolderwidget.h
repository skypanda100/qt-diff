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
    void stopCompare();

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();
    void clearAll();
    void insert(const QList<ZTreeItemModel> &itemModelList);
    void updateInfo();

private slots:
    void searchClicked();
    void onDiffMessage(const QList<ZTreeItemModel> &itemModelList);
    void onDiffEnd();
    void onProgress(int value, int maxValue);

private:
    int mFileAdded;
    int mFileRemoved;
    int mFileModified;
    int mLineAdded;
    int mLineRemoved;
    int mLineModified;
    QString mFileInfo;
    QString mLineInfo;
    QLabel *mFileLabel;
    QLabel *mLineLabel;
    QStringList mHeader;
    QTreeView *mTreeView;
    QLineEdit *mPathEditSrc;
    QPushButton *mSearchButtonSrc;
    QLineEdit *mPathEditDst;
    QPushButton *mSearchButtonDst;
    ZFolderCtl *mFolderCtl;
    QList<ZPathDiffModel> mPathModelLst;
    int mValue;
    int mMaxValue;
};

#endif // ZFOLDERWIDGET

