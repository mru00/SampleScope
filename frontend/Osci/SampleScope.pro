#-------------------------------------------------
#
# Project created by QtCreator 2012-04-27T19:41:45
#
#-------------------------------------------------

QT       += core gui

TARGET = SampleScope
TEMPLATE = app

QMAKE_CXXFLAGS += -I/usr/include/qwt/ -DOSCI_GUI
QMAKE_LIBS += -lqwt -lusb-1.0 `pkg-config --libs fftw3`

SOURCES += main.cpp channelcontrol.cpp \
        mainwindow.cpp \
    device.cpp \
    triggercontrol.cpp \
    timecontrol.cpp \
    lmmin.c \
    lm_eval.c \
    modecontrol.cpp \
    graphcontrol.cpp \
    graphbase.cpp \
    graphfft.cpp \
    graphnormal.cpp \
    graphxy.cpp \
    measurementwidget.cpp \
    measurementtablemodel.cpp \
    graphautocorr.cpp \
    calibdialog.cpp \
    abstracthardware.cpp \
    realhardware.cpp \
    dummyhardware.cpp \
    aboutdialog.cpp \
    measurement.cpp \
    hid-libusb.c

HEADERS  += mainwindow.h channelcontrol.h \
    device.h \
    channelcontrol.h \
    triggercontrol.h \
    timecontrol.h \
    lmmin.h \
    lm_eval.h \
    modecontrol.h \
    graphcontrol.h \
    graphbase.h \
    graphfft.h \
    graphnormal.h \
    graphxy.h \
    ../../shared/common.h \
    measurementwidget.h \
    measurementtablemodel.h \
    graphautocorr.h \
    calibdialog.h \
    abstracthardware.h \
    realhardware.h \
    dummyhardware.h \
    aboutdialog.h \
    measurement.h \
    hidapi.h

FORMS    += mainwindow.ui channelcontrol.ui \
    triggercontrol.ui \
    timecontrol.ui \
    modecontrol.ui \
    graphcontrol.ui \
    graphbase.ui \
    measurementwidget.ui \
    calibdialog.ui \
    aboutdialog.ui

RESOURCES += \
    Resources.qrc
