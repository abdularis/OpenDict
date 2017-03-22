#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T15:36:26
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenDict
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    odict/dictionary.cpp \
    odict/editdistance.cpp \
    odict/library.cpp \
    aboutdialog.cpp \
    dictinfo.cpp \
    dictlistmanagerdialog.cpp \
    odict/dictlistmodel.cpp

HEADERS  += mainwindow.h \
    odict/dictionary.h \
    odict/editdistance.h \
    odict/library.h \
    aboutdialog.h \
    common.h \
    dictinfo.h \
    dictlistmanagerdialog.h \
    odict/dictlistmodel.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    dictlistmanagerdialog.ui \
    dictinfodialog.ui

RESOURCES += \
    res.qrc

OTHER_FILES += \
    db/dlist.db \
    db/eng-ind__ind-eng.db
