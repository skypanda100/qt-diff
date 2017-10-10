#include "difference.h"
#include "util/file.h"
#include <QTextStream>
#include <QtMath>

Difference::Difference()
    : m_diff_rect(NULL)
{

}

Difference::Difference(QFile file_src, QFile file_dst)
    : m_file_src(file_src)
    , m_file_dst(file_dst)
    , m_diff_rect(NULL)
{

}

Difference::~Difference()
{
    if(m_diff_rect != NULL)
    {
        delete m_diff_rect;
        m_diff_rect = NULL;
    }
}

void Difference::setFileSrc(QFile file_src)
{
    m_file_src = file_src;
}

void Difference::setFileDst(QFile file_dst)
{
    m_file_dst = file_dst;
}

void Difference::execute()
{
    if(initRect())
    {
        makeRect();
    }
}

bool Difference::initRect()
{
    m_diff_rect_rows = FileUtil::lines(m_file_src);
    m_diff_rect_cols = FileUtil::lines(m_file_dst);

    if(m_diff_rect_rows == -1 || m_diff_rect_cols == -1)
    {
        return false;
    }
    else
    {
        m_diff_rect_rows += 1;
        m_diff_rect_cols += 1;
    }

    m_diff_rect = new short[m_diff_rect_rows][m_diff_rect_cols];
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

    if(!m_file_src.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    if(!m_file_dst.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in_src(&m_file_src);
    QTextStream in_dst(&m_file_dst);

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

    in_src.close();
    in_dst.close();
}

bool Difference::recallRect()
{
    int row = m_diff_rect_rows;
    int col = m_diff_rect_cols;

    int i = row - 1;
    int j = col - 1;

    while(i >= 0 && j >= 0)
    {
        short top = m_diff_rect[i - 1][j];
        short top_left = m_diff_rect[i - 1][j - 1];
        short left = m_diff_rect[i][j - 1];

        short[] val_arr = {top_left, top, left};
        int[] idx_arr = {0, 1, 2};

        QString line_src = m_line_src_lst[i];
        QString line_dst = m_line_dst_lst[j];
        if(QString::compare(line_src, line_dst) == 0)
        {
            i -= 1;
            j -= 1;
        }
        else
        {
            for(int k = 0;k < sizeof(val_arr) / sizeof(short) - 1;k++)
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

            int idx_last = idx_arr[sizeof(idx_arr) / sizeof(short) - 1];
            switch(idx_last)
            {
                case 0:
                    i -= 1;
                    j -= 1;
                    break;
                case 1:
                    i -= 1;
                    break;
                case 2:
                    j -= 1;
                    break;
            }
        }
    }
}
