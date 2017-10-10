#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T09:40:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-merge
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    diff/difference.cpp \
    util/file.cpp \
    diff/model.cpp

HEADERS  += mainwindow.h \
    diff/difference.h \
    util/file.h \
    diff/model.h

FORMS    += mainwindow.ui
