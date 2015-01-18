cmake_minimum_required(VERSION 2.6)

option(VARIANT_RUN_TESTS "Build all tests." OFF)

project(variant)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

if (VARIANT_RUN_TESTS)
    add_subdirectory(tests)
    enable_testing()
endif()
