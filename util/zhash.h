#ifndef ZHASH
#define ZHASH

#include <QString>

class ZHash{
public:
    ZHash();
    ~ZHash();

    static unsigned int ELFHash(QString key);
};

#endif // ZHASH

