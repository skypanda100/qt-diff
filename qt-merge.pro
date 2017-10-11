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
    diff/zdifference.cpp \
    diff/zmodel.cpp \
    util/zfile.cpp \
    tree/ztreemodel.cpp \
    tree/ztreeitem.cpp

HEADERS  += mainwindow.h \
    diff/zdifference.h \
    diff/zmodel.h \
    util/zfile.h \
    tree/ztreemodel.h \
    tree/ztreeitem.h
