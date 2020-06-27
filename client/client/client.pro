#-------------------------------------------------
#
# Project created by QtCreator 2020-06-18T00:03:31
#
#-------------------------------------------------

QT       += core gui
QT  += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
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
