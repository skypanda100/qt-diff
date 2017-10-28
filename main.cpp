#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int id = QFontDatabase::addApplicationFont(":/font/Helvetica/Helvetica.ttf");
    qDebug() << QFontDatabase::applicationFontFamilies(id);
    id = QFontDatabase::addApplicationFont(":/font/Helvetica/Helvetica-Light.ttf");
    qDebug() << QFontDatabase::applicationFontFamilies(id);
    id = QFontDatabase::addApplicationFont(":/font/Helvetica/Helvetica-Bold.ttf");
    qDebug() << QFontDatabase::applicationFontFamilies(id);

    QFont font;
    font.setFamily("Helvetica");
    qApp->setFont(font);

    QFile qss(":/qdarkstyle/style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    MainWindow w;
    w.show();

    w.resize(QSize(850, 600));
    w.setFocus();

    return a.exec();
}
