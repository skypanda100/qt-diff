#ifndef ZDIFFERENCE
#define ZDIFFERENCE

#include <QString>
#include <QFile>
#include "zdiffmodel.h"

class ZDifference {

public:
    ZDifference();
    ZDifference(const QString &file_src, const QString &file_dst);
    ~ZDifference();

    void setFileSrc(const QString &file_src);
    void setFileDst(const QString &file_dst);

    void execute();

private:
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
    QList<ZDiffModel> m_model_lst;
};

#endif // ZDIFFERENCE

