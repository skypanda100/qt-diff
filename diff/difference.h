#ifndef DIFFERENCE
#define DIFFERENCE

#include <QString>
#include <QFile>
#include "model.h"

class Difference {

public:
    Difference();
    Difference(const QString &file_src, const QString &file_dst);
    ~Difference();

    void setFileSrc(const QString &file_src);
    void setFileDst(const QString &file_dst);

    void execute();

private:
    int lines(QFile *file);

    bool initRect();
    bool makeRect();
    void recallRect();

private:
    QFile *m_file_src;
    QFile *m_file_dst;

    short **m_diff_rect;
    int m_diff_rect_rows;
    int m_diff_rect_cols;

    QList<QString> m_line_src_lst;
    QList<QString> m_line_dst_lst;
    QList<Model> m_model_lst;
};

#endif // DIFFERENCE

