#ifndef ZDIFFAREAWIDGET
#define ZDIFFAREAWIDGET

#include <QtWidgets>
#include "env/zcons.h"
#include "ztextwidget.h"

class ZDiffAreaWidget : public QWidget
{
    Q_OBJECT
public:
    ZDiffAreaWidget(ZTextWidget *textWidget = 0, QWidget *parent = 0);
    ~ZDiffAreaWidget();
    void setDiffList(QList<ZDiffInfo> diffLst);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QList<ZDiffInfo> mDiffLst;
    ZTextWidget *mTextWidget;
};

#endif // ZDIFFAREAWIDGET

