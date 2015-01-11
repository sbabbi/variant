cmake_minimum_required(VERSION 2.6)
project(variant)

add_executable(variant main.cpp)

install(TARGETS variant RUNTIME DESTINATION bin)
