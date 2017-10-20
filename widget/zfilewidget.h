#ifndef ZFILEWIDGET
#define ZFILEWIDGET

#include <QtWidgets>
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"
#include "diff/zpathdiff.h"
#include "diff/zpathdiffmodel.h"

class ZLineNumberWidget;
class ZDiffAreaWidget;
class ZTextWidget;
class ZFileWidget;

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

class ZDiffAreaWidget : public QWidget
{
    Q_OBJECT
public:
    ZDiffAreaWidget(ZTextWidget *textWidget = 0, QWidget *parent = 0);
    ~ZDiffAreaWidget();
    void setDiffList(QList< QList<int> > diffLst);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QList< QList<int> > mDiffLst;
    ZTextWidget *mTextWidget;
};

class ZTextWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    ZTextWidget(QWidget *parent = 0);
    ~ZTextWidget();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    bool isBlockContained(QList<int> blockNoLst);
    QRect blockArea(QList<int> blockNoLst);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

private:
    void initData();
    void initUI();
    void initConnect();

private:
    QWidget *mLineNumberArea;
    int mFirstVisibleBlockNo;
    int mLastVisibleBlockNo;
    int mBlockHeight;
};

class ZScrollTextWidget : public QWidget
{
    Q_OBJECT
public:
    ZScrollTextWidget(Qt::Alignment verticalAlignment, QWidget *parent = 0);
    ~ZScrollTextWidget();

    void appendText(const QString &text);
    void setVerticalValue(int value);
    void setHorizontalValue(int value);
    void setDiffList(QList< QList<int> > diffLst);

private:
    void initData();
    void initUI();
    void initConnect();

private slots:
    void setVerticalRange(int min, int max);
    void setHorizontalRange(int min, int max);

private:
    Qt::Alignment mVerticalAlignment;
    ZTextWidget *mTextWidget;
    QScrollBar *mVerticalBar;
    QScrollBar *mHorizontalBar;
    QWidget *mBelowWidget;
    ZDiffAreaWidget *mAboveWidget;
};

class ZFileWidget : public QWidget
{
    Q_OBJECT
public:
    ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent = 0);
    ~ZFileWidget();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();

private:
    ZPathDiffModel mPathDiffModel;
    QList<ZFileDiffModel> mModelLst;
    QList<QString> mSrcLineLst;
    QList<QString> mDstLineLst;
    QList< QList<int> > mSrcDiffLst;
    QList< QList<int> > mDstDiffLst;
    ZScrollTextWidget *mSrcScrollTextWidget;
    ZScrollTextWidget *mDstScrollTextWidget;
};

#endif // ZFILEWIDGET

