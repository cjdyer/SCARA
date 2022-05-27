cmake_minimum_required(VERSION 3.3)
project(SCARA)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build/debug")

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
include_directories(include)

find_library(wiringPi_LIB wiringPi)

file(GLOB sources src/*.cpp)

add_executable(SCARA ${sources}) 

target_link_libraries(SCARA PRIVATE Threads::Threads)

target_link_libraries(SCARA PRIVATE wiringPi)