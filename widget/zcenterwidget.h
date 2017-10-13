#ifndef ZCENTERWIDGET
#define ZCENTERWIDGET

#include <QtWidgets>

class ZTabBar : public QTabBar{
    Q_OBJECT

public:
    ZTabBar(QWidget *parent = 0);
    ~ZTabBar();

signals:
    void closeTab(int);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    friend class ZCenterWidget;
    QPoint mDragStartPos;
};

class ZCenterWidget : public QTabWidget{
    Q_OBJECT

public:
    ZCenterWidget(QWidget *parent = 0);
    ~ZCenterWidget();

signals:
    void progress(int value, int maxValue);

public slots:
    void closeTab(int index = -1);
    void folderComparison();
    void startOrRecompare();
    void stopCompare();

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private:
    ZTabBar *mTabBar;
};

#endif // ZCENTERWIDGET

