#ifndef ZFILEWIDGET
#define ZFILEWIDGET

#include <QtWidgets>
#include "env/zcons.h"
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"
#include "diff/zpathdiff.h"
#include "diff/zpathdiffmodel.h"
#include "zdiffinfo.h"
#include "zscrolltextwidget.h"
#include "zdiffarea.h"

class ZFileWidget : public QWidget
{
    Q_OBJECT
public:
    ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent = 0);
    ~ZFileWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();
    void clearData();

private slots:
    void onScrollValueChanged(int value);

private:
    ZPathDiffModel mPathDiffModel;
    QList<ZFileDiffModel> mModelLst;
    QList<QString> mSrcLineLst;
    QList<QString> mDstLineLst;
    QList<ZDiffInfo> mSrcDiffLst;
    QList<ZDiffInfo> mDstDiffLst;
    QList<ZDiffArea> mSrcDiffAreaLst;
    QList<ZDiffArea> mDstDiffAreaLst;
    ZScrollTextWidget *mSrcScrollTextWidget;
    ZScrollTextWidget *mDstScrollTextWidget;
};

#endif // ZFILEWIDGET

