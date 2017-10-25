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
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();
    void clearData();
    void setFilePath();
    void setTempFile();
    void getLineFromFile();
    void getDiffInfo();
    void getDiffArea();

private slots:
    void onScrollValueChanged(int value);
    void onSearchClicked();

private:
    ZPathDiffModel mPathDiffModel;
    QList<ZFileDiffModel> mModelLst;
    QList<QString> mSrcLineLst;
    QList<QString> mDstLineLst;
    QList<ZDiffInfo> mSrcDiffLst;
    QList<ZDiffInfo> mDstDiffLst;
    QList<ZDiffArea> mSrcDiffAreaLst;
    QList<ZDiffArea> mDstDiffAreaLst;
    QLineEdit *mPathEditSrc;
    QPushButton *mSearchButtonSrc;
    QLineEdit *mPathEditDst;
    QPushButton *mSearchButtonDst;
    ZScrollTextWidget *mSrcScrollTextWidget;
    ZScrollTextWidget *mDstScrollTextWidget;
};

#endif // ZFILEWIDGET

