#ifndef ZTOPWIDGET
#define ZTOPWIDGET

#include <QtWidgets>

class ZTopWidget : public QToolBar{
    Q_OBJECT
public:
    ZTopWidget(QWidget *parent = 0);
    ~ZTopWidget();

signals:
    void folderComparison();
    void fileComparison();
    void startOrRecompare();
    void stopCompare();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QToolButton *mFolderButton;
    QToolButton *mFileButton;
    QToolButton *mRefreshButton;
    QToolButton *mStopButton;
};

#endif // ZTOPWIDGET

