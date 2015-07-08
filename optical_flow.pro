#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T15:05:29
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = optical_flow
TEMPLATE = app

include(gui/gui.pri)
include(optical_flow/optical_flow.pri)

SOURCES += \ 
    main.cpp

HEADERS  += 

FORMS    += mainwindow.ui


DISTFILES += \
    optical_flow.pro.user
