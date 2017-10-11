#ifndef ZFOLDERWIDGET
#define ZFOLDERWIDGET

#include <QtWidgets>

class ZFolderWidget : public QWidget{
    Q_OBJECT

public:
    ZFolderWidget(QWidget *parent = 0);
    ~ZFolderWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void searchClicked();

private:
    QStringList mHeader;
    QTreeView *mTreeView;
    QLineEdit *mPathEditSrc;
    QPushButton *mSearchButtonSrc;
    QLineEdit *mPathEditDst;
    QPushButton *mSearchButtonDst;
};

#endif // ZFOLDERWIDGET

