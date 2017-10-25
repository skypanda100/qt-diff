#ifndef ZTEXTWIDGET
#define ZTEXTWIDGET

#include <QtWidgets>
#include "zdiffinfo.h"

class ZTextWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    ZTextWidget(QWidget *parent = 0);
    ~ZTextWidget();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    bool isBlockContained(ZDiffInfo diffInfo);
    QRectF blockArea(ZDiffInfo diffInfo);
    int visibleBlockCount();

signals:
    void scrollValueChanged(int value);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

private:
    void initData();
    void initUI();
    void initConnect();
    void updateBlockNo();

private:
    QWidget *mLineNumberArea;
    int mFirstVisibleBlockNo;
    int mLastVisibleBlockNo;
};

#endif // ZTEXTWIDGET

