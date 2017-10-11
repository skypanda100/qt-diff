#ifndef ZTOPWIDGET
#define ZTOPWIDGET

#include <QtWidgets>

class ZTopWidget : public QToolBar{
    Q_OBJECT
public:
    ZTopWidget(QWidget *parent = 0);
    ~ZTopWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QToolButton *m_folder_button;
    QToolButton *m_refresh_button;
};

#endif // ZTOPWIDGET

