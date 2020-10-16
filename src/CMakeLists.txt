cmake_minimum_required(VERSION 3.2)

project(import-html-template)

file(GLOB util "util/*.cpp")
file(GLOB parser "parser/*.cpp")
file(GLOB thread "thread/*.cpp")
file(GLOB main "*.cpp")

add_executable(import-html-template "${main}" "${parser}" "${util}" "${thread}")

target_include_directories(import-html-template PRIVATE util)
target_include_directories(import-html-template PRIVATE parser)
target_include_directories(import-html-template PRIVATE thread)
target_include_directories(import-html-template PRIVATE main)

target_compile_options(import-html-template PRIVATE "-std:c++17")
