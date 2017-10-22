#include "zlinenumberwidget.h"

ZLineNumberWidget::ZLineNumberWidget(ZTextWidget *parent)
    : QWidget(parent)
    , mTextWidget(parent)
{
    initData();
    initUI();
    initConnect();
}

ZLineNumberWidget::~ZLineNumberWidget()
{

}

QSize ZLineNumberWidget::sizeHint() const
{
    return QSize(mTextWidget->lineNumberAreaWidth(), 0);
}

void ZLineNumberWidget::paintEvent(QPaintEvent *event)
{
    mTextWidget->lineNumberAreaPaintEvent(event);
}

void ZLineNumberWidget::initData()
{

}

void ZLineNumberWidget::initUI()
{

}

void ZLineNumberWidget::initConnect()
{

}
