QT += core sql
QT -= gui

TARGET = AccountCheckingTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp\
            UserACcount.cpp \
    UserAccount.cpp

HEADERS += UserAccount.hpp

