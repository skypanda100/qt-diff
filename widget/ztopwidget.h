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
    void startOrRecompare();
    void stopCompare();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QToolButton *mFolderButton;
    QToolButton *mRefreshButton;
    QToolButton *mStopButton;
};

#endif // ZTOPWIDGET

