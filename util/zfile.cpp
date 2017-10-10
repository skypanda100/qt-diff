#include "zfile.h"
#include <QTextStream>

ZFile::ZFile()
{

}

ZFile::~ZFile()
{

}

int ZFile::lines(QFile *file){
    int count = -1;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        return count;
    }else{
        count = 0;
    }

    QTextStream in(file);
    while(!in.atEnd()){
        in.readLine();
        count++;
    }
    file->close();

    return count;
}
