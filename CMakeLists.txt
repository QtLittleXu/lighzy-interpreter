cmake_minimum_required(VERSION 3.5)
project(lighzy-interpreter LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LI_PROGRAM li)
set(LI_LIBRARY lilib)
configure_file(config.h.in config.h)

option(BUILD_TEST "build the target test" ON)

find_package(nlohmann_json REQUIRED)
find_package(argparse REQUIRED)

set(CMAKE_CXX_FLAGS "-ftemplate-backtrace-limit=1")
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstandalone-debug")
endif()

# src
set(LIBRARY_SOURCES
    src/lexer/Token.cpp
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/object/basic/Object.cpp
    src/evaluator/Evaluator.cpp
    src/evaluator/BuiltinFuns.cpp
)

set(PROGRAM_SOURCES
    src/program/Program.cpp
    src/main.cpp)

add_library(${LI_LIBRARY} OBJECT ${LIBRARY_SOURCES})
target_include_directories(${LI_LIBRARY} PUBLIC
    include
    ${PROJECT_BINARY_DIR})

add_executable(${LI_PROGRAM} ${PROGRAM_SOURCES})
target_link_libraries(${LI_PROGRAM} PRIVATE
    argparse::argparse
    ${LI_LIBRARY})

# test
if (BUILD_TEST)
	add_subdirectory(test)
endif()
