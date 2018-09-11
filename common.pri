INCLUDEPATH += $$PWD/include

SOURCES += \
        $$PWD/include/sqlite3.c \
        $$PWD/include/jsoncpp.cpp \

HEADERS  += \
        $$PWD/src/define.h \
    $$PWD/include/sqlite3.h \
    $$PWD/include/json/json.h \
    $$PWD/include/json/json-forwards.h \
    $$PWD/include/json.hpp \
    $$PWD/src/model/orderinfo.h \
    $$PWD/src/model/userinfo.h \
    $$PWD/src/socket.h \
