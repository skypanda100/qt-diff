#include "zfolderwidget.h"
#include "tree/ztreemodel.h"
#include <QMetaType>
#include <QtMath>

ZFolderWidget::ZFolderWidget(QWidget *parent)
    : QWidget(parent)
    , mFolderCtl(NULL)
{
    initData();
    initUI();
    initConnect();
}

ZFolderWidget::~ZFolderWidget()
{
    delete mFileLabel;
    delete mLineLabel;
    delete mPathEditSrc;
    delete mPathEditDst;
    delete mSearchButtonSrc;
    delete mSearchButtonDst;
    delete mTreeView;
    if(mFolderCtl != NULL)
    {
        mFolderCtl->stopRunAndDelete();
    }
}

void ZFolderWidget::compare()
{
    if(mFolderCtl != NULL)
    {
        mFolderCtl->stopRunAndDelete();
    }
    clearAll();

    QString srcBasePath = mPathEditSrc->text().trimmed();
    QString dstBasePath = mPathEditDst->text().trimmed();

    QDir srcBaseDir(srcBasePath);
    QDir dstBaseDir(dstBasePath);

    if(!srcBasePath.isEmpty())
    {
        srcBasePath = srcBaseDir.absolutePath();
    }

    if(!dstBasePath.isEmpty())
    {
        dstBasePath = dstBaseDir.absolutePath();
    }

    ZPathDiff pathDiff(srcBasePath, dstBasePath);
    mPathModelLst = pathDiff.execute();

    mFolderCtl = new ZFolderCtl(srcBasePath, dstBasePath, mPathModelLst);
    connect(mFolderCtl, SIGNAL(diffMessage(const QList<ZTreeItemModel> &)), this, SLOT(onDiffMessage(const QList<ZTreeItemModel> &)), Qt::QueuedConnection);
    connect(mFolderCtl, SIGNAL(diffEnd()), this, SLOT(onDiffEnd()), Qt::QueuedConnection);
    connect(mFolderCtl, SIGNAL(progress(int,int)), this, SLOT(onProgress(int,int)), Qt::QueuedConnection);

    mFolderCtl->start();
}

void ZFolderWidget::stopCompare()
{
    if(mFolderCtl != NULL)
    {
        mFolderCtl->stopRunAndDelete();
    }
}

void ZFolderWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    float progress = (float)((mValue * 1.0f) / (mMaxValue * 1.0f));
    if(progress < 1.0f)
    {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(3, 102, 214)));

        int width = qCeil(this->width() * progress);
        int height = 5;

        painter.drawRect(0, 0, width, height);
    }
}

void ZFolderWidget::initData()
{
    qRegisterMetaType< QList<ZTreeItemModel> >("QList<ZTreeItemModel>");

    mHeader << "#"
            << "Path"
            << "Extension"
            << "Status"
            << "Lines added"
            << "Lines removed"
            << "Lines modified";

    mValue = 1;
    mMaxValue = 1;

    mFileInfo = "%1 files added,%2 files removed,%3 files modified,%4 files total";
    mLineInfo = "%1 lines added,%2 lines removed,%3 lines modified,%4 lines total";

    mFileAdded = 0;
    mFileRemoved = 0;
    mFileModified = 0;
    mLineAdded = 0;
    mLineRemoved = 0;
    mLineModified = 0;
}

void ZFolderWidget::initUI()
{
    mPathEditSrc = new QLineEdit;
    mPathEditSrc->setPlaceholderText("Folder");
    mSearchButtonSrc = new QPushButton;
    mSearchButtonSrc->setIcon(QIcon(":/icon/search.png"));
    mSearchButtonSrc->setToolTip("search");

    mPathEditDst = new QLineEdit;
    mPathEditDst->setPlaceholderText("Folder");
    mSearchButtonDst = new QPushButton;
    mSearchButtonDst->setIcon(QIcon(":/icon/search.png"));
    mSearchButtonDst->setToolTip("search");

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(mPathEditSrc, 1);
    searchLayout->addWidget(mSearchButtonSrc);
    searchLayout->addSpacing(10);
    searchLayout->addWidget(mPathEditDst, 1);
    searchLayout->addWidget(mSearchButtonDst);

    mTreeView = new QTreeView;
    ZTreeModel *model = new ZTreeModel(mHeader, "");
    mTreeView->setModel(model);
    mTreeView->setAlternatingRowColors(true);

    mFileLabel = new QLabel;
    mFileLabel->setText(QString(mFileInfo)
                        .arg(mFileAdded)
                        .arg(mFileRemoved)
                        .arg(mFileModified)
                        .arg(mFileAdded + mFileRemoved + mFileModified));
    mLineLabel = new QLabel;
    mLineLabel->setText(QString(mLineInfo)
                        .arg(mLineAdded)
                        .arg(mLineRemoved)
                        .arg(mLineModified)
                        .arg(mLineAdded + mLineRemoved + mLineModified));

    QVBoxLayout *folderLayout = new QVBoxLayout;
    folderLayout->addLayout(searchLayout);
    folderLayout->addWidget(mTreeView, 1);
    folderLayout->addWidget(mFileLabel, 0, Qt::AlignRight);
    folderLayout->addWidget(mLineLabel, 0, Qt::AlignRight);

    setLayout(folderLayout);
}

