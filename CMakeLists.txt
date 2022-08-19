cmake_minimum_required(VERSION 3.2)

project(import-html-template)

if (UNIX)
	set(CMAKE_CXX_FLAGS "-std=c++17 -lstdc++fs")
endif()

file(GLOB source "src/*.cpp")
# file(GLOB include "src/*.hpp")

file(GLOB parser_source "src/parser/*.cpp")
file(GLOB parser_include "src/parser/*.hpp")

file(GLOB util_source "src/util/*.cpp")
file(GLOB util_include "src/util/*.hpp")

# file(GLOB thread_source "src/thread/*.cpp")
file(GLOB thread_include "src/thread/*.hpp")

add_executable(import-html-template 
  "${source}" 
  "${parser_source}" "${parser_include}"  
  "${util_source}" "${util_include}" 
  "${thread_include}"
  )

# cmake -DCMAKE_CXX_COMPILER=g++-8

target_include_directories(import-html-template PRIVATE 
  "./src"
)

if(WIN32)
	target_compile_options(import-html-template PRIVATE "-std:c++17")
endif()

if (UNIX)
    target_link_libraries(import-html-template stdc++fs)
    target_link_libraries(import-html-template pthread)
endif()
