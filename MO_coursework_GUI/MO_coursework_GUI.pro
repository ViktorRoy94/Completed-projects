#-------------------------------------------------
#
# Project created by QtCreator 2015-12-12T17:09:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = MO_coursework_GUI
TEMPLATE = app
CONFIG += qwt

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    input_from_txt.h \
    input_generation.h \
    Lemke.h \
    Monte-Carlo.h \
    test.h \
    func.h

FORMS    += mainwindow.ui

DISTFILES += \
    questions.txt
