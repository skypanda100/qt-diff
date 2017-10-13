#ifndef ZBOTTOMWIDGET
#define ZBOTTOMWIDGET

#include <QtWidgets>

class ZBottomWidget : public QStatusBar{
    Q_OBJECT

public:
    ZBottomWidget(QWidget *parent = 0);
    ~ZBottomWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
};

#endif // ZBOTTOMWIDGET

