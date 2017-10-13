#include "zhash.h"

ZHash::ZHash()
{

}

ZHash::~ZHash()
{

}

unsigned int ZHash::ELFHash(QString key)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    QByteArray ba = key.toLatin1();
    char *str = ba.data();

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}
