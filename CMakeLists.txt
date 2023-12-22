cmake_minimum_required(VERSION 3.19)
project(Airport)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(SOURCE_FILES
        Cities.cpp
        Flight.cpp
        Infrastructure.h
        Runway.cpp
        Gate.cpp
        Airport.cpp
        FileManager.cpp

        testy.cpp

        GoogleTest/gtest-death-test.cc
        GoogleTest/gtest-filepath.cc
        GoogleTest/gtest-port.cc
        GoogleTest/gtest-printers.cc
        GoogleTest/gtest-test-part.cc
        GoogleTest/gtest-typed-test.cc
        GoogleTest/gtest.cc
        GoogleTest/gtest_main.cc
        )

include_directories(GoogleTest)
add_executable(Airport ${SOURCE_FILES})
target_link_libraries(Airport ${CMAKE_THREAD_LIBS_INIT})