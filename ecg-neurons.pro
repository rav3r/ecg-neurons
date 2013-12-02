#-------------------------------------------------
#
# Project created by QtCreator 2013-12-02T13:01:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ecg-neurons
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ecg-annotation/signal.cpp \
    ecg-annotation/fwt.cpp \
    ecg-annotation/ecgdenoise.cpp \
    ecg-annotation/ecgannotation.cpp \
    ecg-annotation/cwt.cpp \
    qcustomplot/qcustomplot.cpp \
    fann/floatfann.c \
    fann/fann_train_data.c \
    fann/fann_train.c \
    fann/fann_io.c \
    fann/fann_error.c \
    fann/fann_cascade.c \
    fann/fann.c

HEADERS  += MainWindow.h \
    ecg-annotation/signal.h \
    ecg-annotation/lib.h \
    ecg-annotation/fwt.h \
    ecg-annotation/ecgdenoise.h \
    ecg-annotation/ecgannotation.h \
    ecg-annotation/cwt.h \
    qcustomplot/qcustomplot.h \
    fann/floatfann.h \
    fann/fann_train.h \
    fann/fann_io.h \
    fann/fann_internal.h \
    fann/fann_error.h \
    fann/fann_data.h \
    fann/fann_cpp.h \
    fann/fann_cascade.h \
    fann/fann_activation.h \
    fann/fann.h \
    fann/config.h \
    fann/compat_time.h

FORMS    += MainWindow.ui
