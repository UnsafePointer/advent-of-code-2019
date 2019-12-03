cmake_minimum_required(VERSION 3.10.2)
project(cmake-cpp-template)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})
