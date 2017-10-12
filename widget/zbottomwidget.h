#ifndef ZBOTTOMWIDGET
#define ZBOTTOMWIDGET

#include <QtWidgets>

class ZBottomWidget : public QStatusBar{
    Q_OBJECT

public:
    ZBottomWidget(QWidget *parent = 0);
    ~ZBottomWidget();

public slots:
    void onProgress(int value, int maxValue = 100);

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QProgressBar *mProgressBar;
};

#endif // ZBOTTOMWIDGET

