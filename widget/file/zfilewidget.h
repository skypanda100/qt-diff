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
#include <QTimer>

class ZFileWidget : public QWidget
{
    Q_OBJECT
public:
    ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent = 0);
    ~ZFileWidget();

public slots:
    void compare();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();
    void clearData();
    void setFilePath();
    void setTempFile();
    void resetTempFile();
    void getLineFromFile();
    void getDiffInfo();
    void getDiffArea();
    void setText();
    void setDiffInfo();

private slots:
    void onScrollValueChanged(int value);
    void onSearchClicked();
    void onTimeout();
    void onTextChanged();

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

    QTimer *mTimer;
};

#endif // ZFILEWIDGET

