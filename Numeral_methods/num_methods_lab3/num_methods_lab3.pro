QT += core
QT -= gui

TARGET = num_methods_lab3
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    cycle_reduction.cpp

LIBS += -llapack -lblas -larmadillo

HEADERS += \
    cycle_reduction.h

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
