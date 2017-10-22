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
    delete mAboveWidget;
}

void ZScrollTextWidget::appendText(const QString &text)
{
    mTextWidget->appendPlainText(text);
}

void ZScrollTextWidget::setDiffList(QList<ZDiffInfo> diffLst)
{
    mAboveWidget->setDiffList(diffLst);
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

void ZScrollTextWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void ZScrollTextWidget::initData()
{

}

void ZScrollTextWidget::initUI()
{
    mTextWidget = new ZTextWidget;
    mTextWidget->setLineWrapMode(QPlainTextEdit::NoWrap);
    mTextWidget->verticalScrollBar()->setTracking(true);
    mTextWidget->horizontalScrollBar()->setTracking(true);
    mTextWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mTextWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mVerticalBar = new QScrollBar(Qt::Vertical, this);
    mVerticalBar->setFixedWidth(20);
    mVerticalBar->setVisible(false);

    mHorizontalBar = new QScrollBar(Qt::Horizontal, this);
    mHorizontalBar->setFixedHeight(20);
    mHorizontalBar->setVisible(false);

    QGridLayout *gridLayout =new QGridLayout;
    gridLayout->setSpacing(0);
    if(mVerticalAlignment == Qt::AlignLeft)
    {
        gridLayout->addWidget(mVerticalBar, 0, 0);
        gridLayout->addWidget(mTextWidget, 0, 1);
        gridLayout->addWidget(mHorizontalBar, 1, 1);
    }
    else
    {
        gridLayout->addWidget(mTextWidget, 0, 0);
        gridLayout->addWidget(mVerticalBar, 0, 1);
        gridLayout->addWidget(mHorizontalBar, 1, 0);
    }
    mBelowWidget = new QWidget;
    mBelowWidget->setLayout(gridLayout);

    mAboveWidget = new ZDiffAreaWidget(mTextWidget);
    mAboveWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    stackedLayout->addWidget(mBelowWidget);
    stackedLayout->addWidget(mAboveWidget);
    this->setLayout(stackedLayout);
}

void ZScrollTextWidget::initConnect()
{
    connect(mVerticalBar, SIGNAL(valueChanged(int)), mTextWidget->verticalScrollBar(), SLOT(setValue(int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setVerticalRange(int,int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(onVerticalValueChange(int)));
    connect(mTextWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onVerticalValueChange(int)));

    connect(mHorizontalBar, SIGNAL(valueChanged(int)), mTextWidget->horizontalScrollBar(), SLOT(setValue(int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(setHorizontalRange(int,int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(sliderMoved(int)), this, SLOT(onHorizontablValueChanged(int)));
    connect(mTextWidget->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onHorizontablValueChanged(int)));

    connect(mTextWidget, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
}

void ZScrollTextWidget::setVerticalRange(int min, int max)
{
    if(!mVerticalBar->isVisible())
    {
        mVerticalBar->setVisible(true);
    }
    mVerticalBar->setValue(max);
    mVerticalBar->setRange(min, max);
}

void ZScrollTextWidget::setHorizontalRange(int min, int max)
{
    if(!mHorizontalBar->isVisible())
    {
        mHorizontalBar->setVisible(true);
    }
    mHorizontalBar->setValue(max);
    mHorizontalBar->setRange(min, max);
}

void ZScrollTextWidget::onVerticalValueChange(int value)
{
    emit scrollValueChange(value);
    mVerticalBar->setValue(value);
    this->parentWidget()->update();
}

void ZScrollTextWidget::onHorizontablValueChanged(int value)
{
    mHorizontalBar->setValue(value);
    this->parentWidget()->update();
}

void ZScrollTextWidget::onCursorPositionChanged()
{
    emit mTextWidget->verticalScrollBar()->valueChanged(mTextWidget->verticalScrollBar()->value());
    emit mTextWidget->horizontalScrollBar()->valueChanged(mTextWidget->horizontalScrollBar()->value());
}

void ZScrollTextWidget::onScrollValueChange(int value)
{
    mVerticalBar->setValue(value);
//    this->parentWidget()->update();
}
