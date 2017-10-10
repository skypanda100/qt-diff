#ifndef MODEL
#define MODEL

#include <QString>

class Model{

public:
    Model();
    ~Model();

    QString line() const;
    void setLine(const QString &line);

    int status() const;
    void setStatus(int status);

    QString srcLine() const;
    void setSrcLine(const QString &src_line);

    QString dstLine() const;
    void setDstLine(const QString &dst_line);

private:
    QString m_src_line;
    QString m_dst_line;
    int m_status; //0:same 1:modify 2:delete 3:add
};

#endif // MODEL

