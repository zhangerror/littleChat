#-------------------------------------------------
#
# Project created by QtCreator 2020-06-16T21:00:33
#
#-------------------------------------------------

QT       += core gui
QT  += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += main.cpp\
        start.cpp \
    chatting.cpp \
    record.cpp

HEADERS  += start.h \
    chatting.h \
    record.h

FORMS    += start.ui \
    chatting.ui \
    record.ui