void ZFolderWidget::initConnect()
{
    connect(mSearchButtonSrc, SIGNAL(clicked()), this, SLOT(searchClicked()));
    connect(mSearchButtonDst, SIGNAL(clicked()), this, SLOT(searchClicked()));
}

void ZFolderWidget::clearAll()
{
    QAbstractItemModel *model = mTreeView->model();
    if(model->rowCount() > 0)
    {
        model->removeRows(0, model->rowCount());
    }

    mFileAdded = 0;
    mFileRemoved = 0;
    mFileModified = 0;
    mLineAdded = 0;
    mLineRemoved = 0;
    mLineModified = 0;

    updateInfo();
}

void ZFolderWidget::insert(const QList<ZTreeItemModel> &itemModelList)
{
    QModelIndex index = mTreeView->model()->index(itemModelList[0].value().toInt() - 2, 0);
    QAbstractItemModel *model = mTreeView->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
    {
        return;
    }

    int itemCount = itemModelList.size();
    for(int i = 0;i < itemCount;i++)
    {
        QModelIndex child = model->index(index.row() + 1, i, index.parent());
        model->setData(child, QVariant::fromValue(itemModelList[i]), Qt::DisplayRole);
    }

    QString status = itemModelList[3].value().toString();
    int i = 0;
    for(;i < (int)(sizeof(STATUS_STR) / sizeof(QString));i++)
    {
        if(QString::compare(status, STATUS_STR[i]) == 0)
        {
            break;
        }
    }
    switch((Status)i)
    {
    case Same:
        break;
    case Modified:
        mFileModified += 1;
        break;
    case Removed:
        mFileRemoved += 1;
        break;
    case Added:
        mFileAdded += 1;
        break;
    }
    mLineAdded += itemModelList[4].value().toInt();
    mLineRemoved += itemModelList[5].value().toInt();
    mLineModified += itemModelList[6].value().toInt();

    updateInfo();
}

void ZFolderWidget::updateInfo()
{
    mFileLabel->setText(QString(mFileInfo)
                        .arg(mFileAdded)
                        .arg(mFileRemoved)
                        .arg(mFileModified)
                        .arg(mFileAdded + mFileRemoved + mFileModified));

    mLineLabel->setText(QString(mLineInfo)
                        .arg(mLineAdded)
                        .arg(mLineRemoved)
                        .arg(mLineModified)
                        .arg(mLineAdded + mLineRemoved + mLineModified));
}

void ZFolderWidget::searchClicked()
{
    QObject *object = this->sender();
    if(object == mSearchButtonSrc)
    {
        QString currentDir = mPathEditSrc->text().isEmpty() ? QDir::currentPath() : mPathEditSrc->text();
        QString directory = QFileDialog::getExistingDirectory(this,
                                   tr("search"), currentDir);
        if(!directory.isEmpty())
        {
            mPathEditSrc->setText(directory);
        }
    }
    else
    {
        QString currentDir = mPathEditDst->text().isEmpty() ? QDir::currentPath() : mPathEditDst->text();
        QString directory = QFileDialog::getExistingDirectory(this,
                                   tr("search"), currentDir);
        if(!directory.isEmpty())
        {
            mPathEditDst->setText(directory);
        }
    }
}

void ZFolderWidget::onDiffMessage(const QList<ZTreeItemModel> &itemModelList)
{
    insert(itemModelList);
}

void ZFolderWidget::onDiffEnd()
{
    QAbstractItemModel *model = mTreeView->model();
    for(int column = 0; column < model->columnCount(); ++column)
    {
        mTreeView->resizeColumnToContents(column);
    }

    if(mFolderCtl != NULL)
    {
        delete mFolderCtl;
        mFolderCtl = NULL;
    }
}

void ZFolderWidget::onProgress(int value, int maxValue)
{
    if(maxValue == 0)
    {
        mValue = 0;
        mMaxValue = 1;
    }
    else if(value > maxValue)
    {
        mValue = maxValue;
        mMaxValue = maxValue;
    }
    else
    {
        mValue = value;
        mMaxValue = maxValue;
    }
    update();
}
