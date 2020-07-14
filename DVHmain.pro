#-------------------------------------------------
#
# Project created by QtCreator 2017-11-10T21:26:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DVHmain
TEMPLATE = app

CONFIG += static-runtime

SOURCES +=\
        dvh.cpp \
    dvh1.cpp \
    dvh2.cpp \
    dvh3.cpp \
    Func.cpp \
    roianalyzer.cpp \
    getinputs.cpp \
    predose.cpp \
    main.cpp \
    planform.cpp \
    monaco_main.cpp \
    monaco_dvh2.cpp \
    monaco_dvh3.cpp \
    monaco_getinputs.cpp

HEADERS  += dvh.h \
    dvh1.h \
    dvh2.h \
    dvh3.h \
    Func.h \
    roianalyzer.h \
    getinputs.h \
    predose.h \
    planform.h \
    monaco_main.h \
    monaco_dvh2.h \
    monaco_dvh3.h \
    monaco_getinputs.h

FORMS    += dvh.ui \
    dvh1.ui \
    dvh2.ui \
    dvh3.ui \
    roianalyzer.ui \
    getinputs.ui \
    predose.ui \
    plantype.ui \
    planform.ui \
    monaco_main.ui \
    monaco_dvh2.ui \
    monaco_dvh3.ui \
    monaco_getinputs.ui

DISTFILES +=
