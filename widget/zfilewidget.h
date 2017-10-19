#ifndef ZFILEWIDGET
#define ZFILEWIDGET

#include <QtWidgets>
#include "diff/zfilediff.h"
#include "diff/zfilediffmodel.h"
#include "diff/zpathdiff.h"
#include "diff/zpathdiffmodel.h"

class ZLineNumberWidget;
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

class ZTextWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    ZTextWidget(QWidget *parent = 0);
    ~ZTextWidget();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

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
    QWidget *lineNumberArea;
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
};

class ZFileWidget : public QWidget
{
    Q_OBJECT
public:
    ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent = 0);
    ~ZFileWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    ZPathDiffModel mPathDiffModel;
    QList<ZFileDiffModel> mModelLst;
    QList<QString> mSrcLineLst;
    QList<QString> mDstLineLst;
    ZScrollTextWidget *mSrcScrollTextWidget;
    ZScrollTextWidget *mDstScrollTextWidget;
};

#endif // ZFILEWIDGET

