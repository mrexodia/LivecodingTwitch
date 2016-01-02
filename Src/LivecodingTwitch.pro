#-------------------------------------------------
#
# Project created by QtCreator 2016-01-02T00:26:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LivecodingTwitch
TEMPLATE = app
DESTDIR = $$OUT_PWD/bin

# libcommuni
IRC_MODULES = IrcCore IrcModel IrcUtil
IRC_BUILDDIR = $$OUT_PWD
include(libcommuni/src/module_deps.pri)

# qxmpp
include(qxmpp/qxmpp.pri)
INCLUDEPATH += $$QXMPP_INCLUDEPATH
LIBS += $$QXMPP_LIBS
QMAKE_LIBDIR += $$OUT_PWD/src

SOURCES += main.cpp\
    MainWindow.cpp \
    ChatBot.cpp \
    TwitchBot.cpp \
    LivecodingBot.cpp \
    BotSyncer.cpp \
    MainWindowCloseThread.cpp

HEADERS  += MainWindow.h \
    ChatBot.h \
    TwitchBot.h \
    LivecodingBot.h \
    BotSyncer.h \
    MainWindowCloseThread.h

FORMS    += MainWindow.ui

RESOURCES += \
    resources.qrc
