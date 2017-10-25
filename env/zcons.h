#ifndef CONS
#define CONS

#include <QString>
#include <QColor>

enum Status
{
    Same = 0,
    Modified,
    Removed,
    Added
};

const QString STATUS_STR[4] =
{
    "Same",
    "Modified",
    "Removed",
    "Added"
};

const QColor STATUS_CLR[4] =
{
    QColor(0, 0, 0),
    QColor(52, 135, 234),
    QColor(204, 55, 29),
    QColor(25, 122, 31)
};

const int STATUS_CLR_ALPHA = 100;

const QString OBJECT_FOLDER_COMPARISON = "folder";
const QString OBJECT_FILE_COMPARISON = "file";

const QColor LINE_NUMBER_AREA = QColor(230, 230, 230);

const int SCROLL_BAR_WIDTH = 20;

const int OVERVIEW_DIFF_AREA_WIDTH = 16;

const int OVERVIEW_DIFF_AREA_MARGIN = 4;

const QColor OVERVIEW_DIFF_AREA_CLR = QColor(200, 0, 0, 150);

const int SHOW_DIFF_DELAY = 1000;
#endif // CONS

