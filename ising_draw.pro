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
    ising_model.cpp \
        mainwindow.cpp \
    paintwidget.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    ising_model.h \
    paintwidget.h \
    worker.h

FORMS    += mainwindow.ui
