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
        src/MainWindow.cpp
        src/Account/AccountManager.cpp
        src/Account/UserAccount.cpp
        src/Exam/QuizManExam.cpp
        src/Exam/Question.cpp
        src/Exam/Option.cpp
        src/Exam/ExamManager.cpp
        src/RegistryDialog.cpp
        src/RegistryEdit.cpp)
set(QuizMan_FORMS
        resources/ui/MainWindow.ui
        resources/ui/IntroWindow.ui
        resources/ui/RegistryDialog.ui
        resources/ui/RegistryEdit.ui)
set(QuizMan_HEADERS
        include/IntroWindow.hpp
        include/MainWindow.hpp
        include/Account/AccountManager.hpp
        include/Account/UserAccount.hpp
        include/Account.hpp
        include/ProgramConstants.hpp
        include/Exam/QuizManExam.hpp
        include/Exam/Question.hpp
        include/Exam/Option.hpp
        include/Exam/ExamManager.hpp
        include/Exam.hpp
        include/RegistryDialog.hpp
        include/RegistryEdit.hpp)
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