QT     += core gui
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app
UI_DIR = ./ui
DEFINES += __CLIENT__

SOURCES += main.cpp \
            widget.cpp \
            client.cpp

HEADERS += \
            widget.h \
            client.h

FORMS += \
        widget.ui

