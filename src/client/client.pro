QT     += core gui
CONFIG += c++14


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/../../common.pri)

TARGET = client
TEMPLATE = app
UI_DIR = ./ui
DEFINES += __CLIENT__

SOURCES += main.cpp \
            widget.cpp \
            client.cpp \
    stackwidget.cpp \
    userlobby.cpp \
    dialogorder.cpp \
    detailwidget.cpp \
    tablemodel.cpp \
    userswidget.cpp

HEADERS += \
            widget.h \
            client.h \
    ../socket.h \
    stackwidget.h \
    userlobby.h \
    dialogorder.h \
    detailwidget.h \
    tablemodel.h \
    userswidget.h

FORMS += \
        widget.ui \
    stackwidget.ui \
    userlobby.ui \
    dialogorder.ui \
    detailwidget.ui \
    userswidget.ui

