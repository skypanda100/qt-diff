#include "zfile.h"
#include <QTextStream>

ZFile::ZFile()
{

}

ZFile::~ZFile()
{

}

int ZFile::lines(QFile *file, QList<QString> &line_lst){
    int count = -1;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        return count;
    }else{
        count = 0;
    }

    QTextStream in(file);
    while(!in.atEnd()){
        line_lst.append(in.readLine());
        count++;
    }
    file->close();

    return count;
}
