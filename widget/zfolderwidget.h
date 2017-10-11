#ifndef ZFOLDERWIDGET
#define ZFOLDERWIDGET

#include <QtWidgets>

class ZFolderWidget : public QTreeView{
    Q_OBJECT

public:
    ZFolderWidget(QWidget *parent = 0);
    ~ZFolderWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QStringList m_header;
};

#endif // ZFOLDERWIDGET

