#include "zscrolltextwidget.h"

ZScrollTextWidget::ZScrollTextWidget(Qt::Alignment verticalAlignment, QWidget *parent)
    : QWidget(parent)
    , mVerticalAlignment(verticalAlignment)
{
    initData();
    initUI();
    initConnect();
}

ZScrollTextWidget::~ZScrollTextWidget()
{
    delete mTextWidget;
    delete mVerticalBar;
    delete mHorizontalBar;
    delete mBelowWidget;
    delete mDiffAreaWidget;
    delete mVerticalWidget;
    delete mHorizontalWidget;
    delete mAboveWidget;
}

void ZScrollTextWidget::appendText(const QString &text)
{
    mTextWidget->appendPlainText(text);
}

void ZScrollTextWidget::setDiffList(QList<ZDiffInfo> diffLst)
{
    mDiffAreaWidget->setDiffList(diffLst);
}

bool ZScrollTextWidget::isBlockContained(ZDiffInfo diffInfo)
{
    return mTextWidget->isBlockContained(diffInfo);
}

QRectF ZScrollTextWidget::blockArea(ZDiffInfo diffInfo)
{
    QRectF rectf = mTextWidget->blockArea(diffInfo);
    QPoint startPoint = QPoint((int)rectf.x(), (int)rectf.y());
    startPoint = mTextWidget->mapToParent(startPoint);
    return QRectF(startPoint, rectf.size());
}

void ZScrollTextWidget::clearText()
{
    mTextWidget->clear();
}

void ZScrollTextWidget::setTextToFile(const QString &path)
{
    QTextDocumentWriter writer(path);
    writer.setFormat("plaintext");
    writer.write(mTextWidget->document());
}

int ZScrollTextWidget::visibleBlockCount()
{
    return mTextWidget->visibleBlockCount();
}

void ZScrollTextWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void ZScrollTextWidget::initData()
{

}

void ZScrollTextWidget::initUI()
{
    this->setContentsMargins(0, 0, 0, 0);

    mTextWidget = new ZTextWidget;
    mTextWidget->setLineWrapMode(QPlainTextEdit::NoWrap);
    mTextWidget->verticalScrollBar()->setTracking(true);
    mTextWidget->horizontalScrollBar()->setTracking(true);
    mTextWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mTextWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mVerticalBar = new QScrollBar(Qt::Vertical, this);
    mVerticalBar->setFixedWidth(SCROLL_BAR_WIDTH);

    mHorizontalBar = new QScrollBar(Qt::Horizontal, this);
    mHorizontalBar->setFixedHeight(SCROLL_BAR_WIDTH);

    QGridLayout *belowGridLayout =new QGridLayout;
    belowGridLayout->setSpacing(0);
    belowGridLayout->setContentsMargins(0, 0, 0, 0);
    if(mVerticalAlignment == Qt::AlignLeft)
    {
        belowGridLayout->addWidget(mVerticalBar, 0, 0);
        belowGridLayout->addWidget(mTextWidget, 0, 1);
        belowGridLayout->addWidget(mHorizontalBar, 1, 1);
    }
    else
    {
        belowGridLayout->addWidget(mTextWidget, 0, 0);
        belowGridLayout->addWidget(mVerticalBar, 0, 1);
        belowGridLayout->addWidget(mHorizontalBar, 1, 0);
    }
    mBelowWidget = new QWidget;
    mBelowWidget->setLayout(belowGridLayout);

    mDiffAreaWidget = new ZDiffAreaWidget(mTextWidget);

    mVerticalWidget = new QWidget;
    mVerticalWidget->setFixedWidth(SCROLL_BAR_WIDTH);

    mHorizontalWidget = new QWidget;
    mHorizontalWidget->setFixedHeight(SCROLL_BAR_WIDTH);

    QGridLayout *aboveGridLayout =new QGridLayout;
    aboveGridLayout->setSpacing(0);
    aboveGridLayout->setContentsMargins(0, 0, 0, 0);
    if(mVerticalAlignment == Qt::AlignLeft)
    {
        aboveGridLayout->addWidget(mVerticalWidget, 0, 0);
        aboveGridLayout->addWidget(mDiffAreaWidget, 0, 1);
        aboveGridLayout->addWidget(mHorizontalWidget, 1, 1);
    }
    else
    {
        aboveGridLayout->addWidget(mDiffAreaWidget, 0, 0);
        aboveGridLayout->addWidget(mVerticalWidget, 0, 1);
        aboveGridLayout->addWidget(mHorizontalWidget, 1, 0);
    }
    mAboveWidget = new QWidget;
    mAboveWidget->setLayout(aboveGridLayout);
    mAboveWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    mAboveWidget->setStyleSheet("background-color:transparent;");

    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->setSpacing(0);
    stackedLayout->setContentsMargins(0, 0, 0, 0);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->addWidget(mBelowWidget);
    stackedLayout->addWidget(mAboveWidget);
    this->setLayout(stackedLayout);
}

void ZScrollTextWidget::initConnect()
{
    connect(mVerticalBar, SIGNAL(valueChanged(int)), this, SLOT(onVerticalValueChanged(int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setVerticalRange(int,int)));

    connect(mHorizontalBar, SIGNAL(valueChanged(int)), this, SLOT(onHorizontablValueChanged(int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setHorizontalRange(int,int)));

    connect(mTextWidget, SIGNAL(scrollValueChanged(int)), this, SLOT(onScrollValueChanged(int)));
    connect(mTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    connect(mTextWidget, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
}

void ZScrollTextWidget::setVerticalRange(int min, int max)
{
    mVerticalBar->setRange(min, max);
}

void ZScrollTextWidget::setHorizontalRange(int min, int max)
{
    mHorizontalBar->setRange(min, max);
}

void ZScrollTextWidget::onVerticalValueChanged(int value)
{
    emit scrollValueChanged(value);
    mVerticalBar->setValue(value);
    mTextWidget->verticalScrollBar()->setValue(value);
    this->parentWidget()->update();
}

void ZScrollTextWidget::onHorizontablValueChanged(int value)
{
    mHorizontalBar->setValue(value);
    mTextWidget->horizontalScrollBar()->setValue(value);
    this->parentWidget()->update();
}

void ZScrollTextWidget::onCursorPositionChanged()
{
    emit mTextWidget->verticalScrollBar()->valueChanged(mTextWidget->verticalScrollBar()->value());
    emit mTextWidget->horizontalScrollBar()->valueChanged(mTextWidget->horizontalScrollBar()->value());
}

void ZScrollTextWidget::onScrollValueChanged(int value)
{
    onVerticalValueChanged(value);
}

void ZScrollTextWidget::onScrollValueChangedWithoutSignal(int value)
{
    mVerticalBar->setValue(value);
    mTextWidget->verticalScrollBar()->setValue(value);
    this->parentWidget()->update();
}
