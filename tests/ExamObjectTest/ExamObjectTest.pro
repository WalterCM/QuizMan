QT += core sql
QT -= gui

TARGET = ExamObjectTest
CONFIG += consoleb c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp\
        src/Exam/Option.cpp\
        src/Exam/QuizManExam.cpp\
        src/Exam/Question.cpp\
        src/Exam/ExamManager.cpp

HEADERS += include/Exam/Option.hpp\
        include/Exam/QuizManExam.hpp\
        include/Exam.hpp\
        include/Exam/Question.hpp\
        include/Exam/ExamManager.hpp

DESTDIR = $$PWD

