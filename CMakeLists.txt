cmake_minimum_required(VERSION 3.3)
project(QuizMan)

# C++ Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

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
        src/Account/UserAccount.cpp)
set(QuizMan_FORMS
        resources/ui/MainWindow.ui
        resources/ui/IntroWindow.ui)
set(QuizMan_HEADERS
        include/IntroWindow.hpp
        include/MainWindow.hpp
        include/Account/AccountManager.hpp
        include/Account/UserAccount.hpp
        include/Account.hpp
        include/ProgramConstants.hpp)

qt5_wrap_cpp(QuizMan_HEADERS_MOC ${QuizMan_HEADERS})
qt5_wrap_ui(QuizMan_FORMS_HEADERS ${QuizMan_FORMS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_executable(QuizMan
        ${QuizMan_SOURCES}
        ${QuizMan_HEADERS_MOC}
        ${QuizMan_FORMS_HEADERS}
)
#target_link_libraries(QuizMan ${QT_LIBRARIES})

#qt5_use_modules(QuizMan Widgets Core Gui Sql)
target_link_libraries(QuizMan Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql)

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${PROJECT_SOURCE_DIR})