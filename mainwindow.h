#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "widget/ztopwidget.h"
#include "widget/zcenterwidget.h"
#include "widget/zbottomwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    ZTopWidget *mTopWidget;
    ZCenterWidget *mCenterWidget;
    ZBottomWidget *mBottomWidget;
};

#endif // MAINWINDOW_H
