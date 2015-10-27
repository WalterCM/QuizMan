#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T13:51:23
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuizMan
TEMPLATE = app

CONFIG += c++11

SOURCES += src/Main.cpp\
        src/IntroWindow.cpp \
        src/Account/UserAccount.cpp \
        src/Account/AccountManager.cpp \
        src/MainWindow.cpp

HEADERS  += include/IntroWindow.hpp \
        include/Account.hpp \
        include/Account/UserAccount.hpp \
        include/Account/AccountManager.hpp \
        include/ProgramConstants.hpp \
        include/MainWindow.hpp

FORMS    += resources/ui/IntroWindow.ui \
            resources/ui/MainWindow.ui

DESTDIR = $$PWD
