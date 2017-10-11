#ifndef ZFILE
#define ZFILE

#include <QFile>
#include <QList>

class ZFile{
public:
    ZFile();
    ~ZFile();

    static int lines(QFile *file, QList<QString> &lineLst);
};

#endif // ZFILE

