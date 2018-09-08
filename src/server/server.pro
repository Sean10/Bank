CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

TARGET = server
DEFINES += __SERVER__

SOURCES +=  main.cpp \
             server.cpp \

HEADERS +=  server.h \
