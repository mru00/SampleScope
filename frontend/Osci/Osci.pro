#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T19:41:45
#
#-------------------------------------------------

QT       += core gui

TARGET = Osci
TEMPLATE = app


SOURCES += main.cpp channelcontrol.cpp \
        mainwindow.cpp \
    device.cpp \
    triggercontrol.cpp \
    timecontrol.cpp \
    lmmin.c \
    lm_eval.c

HEADERS  += mainwindow.h channelcontrol.h \
    device.h \
    channelcontrol.h \
    triggercontrol.h \
    timecontrol.h \
    lmmin.h \
    lm_eval.h

FORMS    += mainwindow.ui channelcontrol.ui \
    triggercontrol.ui \
    timecontrol.ui
