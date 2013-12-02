#-------------------------------------------------
#
# Project created by QtCreator 2013-12-02T13:01:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ecg-neurons
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ecg-annotation/signal.cpp \
    ecg-annotation/fwt.cpp \
    ecg-annotation/ecgdenoise.cpp \
    ecg-annotation/ecgannotation.cpp \
    ecg-annotation/cwt.cpp

HEADERS  += MainWindow.h \
    ecg-annotation/signal.h \
    ecg-annotation/lib.h \
    ecg-annotation/fwt.h \
    ecg-annotation/ecgdenoise.h \
    ecg-annotation/ecgannotation.h \
    ecg-annotation/cwt.h

FORMS    += MainWindow.ui
