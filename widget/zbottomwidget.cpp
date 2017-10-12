#include "zbottomwidget.h"

ZBottomWidget::ZBottomWidget(QWidget *parent)
    :QStatusBar(parent)
{
    initData();
    initUI();
    initConnect();
}

ZBottomWidget::~ZBottomWidget()
{
    delete mProgressBar;
}

void ZBottomWidget::initData()
{

}

void ZBottomWidget::initUI()
{
    mProgressBar = new QProgressBar;
    mProgressBar->setTextVisible(false);
    mProgressBar->setFixedHeight(15);
    mProgressBar->setRange(0, 100);
    mProgressBar->setVisible(false);
    addPermanentWidget(mProgressBar);
}

void ZBottomWidget::initConnect()
{

}

void ZBottomWidget::onProgress(int value, int maxValue)
{
    if(mProgressBar->maximum() != maxValue){
        mProgressBar->setRange(0, maxValue);
    }
    mProgressBar->setValue(value);
    if(value < mProgressBar->maximum()){
        showMessage(tr("Processing......"));
    }else{
        showMessage(tr("Processed"));
    }
    if(value <= 0){
        mProgressBar->setVisible(false);
    }else if(value > 0 && value < mProgressBar->maximum()){
        mProgressBar->setVisible(true);
    }else{
        mProgressBar->setVisible(false);
    }
}
