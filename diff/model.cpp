#include "model.h"

Model::Model(){

}

Model::~Model(){

}

QString Model::srcLine() const
{
    return m_src_line;
}

void Model::setSrcLine(const QString &src_line)
{
    m_src_line = src_line;
}

QString Model::dstLine() const
{
    return m_dst_line;
}

void Model::setDstLine(const QString &dst_line)
{
    m_dst_line = dst_line;
}

int Model::status() const
{
    return m_status;
}

void Model::setStatus(int status)
{
    m_status = status;
}
