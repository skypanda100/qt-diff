#ifndef ZLINENUMBERWIDGET
#define ZLINENUMBERWIDGET

#include <QtWidgets>
#include "ztextwidget.h"

class ZLineNumberWidget : public QWidget
{
    Q_OBJECT

public:
    ZLineNumberWidget(ZTextWidget *parent = 0);
    ~ZLineNumberWidget();

    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private:
    ZTextWidget *mTextWidget;
};

#endif // ZLINENUMBERWIDGET

