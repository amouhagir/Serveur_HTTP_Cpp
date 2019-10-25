# -------------------------------------------------
# Project created by QtCreator 2009-01-10T14:03:23
# -------------------------------------------------
QT += network widgets
QT += widgets
QT += core
TARGET = MyQServer
TEMPLATE = app

SOURCES += ./src/main.cpp \
    ./src/MySocketServer.cpp \
    ./src/MySocketClient.cpp \
    ./src/dialog.cpp \
    src/stats.cpp \
    src/trait_requete.cpp \
    src/Gest_cache.cpp \
    src/page.cpp

HEADERS += ./src/MySocketServer.h \
    ./src/MySocketClient.h \
    ./src/dialog.h \
    src/stats.h \
    src/trait_requete.h \
    src/Gest_cache.h \
    src/page.h

OTHER_FILES += MyQServer.pro
CONFIG += c++11
