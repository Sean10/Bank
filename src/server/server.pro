CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/../../common.pri)

TARGET = server
DEFINES += __SERVER__

SOURCES +=  main.cpp \
             server.cpp \
    dispatcher.cpp \


HEADERS +=  server.h \
    dispatcher.h \
    ormlite.h \
    nullable.h \
