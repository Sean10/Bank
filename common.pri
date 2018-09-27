INCLUDEPATH += $$PWD/include \
    /usr/local/Cellar/openssl/1.0.2o_1/include

LIBS += -L/usr/local/Cellar/openssl/1.0.2o_1/lib \
    -lcrypto \
    -lssl

SOURCES += \
        $$PWD/include/sqlite3.c \

HEADERS  += \
        $$PWD/src/define.h \
    $$PWD/include/sqlite3.h \
    $$PWD/include/json/json.h \
    $$PWD/include/json/json-forwards.h \
    $$PWD/include/json.hpp \
    $$PWD/include/sole.hpp \
    $$PWD/src/model/orderinfo.h \
    $$PWD/src/model/userinfo.h \
    $$PWD/src/socket.h \
