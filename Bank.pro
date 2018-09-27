#-------------------------------------------------
#
# Project created by QtCreator 2018-09-07T12:21:16
#
#-------------------------------------------------

TEMPLATE = subdirs
SUBDIRS = \
        src/client \
        src/server \


#client {
#QT     += core gui
#CONFIG += c++14
#}

#server {
#    CONFIG += console c++14
#    CONFIG -= app_bundle
#    CONFIG -= qt
#}


#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = Bank
#TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#debug {
#  DEFINES += __DEBUG__
#}

#SOURCES += \
#        include/sqlite3.c \
#        include/jsoncpp.cpp \

#HEADERS  += \
#        src/define.h \
#    include/sqlite3.h \
#    include/json/json.h \
#    include/json/json-forwards.h \
#    src/model/orderinfo.h \
#    src/model/userinfo.h \

#server{
#    TARGET = server
#    DEFINES += __SERVER__

#    SOURCES += src/server/main.cpp \
#                src/server/server.cpp \

#    HEADERS += src/server/server.h \
#}

#client{
#    TARGET = client
#    UI_DIR = ./ui
#    DEFINES += __CLIENT__

#    SOURCES += src/client/main.cpp \
#                src/client/widget.cpp \
#                src/client/client.cpp

#    HEADERS += \
#                src/client/widget.h \
#                src/client/client.h

#    FORMS += \
#            src/client/widget.ui
#}






