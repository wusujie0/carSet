TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    db.cpp \
    dao.cpp \
    chat_server.cpp

HEADERS += \
    db.h \
    dao.h \
    chat_server.h

LIBS += -lmysqlclient -lboost_system
