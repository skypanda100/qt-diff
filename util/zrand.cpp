#include "zrand.h"
#include <QMutex>
#include <QTime>

static uint seed = QTime::currentTime().elapsed();
static QMutex mutex;

ZRand::ZRand()
{

}

ZRand::~ZRand()
{

}

QString ZRand::randString()
{
    QMutexLocker locker(&mutex);
    int max = 8;
    QString tmp = QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWZYZ");
    QString str;
    qsrand(seed++);
    for(int i = 0;i < max;i++)
    {
        int ir = qrand() % tmp.length();
        str[i] = tmp.at(ir);
    }

    return str;
}
