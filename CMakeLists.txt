cmake_minimum_required(VERSION 3.3)
project(QuizMan)

# C++ Flags
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (WIN32)
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -ldl")
endif (UNIX)

# Set executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(SOURCE_FILES    tests/SimpleClient.cpp
                    src/QuizMan.cpp
                    src/QuizMan.hpp
                    src/sqlite3/sqlite3.c
                    src/SQLiteCpp/Column.cpp
                    src/SQLiteCpp/Database.cpp
                    src/SQLiteCpp/Statement.cpp
                    src/SQLiteCpp/Transaction.cpp
                    src/SQLiteCpp/Column.h
                    src/SQLiteCpp/Database.h
                    src/SQLiteCpp/Statement.h
                    src/SQLiteCpp/Transaction.h
                    src/sqlite3/sqlite3.h)
add_executable(QuizMan ${SOURCE_FILES})

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${PROJECT_SOURCE_DIR})