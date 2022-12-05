#-------------------------------------------------
#
# Project created by QtCreator 2020-04-16T12:20:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ising_model
TEMPLATE = app


SOURCES += main.cpp\
    debug.cpp \
    funcs.cpp \
    ising_model.cpp \
    lattice.cpp \
    mainwindow.cpp \
    paintwidget.cpp \
    worker.cpp

HEADERS  += hdr/mainwindow.h \
    hdr/debug.h \
    hdr/funcs.h \
    hdr/ising_model.h \
    hdr/lattice.h \
    hdr/paintwidget.h \
    hdr/worker.h

FORMS    += mainwindow.ui

CONFIG += console

DISTFILES +=
