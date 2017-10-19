#ifndef ZFILEWIDGET
#define ZFILEWIDGET

#include <QtWidgets>

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

class ZFileWidget : public QWidget
{
    Q_OBJECT
public:
    ZFileWidget(QWidget *parent = 0);
    ~ZFileWidget();

private:
    void initData();
    void initUI();
    void initConnect();

private:
    ZTextWidget *mSrcTextWidget;
    ZTextWidget *mDstTextWidget;
};

#endif // ZFILEWIDGET

