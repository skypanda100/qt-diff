#include "difference.h"
#include <QDebug>
#include <QtMath>
#include <QTextStream>

Difference::Difference()
{
    m_diff_rect = NULL;
    m_file_src = NULL;
    m_file_dst = NULL;
}

Difference::Difference(const QString &file_src, const QString &file_dst)
{
    m_diff_rect = NULL;
    m_file_src = new QFile(file_src);
    m_file_dst = new QFile(file_dst);
}

Difference::~Difference()
{
    if(m_diff_rect != NULL)
    {
        delete[] m_diff_rect;
        m_diff_rect = NULL;
    }

    if(m_file_src != NULL)
    {
        delete m_file_src;
        m_file_src = NULL;
    }

    if(m_file_dst != NULL)
    {
        delete m_file_dst;
        m_file_dst = NULL;
    }
}

void Difference::setFileSrc(const QString &file_src)
{
    m_file_src = new QFile(file_src);
}

void Difference::setFileDst(const QString &file_dst)
{
    m_file_dst = new QFile(file_dst);
}

void Difference::execute()
{
    if(initRect())
    {
        if(makeRect())
        {
            recallRect();
            for(int i = 0;i < m_model_lst.size();i++)
            {
                Model model = m_model_lst[i];
                qDebug() << model.srcLine() << model.dstLine() << model.status();
            }
        }
    }
}

int Difference::lines(QFile *file)
{
    int count = -1;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return count;
    }
    else
    {
        count = 0;
    }

    QTextStream in(file);
    while(!in.atEnd())
    {
        count++;
    }
    file->close();

    return count;
}

bool Difference::initRect()
{
    m_diff_rect_rows = lines(m_file_src);
    m_diff_rect_cols = lines(m_file_dst);

    if(m_diff_rect_rows == -1 || m_diff_rect_cols == -1)
    {
        return false;
    }
    else
    {
        m_diff_rect_rows += 1;
        m_diff_rect_cols += 1;
    }

    m_diff_rect = new short*[m_diff_rect_rows];
    for(int i = 0;i < m_diff_rect_rows;i++)
    {
        m_diff_rect[i] = new short[m_diff_rect_cols];
    }

    memset(m_diff_rect, 0, m_diff_rect_rows * m_diff_rect_cols);
    for(int i = 1;i < m_diff_rect_rows;i++)
    {
        m_diff_rect[i][0] = i;
    }

    for(int i = 0;i < m_diff_rect_cols;i++)
    {
        m_diff_rect[0][i] = i;
    }

    return true;
}

bool Difference::makeRect()
{
    int diff_rect_row = 1;
    int diff_rect_col = 1;

    if(!m_file_src->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    if(!m_file_dst->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in_src(m_file_src);
    QTextStream in_dst(m_file_dst);

    while(!in_src.atEnd())
    {
        short top = m_diff_rect[diff_rect_row - 1][diff_rect_col];
        short top_left = m_diff_rect[diff_rect_row - 1][diff_rect_col - 1];
        short left = m_diff_rect[diff_rect_row][diff_rect_col - 1];

        QString line_src = in_src.readLine();
        QString line_dst = NULL;
        if(!in_dst.atEnd())
        {
            line_dst = in_dst.readLine();
            if(QString::compare(line_src, line_dst) == 0)
            {
                m_diff_rect[diff_rect_row][diff_rect_col] = top_left;
            }
            else
            {
                m_diff_rect[diff_rect_row][diff_rect_col] = qMin(top, qMin(top_left, left)) + 1;
            }

            diff_rect_col++;
            m_line_dst_lst.append(line_dst);
        }
        else
        {
            m_diff_rect[diff_rect_row][diff_rect_col] = qMin(top, qMin(top_left, left)) + 1;
        }

        diff_rect_row++;
        m_line_src_lst.append(line_src);
    }

    while(!in_dst.atEnd())
    {
        short top = m_diff_rect[diff_rect_row - 1][diff_rect_col];
        short top_left = m_diff_rect[diff_rect_row - 1][diff_rect_col - 1];
        short left = m_diff_rect[diff_rect_row][diff_rect_col - 1];

        QString line_dst = in_dst.readLine();

        m_diff_rect[diff_rect_row][diff_rect_col] = qMin(top, qMin(top_left, left)) + 1;

        diff_rect_col++;
        m_line_dst_lst.append(line_dst);
    }

    m_file_src->close();
    m_file_dst->close();

    return true;
}

void Difference::recallRect()
{
    int row = m_diff_rect_rows;
    int col = m_diff_rect_cols;

    int i = row - 1;
    int j = col - 1;

    while(i >= 0 && j >= 0)
    {
        QString line_src = NULL;
        QString line_dst = NULL;
        if(i - 1 >= 0)
        {
            line_src = m_line_src_lst[i - 1];
        }
        if(j - 1 >= 0)
        {
            line_dst = m_line_dst_lst[j - 1];
        }

        short top = 0;
        short top_left = 0;
        short left = 0;

        short val_arr[3] = {top_left, top, left};
        int idx_arr[3] = {0, 1, 2};

        if(i == 0 && j > 0)
        {
            val_arr[0] = 1;
            val_arr[1] = 1;
            val_arr[2] = 0;
        }
        else if(i > 0 && j == 0)
        {
            val_arr[0] = 1;
            val_arr[1] = 0;
            val_arr[2] = 1;
        }
        else
        {
            if(QString::compare(line_src, line_dst) == 0)
            {
                val_arr[0] = 0;
                val_arr[1] = 1;
                val_arr[2] = 1;
            }
            else
            {
                val_arr[0] = m_diff_rect[i - 1][j - 1];
                val_arr[1] = m_diff_rect[i - 1][j];
                val_arr[2] = m_diff_rect[i][j - 1];
            }
        }

        for(int k = 0;k < (int)(sizeof(val_arr) / sizeof(short) - 1);k++)
        {
            short val_1 = val_arr[k];
            short val_2 = val_arr[k + 1];

            int idx_1 = idx_arr[k];
            int idx_2 = idx_arr[k + 1];

            if(val_1 <= val_2)
            {
                short val_tmp = val_1;
                val_arr[k] = val_2;
                val_arr[k + 1] = val_tmp;

                int idx_tmp = idx_1;
                idx_arr[k] = idx_2;
                idx_arr[k + 1] = idx_tmp;
            }
        }

        Model model;
        int idx_last = idx_arr[sizeof(idx_arr) / sizeof(int) - 1];
        switch(idx_last)
        {
            case 0:
                model.setSrcLine(line_src);
                model.setDstLine(line_dst);
                if(QString::compare(line_src, line_dst) == 0)
                {
                    model.setStatus(0);
                }
                else
                {
                    model.setStatus(1);
                }
                i -= 1;
                j -= 1;
                break;
            case 1:
                model.setSrcLine(line_src);
                model.setDstLine(NULL);
                model.setStatus(2);
                i -= 1;
                break;
            case 2:
                model.setSrcLine(NULL);
                model.setDstLine(line_dst);
                model.setStatus(3);
                j -= 1;
                break;
        }

        m_model_lst.append(model);
    }
}

