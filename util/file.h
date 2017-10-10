#ifndef FILE
#define FILE

#include <QFile>

class FileUtil{
public:
    FileUtil();
    ~FileUtil();

    static int lines(QFile file);
};

#endif // FILE

