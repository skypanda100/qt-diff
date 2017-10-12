#ifndef ZFILE
#define ZFILE

#include <QFile>
#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QIcon>

class ZFile{
public:
    ZFile();
    ~ZFile();

    static int lines(QFile *file, QList<QString> &lineLst);

    static QFileInfoList files(const QString &path);

    static QIcon icon(const QString &path);
};

#endif // ZFILE

