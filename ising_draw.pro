#-------------------------------------------------
#
# Project created by QtCreator 2014-11-12T16:36:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drawing
TEMPLATE = app


SOURCES += main.cpp\
    debug.cpp \
    funcs.cpp \
    ising_model.cpp \
    lattice.cpp \
        mainwindow.cpp \
    paintwidget.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    debug.h \
    funcs.h \
    ising_model.h \
    lattice.h \
    paintwidget.h \
    worker.h

FORMS    += mainwindow.ui
