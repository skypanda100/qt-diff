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
    QColor(43, 84, 208),
    QColor(126, 0, 0),
    QColor(0, 128, 0)
};

const QString OBJECT_FOLDER_COMPARISON = "folder";
const QString OBJECT_FILE_COMPARISON = "file";

const QColor LINE_NUMBER_AREA = QColor(230, 230, 230);
#endif // CONS

