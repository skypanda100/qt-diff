#ifndef DIFFERENCE
#define DIFFERENCE

#include <QString>
#include <QFile>
#include "model.h"

class Difference {

public:
    Difference();
    Difference(QFile file_src, QFile file_dst);
    ~Difference();

    void setFileSrc(QFile file_src);
    void setFileDst(QFile file_dst);

    void execute();

private:
    bool initRect();
    bool makeRect();
    bool recallRect();

private:
    QFile m_file_src;
    QFile m_file_dst;

    short **m_diff_rect;
    int m_diff_rect_rows;
    int m_diff_rect_cols;

    QList<const QString &> m_line_src_lst;
    QList<const QString &> m_line_dst_lst;
    QList<Model &> m_model_lst;
};

#endif // DIFFERENCE

