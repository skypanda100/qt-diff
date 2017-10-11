#include "zdiffmodel.h"

ZDiffModel::ZDiffModel(){

}

ZDiffModel::~ZDiffModel(){

}

QString ZDiffModel::srcLine() const
{
    return m_src_line;
}

void ZDiffModel::setSrcLine(const QString &src_line)
{
    m_src_line = src_line;
}

QString ZDiffModel::dstLine() const
{
    return m_dst_line;
}

void ZDiffModel::setDstLine(const QString &dst_line)
{
    m_dst_line = dst_line;
}

int ZDiffModel::status() const
{
    return m_status;
}

void ZDiffModel::setStatus(int status)
{
    m_status = status;
}
