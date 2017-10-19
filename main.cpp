#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    int id = QFontDatabase::addApplicationFont(":/font/times_new_roman.fon");
//    qDebug() << QFontDatabase::applicationFontFamilies(id);
//    QFont font;
//    font.setFamily("Helvetica-Condensed-Black-Se");
//    qApp->setFont(font);

    MainWindow w;
    w.show();

    w.resize(QSize(850, 600));

    return a.exec();
}
