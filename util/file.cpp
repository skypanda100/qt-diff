#include "file.h"
#include <QTextStream>

FileUtil::FileUtil(){

}

FileUtil::~FileUtil(){

}

int FileUtil::lines(QFile file){
    int count = -1;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return count;
    }else{
        count = 0;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        count++;
    }
    file.close();

    return count;
}
