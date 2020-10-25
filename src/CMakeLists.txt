cmake_minimum_required(VERSION 3.2)

project(import-html-template)

if (UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs")
endif()

file(GLOB main "*.cpp")
file(GLOB parser "parser/*.cpp")
file(GLOB util "util/*.cpp")
file(GLOB thread "thread/*.cpp")

add_executable(import-html-template "${main}" "${parser}" "${util}" "${thread}")

# cmake -DCMAKE_CXX_COMPILER=g++-8

target_include_directories(import-html-template PRIVATE main)
target_include_directories(import-html-template PRIVATE parser)
target_include_directories(import-html-template PRIVATE util)
target_include_directories(import-html-template PRIVATE thread)

if(WIN32)
	target_compile_options(import-html-template PRIVATE "-std:c++17")
endif()

if (UNIX)
    target_link_libraries(import-html-template stdc++fs)
    target_link_libraries(import-html-template pthread)
endif()