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

IRC_MODULES = IrcCore IrcModel IrcUtil
IRC_BUILDDIR = $$OUT_PWD
include(libcommuni/src/module_deps.pri)

SOURCES += main.cpp\
    MainWindow.cpp \
    ChatBot.cpp \
    TwitchBot.cpp

HEADERS  += MainWindow.h \
    ChatBot.h \
    TwitchBot.h \

FORMS    += MainWindow.ui
