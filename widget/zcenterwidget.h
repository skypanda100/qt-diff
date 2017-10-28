#ifndef ZCENTERWIDGET
#define ZCENTERWIDGET

#include <QtWidgets>
#include "diff/zpathdiffmodel.h"

class ZCenterWidget : public QWidget{
    Q_OBJECT

public:
    ZCenterWidget(QWidget *parent = 0);
    ~ZCenterWidget();

public slots:
    void closeTab(int index = -1);
    void folderComparison();
    void fileComparison();
    void startOrRecompare();
    void stopCompare();
    void fileCompare(ZPathDiffModel pathDiffModel);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QTabWidget *mTabWidget;
    int mTextCount;
    int mFolderCount;
};

#endif // ZCENTERWIDGET

