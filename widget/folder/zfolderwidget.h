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

public slots:
    void compare();
    void stopCompare();

signals:
    void fileCompare(ZPathDiffModel pathDiffModel);

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
    void onSearchClicked();
    void onDiffMessage(const QList<ZTreeItemModel> &itemModelList);
    void onDiffEnd();
    void onProgress(int value, int maxValue);
    void onDoubleClicked(const QModelIndex & index);

private:
    int mFileAdded;
    int mFileRemoved;
    int mFileModified;
    int mLineAdded;
    int mLineRemoved;
    int mLineModified;
    QTime mTime;
    QString mFileInfo;
    QString mLineInfo;
    QString mTimeInfo;
    QLabel *mFileLabel;
    QLabel *mLineLabel;
    QLabel *mTimeLabel;
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

