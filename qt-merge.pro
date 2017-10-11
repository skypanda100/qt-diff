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
    util/zfile.cpp \
    widget/tree/ztreemodel.cpp \
    widget/tree/ztreeitem.cpp \
    diff/zdiffmodel.cpp \
    widget/ztopwidget.cpp

HEADERS  += mainwindow.h \
    diff/zdifference.h \
    util/zfile.h \
    widget/tree/ztreemodel.h \
    widget/tree/ztreeitem.h \
    diff/zdiffmodel.h \
    widget/ztopwidget.h
