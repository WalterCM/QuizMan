x#-------------------------------------------------
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
        src/MainWindow.cpp \
        src/Exam/QuizManExam.cpp \
        src/Exam/Question.cpp \
        src/Exam/Option.cpp \
        src/Exam/ExamManager.cpp \
        src/RegistryDialog.cpp

HEADERS  += include/IntroWindow.hpp \
        include/Account.hpp \
        include/Account/UserAccount.hpp \
        include/Account/AccountManager.hpp \
        include/ProgramConstants.hpp \
        include/MainWindow.hpp \
        include/Exam/QuizManExam.hpp \
        include/Exam/Question.hpp \
        include/Exam/Option.hpp \
        include/Exam/ExamManager.hpp \
        include/Exam.hpp \
        include/RegistryDialog.hpp

FORMS    += resources/ui/IntroWindow.ui \
            resources/ui/MainWindow.ui \
            resources/ui/RegistryDialog.ui

DESTDIR = $$PWD

RESOURCES += \
    resources/resources.qrc
