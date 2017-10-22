#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T09:40:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-diff
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    util/zfile.cpp \
    widget/tree/ztreemodel.cpp \
    widget/tree/ztreeitem.cpp \
    widget/ztopwidget.cpp \
    widget/zcenterwidget.cpp \
    diff/zfilediff.cpp \
    diff/zfilediffmodel.cpp \
    diff/zpathdiffmodel.cpp \
    diff/zpathdiff.cpp \
    ctl/zfolderctl.cpp \
    widget/tree/ztreeitemmodel.cpp \
    widget/zbottomwidget.cpp \
    util/zhash.cpp \
    widget/folder/zfolderwidget.cpp \
    widget/file/zfilewidget.cpp \
    widget/file/zdiffinfo.cpp \
    widget/file/zlinenumberwidget.cpp \
    widget/file/zdiffareawidget.cpp \
    widget/file/ztextwidget.cpp \
    widget/file/zscrolltextwidget.cpp

HEADERS  += mainwindow.h \
    util/zfile.h \
    widget/tree/ztreemodel.h \
    widget/tree/ztreeitem.h \
    widget/ztopwidget.h \
    widget/zcenterwidget.h \
    diff/zfilediffmodel.h \
    diff/zfilediff.h \
    diff/zpathdiffmodel.h \
    diff/zpathdiff.h \
    ctl/zfolderctl.h \
    widget/tree/ztreeitemmodel.h \
    widget/zbottomwidget.h \
    env/zcons.h \
    util/zhash.h \
    widget/folder/zfolderwidget.h \
    widget/file/zfilewidget.h \
    widget/file/zdiffinfo.h \
    widget/file/zlinenumberwidget.h \
    widget/file/zdiffareawidget.h \
    widget/file/ztextwidget.h \
    widget/file/zscrolltextwidget.h

RESOURCES += \
    resource.qrc
