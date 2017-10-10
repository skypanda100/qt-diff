#include "zmodel.h"

ZModel::ZModel(){

}

ZModel::~ZModel(){

}

QString ZModel::srcLine() const
{
    return m_src_line;
}

void ZModel::setSrcLine(const QString &src_line)
{
    m_src_line = src_line;
}

QString ZModel::dstLine() const
{
    return m_dst_line;
}

void ZModel::setDstLine(const QString &dst_line)
{
    m_dst_line = dst_line;
}

int ZModel::status() const
{
    return m_status;
}

void ZModel::setStatus(int status)
{
    m_status = status;
}
