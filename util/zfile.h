#ifndef ZFILE
#define ZFILE

#include <QFile>

class ZFile{
public:
    ZFile();
    ~ZFile();

    static int lines(QFile *file);
};

#endif // ZFILE

