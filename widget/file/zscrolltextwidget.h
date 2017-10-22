#ifndef ZSCROLLTEXTWIDGET
#define ZSCROLLTEXTWIDGET

#include <QtWidgets>
#include "ztextwidget.h"
#include "zdiffareawidget.h"

class ZScrollTextWidget : public QWidget
{
    Q_OBJECT
public:
    ZScrollTextWidget(Qt::Alignment verticalAlignment, QWidget *parent = 0);
    ~ZScrollTextWidget();

    void appendText(const QString &text);
    void setDiffList(QList<ZDiffInfo> diffLst);
    bool isBlockContained(ZDiffInfo diffInfo);
    QRectF blockArea(ZDiffInfo diffInfo);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void setVerticalRange(int min, int max);
    void setHorizontalRange(int min, int max);
    void onVerticalValueChange(int value);
    void onHorizontablValueChanged(int value);
    void onCursorPositionChanged();

private:
    Qt::Alignment mVerticalAlignment;
    ZTextWidget *mTextWidget;
    QScrollBar *mVerticalBar;
    QScrollBar *mHorizontalBar;
    QWidget *mBelowWidget;
    ZDiffAreaWidget *mAboveWidget;
};

#endif // ZSCROLLTEXTWIDGET

