#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "widget/ztopwidget.h"
#include "widget/zcenterwidget.h"

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
    ZTopWidget *m_top_widget;
    ZCenterWidget *m_center_widget;
};

#endif // MAINWINDOW_H
