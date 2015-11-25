cmake_minimum_required(VERSION 3.3)
project(QuizMan)

# C++ Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Sql REQUIRED)

#include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(QuizMan_SOURCES
        src/Main.cpp
        src/IntroWindow.cpp
        src/Account/UserAccount.cpp
        src/Account/AccountManager.cpp
        src/MainWindow.cpp
        src/Exam/QuizManExam.cpp
        src/Exam/Question.cpp
        src/Exam/Option.cpp
        src/Exam/ExamManager.cpp
        src/QuestionForm.cpp
        src/QuestionEditor.cpp
        src/DroppableImageView.cpp
        src/Exam/QuestionDBManager.cpp
        src/SectionEdit.cpp
        src/SectionDialog.cpp
        src/ExamNameAsker.cpp)
set(QuizMan_FORMS
        resources/ui/IntroWindow.ui
        resources/ui/MainWindow.ui
        resources/ui/QuestionForm.ui
        resources/ui/QuestionEditor.ui
        resources/ui/DroppableImageView.ui
        resources/ui/SectionDialog.ui
        resources/ui/SectionEdit.ui
        resources/ui/ExamNameAsker.ui)
set(QuizMan_HEADERS
        include/IntroWindow.hpp
        include/Account.hpp
        include/Account/UserAccount.hpp
        include/Account/AccountManager.hpp
        include/ProgramConstants.hpp
        include/MainWindow.hpp
        include/Exam/QuizManExam.hpp
        include/Exam/Question.hpp
        include/Exam/Option.hpp
        include/Exam/ExamManager.hpp
        include/Exam.hpp
        include/QuestionForm.hpp
        include/QuestionEditor.hpp
        include/DroppableImageView.hpp
        include/Exam/QuestionDBManager.hpp
        include/SectionDialog.hpp
        include/SectionEdit.hpp
        include/ExamNameAsker.hpp)
set(UI_RESOURCES
        resources/resources.qrc
)

qt5_wrap_cpp(QuizMan_HEADERS_MOC ${QuizMan_HEADERS})
qt5_wrap_ui(QuizMan_FORMS_HEADERS ${QuizMan_FORMS})
qt5_add_resources(UI_RESOURCES_RCC ${UI_RESOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_executable(QuizMan
        ${QuizMan_SOURCES}
        ${QuizMan_HEADERS_MOC}
        ${QuizMan_FORMS_HEADERS}
        ${UI_RESOURCES_RCC}
)

target_link_libraries(QuizMan Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql)

# Install target
#install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)