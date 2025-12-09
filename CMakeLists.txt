cmake_minimum_required(VERSION 4.0)
project(storage_simulator_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(storage_simulator_cpp
        main.cpp
        storage.cpp
)
