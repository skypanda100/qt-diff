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
    void clearText();
    void setTextToFile(const QString &path);

signals:
    void textChanged();

public slots:
    void onScrollValueChange(int value);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void setVerticalRange(int min, int max);
    void setHorizontalRange(int min, int max);
    void onCursorPositionChanged();
    void onVerticalValueChange(int value);
    void onHorizontablValueChanged(int value);

private:
    Qt::Alignment mVerticalAlignment;
    ZTextWidget *mTextWidget;
    QScrollBar *mVerticalBar;
    QScrollBar *mHorizontalBar;
    QWidget *mBelowWidget;
    ZDiffAreaWidget *mDiffAreaWidget;
    QWidget *mVerticalWidget;
    QWidget *mHorizontalWidget;
    QWidget *mAboveWidget;
};

#endif // ZSCROLLTEXTWIDGET

