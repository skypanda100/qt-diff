#ifndef ZFOLDERWIDGET
#define ZFOLDERWIDGET

#include <QtWidgets>

class ZFolderWidget : public QWidget{
    Q_OBJECT

public:
    ZFolderWidget(QWidget *parent = 0);
    ~ZFolderWidget();

    void compare();

private:
    void initData();
    void initUI();
    void initConnect();
    void clearAll();

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

